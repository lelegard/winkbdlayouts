//---------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Utility to inspect installed keyboard layout DLL's.
//
//---------------------------------------------------------------------------

#include "options.h"
#include "strutils.h"


//----------------------------------------------------------------------------
// Command line options.
//----------------------------------------------------------------------------

class InspectOptions : public Options
{
public:
    // Constructor.
    InspectOptions(int argc, char* argv[]);

    // Command line options.
    std::string output;
    bool        inspect_processes;
};

InspectOptions::InspectOptions(int argc, char* argv[]) :
    Options(argc, argv,
        "[options]\n"
        "\n"
        "Options:\n"
        "\n"
        "  -h : display this help text\n"
        "  -o file : output file name, default is standard output\n"
        "  -p : inspect all processes in the system for loaded kbd DLL's"),
    output(),
    inspect_processes(false)
{
    // Parse arguments.
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--help" || args[i] == "-h") {
            usage();
        }
        else if (args[i] == "-p") {
            inspect_processes = true;
        }
        else if (args[i] == "-o" && i + 1 < args.size()) {
            output = args[++i];
        }
        else {
            fatal("invalid option '" + args[i] + "', try --help");
        }
    }
}


//---------------------------------------------------------------------------
// Get the file of a module.
//---------------------------------------------------------------------------

std::string ModuleFileName(::HANDLE hproc, ::HMODULE hmod)
{
    std::string name(2048, ' ');
    ::DWORD size = ::GetModuleFileNameExA(hproc, hmod, &name[0], ::DWORD(name.size()));
    name.resize(std::min<size_t>(size, name.size()));
    return name;
}


//---------------------------------------------------------------------------
// Inspect processes
//---------------------------------------------------------------------------

void InspectProcesses(const InspectOptions& opt, std::ostream& out)
{
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
    size_t error_count = 0;
    for (auto pid : pids) {

        // Open access to process.
        if (pid == 0) {
            continue; // why do we get pid zero ?
        }
        ::HANDLE hproc = ::OpenProcess(PROCESS_ALL_ACCESS, false, pid);
        if (hproc == NULL) {
            const ::DWORD err = ::GetLastError();
            std::cerr << opt.command << Format(": error opening process 0x%08X, ", pid) << Error(err) << std::endl;
            error_count++;
            continue;
        }

        // Enumerate all DLL's in this process.
        std::vector<::HMODULE> mods(4096);
        insize = ::DWORD(mods.size() * sizeof(::HMODULE));
        retsize = 0;
        if (!::EnumProcessModules(hproc, &mods[0], insize, &retsize)) {
            const ::DWORD err = ::GetLastError();
            std::cerr << opt.command << Format(": error getting modules from process 0x%08X, ", pid) << Error(err) << std::endl;
            error_count++;
            ::CloseHandle(hproc);
            continue;
        }
        mods.resize(retsize / sizeof(::HMODULE));

        // Get the module names.
        for (auto hmod : mods) {
            const std::string file(ModuleFileName(hproc, hmod));
            const std::string base(ToLower(FileBaseName(file)));
            if (base.length() >= 3 && base.substr(0, 3) == "kbd") {
                out << Format("Process 0x%08X ", pid) << FileName(ModuleFileName(hproc, NULL)) << " uses " << file << std::endl;
            }
        }
        ::CloseHandle(hproc);
    }
    out << "Found " << pids.size() << " processes, " << error_count << " not accessed" << std::endl;
}


//---------------------------------------------------------------------------
// Main processing
//---------------------------------------------------------------------------

void Inspect(const InspectOptions& opt, std::ostream& out)
{
    if (opt.inspect_processes) {
        InspectProcesses(opt, out);
    }
    // More opions later...
}


//---------------------------------------------------------------------------
// Application entry point.
//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Parse command line options.
    InspectOptions opt(argc, argv);

    // Keyboard tables are now identified, generate the source file.
    if (opt.output.empty()) {
        // No output specified, using standard output.
        Inspect(opt, std::cout);
    }
    else {
        // Create the specified output file.
        std::ofstream out(opt.output);
        if (out) {
            Inspect(opt, out);
        }
        else {
            opt.fatal("cannot create " + opt.output);
        }
    }
    return EXIT_SUCCESS;
}
