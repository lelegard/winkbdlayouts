//---------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Utility to manage or install keyboard layout DLL's.
//
//---------------------------------------------------------------------------

#include "options.h"
#include "registry.h"
#include "strutils.h"
#include "winutils.h"
#include "grid.h"

// Registry entry of all keyboard layouts.
#define REGISTRY_LAYOUT_KEY       "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts"
#define REGISTRY_USER_PRELOAD_KEY "HKEY_CURRENT_USER\\Keyboard Layout\\Preload"
#define REGISTRY_USER_SUBSTS_KEY  "HKEY_CURRENT_USER\\Keyboard Layout\\Substitutes"
#define REGISTRY_LAYOUT_FILE      "Layout File"
#define REGISTRY_LAYOUT_TEXT      "Layout Text"


//----------------------------------------------------------------------------
// Command line options.
//----------------------------------------------------------------------------

class AdminOptions : public Options
{
public:
    // Constructor.
    AdminOptions(int argc, char* argv[]);

    // Command line options.
    std::string output;
    bool        list_keyboards;
    bool        show_user;
    bool        search_active;
};

AdminOptions::AdminOptions(int argc, char* argv[]) :
    Options(argc, argv,
        "[options]\n"
        "\n"
        "Options:\n"
        "\n"
        "  -h : display this help text\n"
        "  -l : list installed keyboards\n"
        "  -o file : output file name, default is standard output\n"
        "  -p : prompt user on end of execution\n"
        "  -np : ignore -p, don't prompt\n"
        "  -s : search active keyboard layout DLL's in all processes\n"
        "  -u : display user setup"),
    output(),
    list_keyboards(false),
    show_user(false),
    search_active(false)
{
    bool dont_prompt = false;

    // Parse arguments.
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--help" || args[i] == "-h") {
            usage();
        }
        else if (args[i] == "-l") {
            list_keyboards = true;
        }
        else if (args[i] == "-s") {
            search_active = true;
        }
        else if (args[i] == "-u") {
            show_user = true;
        }
        else if (args[i] == "-p") {
            setPromptOnExit(!dont_prompt);
        }
        else if (args[i] == "-np") {
            dont_prompt = true;
            setPromptOnExit(false);
        }
        else if (args[i] == "-o" && i + 1 < args.size()) {
            output = args[++i];
        }
        else {
            fatal("invalid option '" + args[i] + "', try --help");
        }
    }

    // Error if nothing is specified.
    if (!list_keyboards && !show_user && !search_active) {
        fatal("no option specified, try --help");
    }
}


//---------------------------------------------------------------------------
// List available keyboards
//---------------------------------------------------------------------------

void ListKeyboards(AdminOptions& opt)
{
    // Enumerate keyboard layouts in registry.
    Registry reg(&std::cerr);
    StringList layout_ids;
    if (!reg.getSubKeys(REGISTRY_LAYOUT_KEY, layout_ids)) {
        return;
    }

    Grid grid("", "  ");
    grid.addLine({ "Id", "Lang", "File", "Description" });
    grid.addUnderlines();

    for (const auto& id : layout_ids) {
        const std::string file(reg.getValue(REGISTRY_LAYOUT_KEY "\\" + id, REGISTRY_LAYOUT_FILE));
        const std::string text(reg.getValue(REGISTRY_LAYOUT_KEY "\\" + id, REGISTRY_LAYOUT_TEXT));
        std::string lang(FileBaseName(ToLower(file)));
        if (StartsWith(lang, "kbd")) {
            lang.erase(0, 3);
        }
        else {
            lang.clear();
        }
        grid.addLine({id, lang, file, text});
    }
    opt.out() << std::endl;
    grid.print(opt.out());
}


//---------------------------------------------------------------------------
// Display user setup.
//---------------------------------------------------------------------------

void DisplayUserSetup(AdminOptions& opt)
{
    // Enumerate user's preloads in registry.
    Registry reg(&std::cerr);
    StringList names;
    if (!reg.getValueNames(REGISTRY_USER_PRELOAD_KEY, names)) {
        return;
    }

    Grid grid("", "  ");
    for (const auto& n : names) {
        const std::string id(reg.getValue(REGISTRY_USER_PRELOAD_KEY, n));
        grid.addLine({n + ":", id});
        const std::string idkey(REGISTRY_LAYOUT_KEY "\\" + id);
        if (reg.keyExists(idkey)) {
            grid.addColumn(reg.getValue(idkey, REGISTRY_LAYOUT_TEXT) + " (" + reg.getValue(idkey, REGISTRY_LAYOUT_FILE) + ")");
        }
    }

    opt.out() << std::endl
              << "Preload" << std::endl
              << "-------" << std::endl;
    grid.print(opt.out());

    // Enumertate user's substitutions.
    if (!reg.getValueNames(REGISTRY_USER_SUBSTS_KEY, names)) {
        return;
    }

    grid.clear();
    for (const auto& n : names) {
        const std::string id(reg.getValue(REGISTRY_USER_SUBSTS_KEY, n));
        grid.addLine({n, "->", id});
        const std::string idkey(REGISTRY_LAYOUT_KEY "\\" + id);
        if (reg.keyExists(idkey)) {
            grid.addColumn(reg.getValue(idkey, REGISTRY_LAYOUT_TEXT) + " (" + reg.getValue(idkey, REGISTRY_LAYOUT_FILE) + ")");
        }
    }

    opt.out() << std::endl
              << "Substitutes" << std::endl
              << "-----------" << std::endl;
    grid.print(opt.out());
}


//---------------------------------------------------------------------------
// Search active keyboard DLL's in all processes.
//---------------------------------------------------------------------------

void SearchActiveKeyboards(AdminOptions& opt)
{
    // Need to be admin to explore all processes.
    if (!IsAdmin()) {
        opt.closeOutput();
        std::cout << "Restarting as admin..." << std::endl;
        RestartAsAdmin(opt.args + "-p", true);
    }
    else {
        // Get all process ids in the system.
        std::vector<::DWORD> pids(4096);
        ::DWORD insize = ::DWORD(pids.size() * sizeof(::DWORD));
        ::DWORD retsize = 0;
        if (!::EnumProcesses(&pids[0], insize, &retsize)) {
            const ::DWORD err = ::GetLastError();
            opt.fatal("EnumProcesses error: " + Error(err));
        }
        pids.resize(retsize / sizeof(::DWORD));

        // Loop on all processes.
        opt.out() << std::endl;
        size_t error_count = 0;
        for (auto pid : pids) {

            // Open access to process.
            if (pid == 0) {
                continue; // why do we get pid zero ?
            }
            ::HANDLE hproc = ::OpenProcess(PROCESS_ALL_ACCESS, false, pid);
            if (hproc == NULL) {
                const ::DWORD err = ::GetLastError();
                error_count++;
                continue;
            }

            // Enumerate all DLL's in this process.
            std::vector<::HMODULE> mods(4096);
            insize = ::DWORD(mods.size() * sizeof(::HMODULE));
            retsize = 0;
            if (!::EnumProcessModules(hproc, &mods[0], insize, &retsize)) {
                const ::DWORD err = ::GetLastError();
                error_count++;
                ::CloseHandle(hproc);
                continue;
            }
            mods.resize(retsize / sizeof(::HMODULE));

            // Get the module names and display potential keyboards DLL's.
            for (auto hmod : mods) {
                const std::string file(ModuleFileName(hproc, hmod));
                const std::string base(ToLower(FileName(file)));
                if (StartsWith(base, "kbd") && EndsWith(base, ".dll")) {
                    opt.out() << Format("Process 0x%08X ", pid)
                              << FileName(ModuleFileName(hproc, nullptr))
                              << " uses " << file << std::endl;
                }
            }
            ::CloseHandle(hproc);
        }
        opt.out() << "Found " << pids.size() << " processes, " << error_count << " cannot be accessed" << std::endl;
    }
}


//---------------------------------------------------------------------------
// Application entry point.
//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Parse command line options.
    AdminOptions opt(argc, argv);
    opt.setOutput(opt.output);

    if (opt.list_keyboards) {
        ListKeyboards(opt);
    }
    if (opt.show_user) {
        DisplayUserSetup(opt);
    }
    if (opt.search_active) {
        SearchActiveKeyboards(opt);
    }
    opt.out() << std::endl;
    opt.exit(EXIT_SUCCESS);
}
