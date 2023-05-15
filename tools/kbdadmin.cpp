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
#include "kbdinstall.h"
#include "registry.h"
#include "strutils.h"
#include "winutils.h"
#include "grid.h"
#include "kbdrc.h"

// Configure the terminal console on init, restore on exit.
ConsoleState state;


//----------------------------------------------------------------------------
// Command line options.
//----------------------------------------------------------------------------

class AdminOptions : public Options
{
public:
    // Constructor.
    AdminOptions(int argc, wchar_t* argv[]);

    // Command line options.
    WString       output;
    WStringVector dll_install;
    WString       activate;
    bool          remove_wkl;
    bool          list_keyboards;
    bool          show_user;
    bool          search_active;
};

AdminOptions::AdminOptions(int argc, wchar_t* argv[]) :
    Options(argc, argv,
        L"[options]\n"
        L"\n"
        L"Options:\n"
        L"\n"
        L"  -a name : activate the specified keyboard DLL\n"
        L"  -i dll-or-directory : install specified keyboard DLL\n"
        L"  -o file : output file name, default is standard output\n"
        L"  -h  : display this help text\n"
        L"  -l  : list installed keyboards\n"
        L"  -p  : prompt user on end of execution\n"
        L"  -np : ignore -p, don't prompt\n"
        L"  -r  : remove all installed keyboard DLL's from this project\n"
        L"  -s  : search active keyboard layout DLL's in all processes\n"
        L"  -u  : display user setup\n"
        L"  -v  : verbose messages"),
    output(),
    dll_install(),
    activate(),
    remove_wkl(false),
    list_keyboards(false),
    show_user(false),
    search_active(false)
{
    bool dont_prompt = false;

    // Parse arguments.
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == L"--help" || args[i] == L"-h") {
            usage();
        }
        else if (args[i] == L"-l") {
            list_keyboards = true;
        }
        else if (args[i] == L"-s") {
            search_active = true;
        }
        else if (args[i] == L"-u") {
            show_user = true;
        }
        else if (args[i] == L"-r") {
            remove_wkl = true;
        }
        else if (args[i] == L"-v") {
            setVerbose(true);
        }
        else if (args[i] == L"-p") {
            setPromptOnExit(!dont_prompt);
        }
        else if (args[i] == L"-np") {
            dont_prompt = true;
            setPromptOnExit(false);
        }
        else if (args[i] == L"-a" && i + 1 < args.size()) {
            activate = args[++i];
        }
        else if (args[i] == L"-o" && i + 1 < args.size()) {
            output = args[++i];
        }
        else if (args[i] == L"-i" && i + 1 < args.size()) {
            dll_install.push_back(args[++i]);
        }
        else {
            fatal("invalid option '" + args[i] + "', try --help");
        }
    }

    // Default action if nothing is specified.
    if (!list_keyboards && !show_user && !search_active && !remove_wkl && dll_install.empty() && activate.empty()) {
        // If the exe is named "setup.exe", default to "-i same-directory-as-exe".
        const WString exe(GetCurrentProgram());
        if (ToLower(FileName(exe)) == L"setup.exe") {
            dll_install.push_back(DirName(exe));
            setPromptOnExit(!dont_prompt);
        }
        else {
            fatal(L"no option specified, try --help");
        }
    }
}


//---------------------------------------------------------------------------
// List available keyboards
//---------------------------------------------------------------------------

void ListKeyboards(AdminOptions& opt)
{
    // Enumerate keyboard layouts in registry.
    Registry reg(opt);
    WStringList all_lang_ids;
    if (!reg.getSubKeys(REGISTRY_LAYOUT_KEY, all_lang_ids)) {
        return;
    }

    Grid grid(L"", L"  ");
    grid.addLine({L"Lang id", L"Lang", L"KLID", L"File", L"Description"});
    grid.addUnderlines();

    for (const auto& lang_id : all_lang_ids) {
        const WString key(REGISTRY_LAYOUT_KEY "\\" + lang_id);
        const WString file(reg.getValue(key, REGISTRY_LAYOUT_FILE, true));
        const WString layout_id(reg.getValue(key, REGISTRY_LAYOUT_ID, L"", false));
        WString text(reg.getValue(key, REGISTRY_LAYOUT_DISPLAY, L"", true));
        if (text.empty() || text[0] == L'@') {
            text = reg.getValue(key, REGISTRY_LAYOUT_TEXT, L"", false);
        }
        WString lang(FileBaseName(ToLower(file)));
        if (StartsWith(lang, L"kbd")) {
            lang.erase(0, 3);
        }
        else {
            lang.clear();
        }
        grid.addLine({lang_id, lang, layout_id, file, text});
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
    Registry reg(opt);
    WStringList names;
    if (!reg.getValueNames(REGISTRY_USER_PRELOAD_KEY, names)) {
        return;
    }

    Grid grid(L"", L"  ");
    for (const auto& n : names) {
        const WString id(reg.getValue(REGISTRY_USER_PRELOAD_KEY, n, false));
        grid.addLine({n + L":", id});
        const WString idkey(REGISTRY_LAYOUT_KEY "\\" + id);
        if (reg.keyExists(idkey)) {
            grid.addColumn(reg.getValue(idkey, REGISTRY_LAYOUT_TEXT, L"", true) + L" (" + reg.getValue(idkey, REGISTRY_LAYOUT_FILE, L"", true) + L")");
        }
    }

    opt.out() << std::endl
              << "Preload" << std::endl
              << "-------" << std::endl;
    grid.print(opt.out());

    // Enumerate user's substitutions.
    if (!reg.getValueNames(REGISTRY_USER_SUBSTS_KEY, names)) {
        return;
    }

    grid.clear();
    for (const auto& n : names) {
        const WString id(reg.getValue(REGISTRY_USER_SUBSTS_KEY, n, false));
        grid.addLine({n, L"->", id});
        const WString idkey(REGISTRY_LAYOUT_KEY "\\" + id);
        if (reg.keyExists(idkey)) {
            grid.addColumn(reg.getValue(idkey, REGISTRY_LAYOUT_TEXT, L"", true) + L" (" + reg.getValue(idkey, REGISTRY_LAYOUT_FILE, L"", true) + L")");
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
    // Enumerate keyboard layouts in registry.
    Registry reg(opt);
    WStringList all_lang_ids;
    if (!reg.getSubKeys(REGISTRY_LAYOUT_KEY, all_lang_ids)) {
        return;
    }

    // Get all known keybord DLL's.
    WStringSet known_dlls;
    for (const auto& id : all_lang_ids) {
        const WString key(REGISTRY_LAYOUT_KEY "\\" + id);
        const WString file(reg.getValue(key, REGISTRY_LAYOUT_FILE, L"", true));
        if (!file.empty()) {
            known_dlls.insert(ToLower(FileName(file)));
        }
    }

    // Get all process ids in the system.
    std::vector<DWORD> pids(4096);
    DWORD insize = DWORD(pids.size() * sizeof(DWORD));
    DWORD retsize = 0;
    if (!EnumProcesses(&pids[0], insize, &retsize)) {
        const DWORD err = GetLastError();
        opt.fatal("EnumProcesses: " + ErrorText(err));
    }
    pids.resize(retsize / sizeof(DWORD));

    // Loop on all processes.
    opt.out() << std::endl;
    size_t error_count = 0;
    for (auto pid : pids) {

        // Open access to process.
        if (pid == 0) {
            continue; // why do we get pid zero ?
        }
        HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
        if (hproc == NULL) {
            const DWORD err = GetLastError();
            error_count++;
            continue;
        }

        // Enumerate all DLL's in this process.
        std::vector<HMODULE> mods(4096);
        insize = DWORD(mods.size() * sizeof(HMODULE));
        retsize = 0;
        if (!EnumProcessModules(hproc, &mods[0], insize, &retsize)) {
            error_count++;
            CloseHandle(hproc);
            continue;
        }
        mods.resize(retsize / sizeof(HMODULE));

        // Get the module names and display potential keyboards DLL's.
        for (auto hmod : mods) {
            const WString file(ModuleFileName(hproc, hmod));
            if (known_dlls.find(ToLower(FileName(file))) != known_dlls.end()) {
                opt.out() << "Process " << FileName(ModuleFileName(hproc, nullptr)) << ", PID " << pid << ", uses " << file << std::endl;
            }
        }
        CloseHandle(hproc);
    }
    opt.verbose(Format(L"Found %d processes, %d cannot be accessed", pids.size(), error_count));
}


//---------------------------------------------------------------------------
// Activate a keyboard DLL.
//---------------------------------------------------------------------------

void ActivateKeyboard(AdminOptions& opt)
{
    // Get DLL file name, lower case.
    WString dllname(ToLower(FileName(opt.activate)));
    if (!StartsWith(dllname, L"kbd")) {
        dllname.insert(0, L"kbd");
    }
    if (!EndsWith(dllname, L".dll")) {
        dllname.append(L".dll");
    }

    // Search which keyboard layout has this DLL name.
    Registry reg(opt);
    WStringList all_lang_ids;
    if (reg.getSubKeys(REGISTRY_LAYOUT_KEY, all_lang_ids)) {
        for (const auto& lang_id : all_lang_ids) {
            const WString key(REGISTRY_LAYOUT_KEY "\\" + lang_id);
            const WString file(reg.getValue(key, REGISTRY_LAYOUT_FILE, true));
            DWORD int_id = 0;
            if (ToLower(file) == dllname && FromHexa(int_id, lang_id)) {
                // Found the right language.
                //
                // WARNING: the code below does not work as expected.
                // The selected keyboard lyaout is not activated at system level.
                // This is exploratory code only. The proper solution is still to be found.
                //
                opt.info(L"Activating language " + lang_id + " (" + dllname + ")");
                HKL hkl = LoadKeyboardLayoutW(lang_id.c_str(), KLF_ACTIVATE);
                if (hkl == NULL) {
                    const DWORD err = GetLastError();
                    opt.fatal("LoadKeyboardLayout: " + ErrorText(err));
                }
                if (!SystemParametersInfoW(SPI_SETDEFAULTINPUTLANG, 0, &hkl, SPIF_SENDCHANGE)) {
                    const DWORD err = GetLastError();
                    opt.fatal("SystemParametersInfo: " + ErrorText(err));
                }
                if (!PostMessage(HWND_BROADCAST, WM_INPUTLANGCHANGEREQUEST, 0, LPARAM(int_id)) ||
                    !PostMessage(HWND_BROADCAST, WM_INPUTLANGCHANGE, 0, LPARAM(int_id)))
                {
                    const DWORD err = GetLastError();
                    opt.fatal("PostMessage(HWND_BROADCAST): " + ErrorText(err));
                }
                return;
            }
        }
        opt.error(L"cannot find a language id for " + dllname);
    }
}


//---------------------------------------------------------------------------
// Install one keyboard DLL's.
//---------------------------------------------------------------------------

void InstallOneKeyboard(AdminOptions& opt, const WString& dll_path)
{
    // Get all expected resource strings from a WKL DLL.
    HMODULE hmod = LoadLibraryExW(dll_path.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE);
    if (hmod == nullptr) {
        const DWORD err = GetLastError();
        opt.error(dll_path + ": " + ErrorText(err));
        return;
    }
    const WString dll_name(ToLower(FileName(dll_path)));
    const WString dll_text(GetResourceString(hmod, WKL_RES_TEXT));
    const WString dll_provider(GetResourceString(hmod, WKL_RES_PROVIDER));
    WString dll_lang_id(ToLower(GetResourceString(hmod, WKL_RES_LANG)));
    int base_language = 0;
    FromHexa(base_language, dll_lang_id);
    FreeLibrary(hmod);

    // A WKL DLL is expected to have resource strings: "WKL" as provider and non-null language id.
    if (dll_provider != L"WKL" || base_language == 0) {
        return;
    }

    // Install the keyboard layout DLL.
    opt.info("==== Installing " + dll_name + " (" + dll_text + ")");
    const uint32_t lang_id = InstallKeyboardLayout(opt, dll_path, base_language, dll_text);

    // Add specific WKL entries in the registry.
    if (lang_id != 0) {
        Registry reg(opt);
        const WString key(REGISTRY_LAYOUT_KEY L"\\" + Format(L"%08x", lang_id));
        reg.setValue(key, REGISTRY_LAYOUT_PROVIDER, dll_provider);
        reg.setValue(key, REGISTRY_LAYOUT_DISPLAY, L"@%SystemRoot%\\system32\\" + dll_name + Format(L",-%d", WKL_RES_TEXT), true);
    }
}


//---------------------------------------------------------------------------
// Install all specified keyboard DLL's.
//---------------------------------------------------------------------------

void InstallKeyboards(AdminOptions& opt)
{
    for (const auto& path : opt.dll_install) {
        if (IsDirectory(path)) {
            WStringList files;
            SearchFiles(files, path, L"*.dll");
            for (const auto& file : files) {
                InstallOneKeyboard(opt, path + L"\\" + file);
            }
        }
        else {
            InstallOneKeyboard(opt, path);
        }
    }
}


//---------------------------------------------------------------------------
// Remove all installed WKL keyboard DLL's.
//---------------------------------------------------------------------------

void RemoveKeyboards(AdminOptions& opt)
{
    // Enumerate keyboard layouts in registry and remove all WKL keyboads.
    Registry reg(opt);
    WStringList all_lang_ids;
    if (reg.getSubKeys(REGISTRY_LAYOUT_KEY, all_lang_ids)) {
        for (const auto& lang_id : all_lang_ids) {
            const WString file(reg.getValue(REGISTRY_LAYOUT_KEY L"\\" + lang_id, REGISTRY_LAYOUT_FILE, L"", true));
            if (!file.empty() && GetResourceString(GetSystem32() + L"\\" + file, WKL_RES_PROVIDER) == L"WKL") {
                UninstallKeyboardLayout(opt, file);
            }
        }
    }
}


//---------------------------------------------------------------------------
// Application entry point.
//---------------------------------------------------------------------------

int wmain(int argc, wchar_t* argv[])
{
    // Parse command line options.
    AdminOptions opt(argc, argv);

    // Need to be admin to install keyboards or explore all processes.
    if ((!opt.dll_install.empty() || opt.remove_wkl || opt.search_active) && !IsAdmin()) {
        opt.info("Restarting as admin...");
        RestartAsAdmin(opt.args + L"-p", true);
        opt.exit(EXIT_SUCCESS);
    }

    // Now perform all requested operations.
    opt.setOutput(opt.output);
    if (opt.remove_wkl) {
        RemoveKeyboards(opt);
    }
    if (!opt.dll_install.empty()) {
        InstallKeyboards(opt);
    }
    if (opt.list_keyboards) {
        ListKeyboards(opt);
    }
    if (opt.show_user) {
        DisplayUserSetup(opt);
    }
    if (!opt.activate.empty()) {
        ActivateKeyboard(opt);
    }
    if (opt.search_active) {
        SearchActiveKeyboards(opt);
    }
    opt.out() << std::endl;
    opt.exit(EXIT_SUCCESS);
}
