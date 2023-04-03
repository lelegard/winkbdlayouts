//---------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Utility to inspect installed keyboard layout DLL's.
//
//---------------------------------------------------------------------------

#include <windows.h>
#include <psapi.h>
#if defined(min)
    #undef min
#endif
#if defined(max)
    #undef max
#endif
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <map>


//----------------------------------------------------------------------------
// Command line options.
//----------------------------------------------------------------------------

class Options
{
public:
    // Constructor.
    Options(int argc, char* argv[]);

    // Command line options.
    std::string command;
    std::string output;
    bool        inspect_processes;

    // Print help and exits.
    [[noreturn]] void usage() const;

    // Print a fatal error and exit.
    [[noreturn]] void fatal(const std::string& message) const;
};

[[noreturn]] void Options::usage() const
{
    std::cerr << std::endl
        << "Syntax: " << command << " [options]" << std::endl
        << std::endl
        << "Options:" << std::endl
        << std::endl
        << "  -h : display this help text" << std::endl
        << "  -o file : output file name, default is standard output" << std::endl
        << "  -p : inspect all processes in the system for loaded kbd DLL's" << std::endl
        << std::endl;
    ::exit(EXIT_FAILURE);
}

[[noreturn]] void Options::fatal(const std::string& message) const
{
    std::cerr << command << ": " << message << std::endl;
    ::exit(EXIT_FAILURE);
}

Options::Options(int argc, char* argv[]) :
    command(argc < 1 ? "" : argv[0]),
    output(),
    inspect_processes(false)
{
    // Cleanup command name.
    size_t pos = command.find_last_of(":/\\");
    if (pos != std::string::npos) {
        command.erase(0, pos + 1);
    }
    pos = command.find_last_of(".");
    if (pos != std::string::npos) {
        command.resize(pos);
    }

    // Parse arguments.
    for (int i = 1; i < argc; ++i) {
        const std::string arg(argv[i]);
        if (arg == "--help" || arg == "-h") {
            usage();
        }
        else if (arg == "-p") {
            inspect_processes = true;
        }
        else if (arg == "-o" && i + 1 < argc) {
            output = argv[++i];
        }
        else {
            fatal("invalid option '" + arg + "', try --help");
        }
    }
}


//---------------------------------------------------------------------------
// Format a C++ string in a printf-way.
//---------------------------------------------------------------------------

std::string Format(const char* fmt, ...)
{
    va_list ap;

    // Get required output size.
    va_start(ap, fmt);
    int len = ::vsnprintf(nullptr, 0, fmt, ap);
    va_end(ap);

    if (len < 0) {
        return std::string(); // error
    }

    // Actual formatting.
    std::string buf(len + 1, '\0');
    va_start(ap, fmt);
    len = ::vsnprintf(&buf[0], buf.size(), fmt, ap);
    va_end(ap);

    buf.resize(std::max(0, len));
    return buf;
}


//----------------------------------------------------------------------------
// Transform an error code into an error message string.
//----------------------------------------------------------------------------

std::string Error(::DWORD code = ::GetLastError())
{
    std::string message(1024, ' ');
    size_t length = ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, 0, &message[0], ::DWORD(message.size()), nullptr);
    message.resize(std::min(length, message.size()));

    if (!message.empty()) {
        return message;
    }
    else {
        // Message is not found.
        return Format("System error %d (0x%X)", code, code);
    }
}


//---------------------------------------------------------------------------
// Inspect processes
//---------------------------------------------------------------------------

void InspectProcesses(const Options& opt, std::ostream& out)
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

    // More to come...
}


//---------------------------------------------------------------------------
// Main processing
//---------------------------------------------------------------------------

void Inspect(const Options& opt, std::ostream& out)
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
    Options opt(argc, argv);

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
