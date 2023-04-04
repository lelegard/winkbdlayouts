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
// Inspect processes
//---------------------------------------------------------------------------

void InspectProcesses(const InspectOptions& opt, std::ostream& out)
{
    std::vector<::DWORD> pids(512);

    // Get all process ids in the system. There is no way to anticipate the
    // number of processes and the required space. So, if the pids buffer is
    // filled, assume there are more processes and retry with doubled size.
    for (;;) {
        const ::DWORD insize = ::DWORD(pids.size() * sizeof(::DWORD));
        ::DWORD retsize = 0;
        if (!::EnumProcesses(&pids[0], insize, &retsize)) {
            const ::DWORD err = ::GetLastError();
            opt.fatal("EnumProcesses error: " + Error(err));
        }
        if (retsize < insize) {
            pids.resize(retsize / sizeof(::DWORD));
            break;
        }
        pids.resize(2 * pids.size());
    }

    out << "Found " << pids.size() << " processes" << std::endl;

    // Loop on all processes.
    for (auto pid : pids) {

        // Open access to process.
        if (pid == 0) {
            continue; // why do we get pid zero ?
        }
        ::HANDLE hproc = ::OpenProcess(READ_CONTROL, false, pid);
        if (hproc == NULL) {
            const ::DWORD err = ::GetLastError();
            std::cerr << opt.command << Format(": error opening process 0x%08X, ", pid) << Error(err) << std::endl;
            continue;
        }

        ::CloseHandle(hproc);
    }
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
