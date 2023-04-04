//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Base class for command line options.
//
//----------------------------------------------------------------------------

#include "options.h"


//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

Options::Options(int argc, char* argv[], const std::string syntax) :
    command(CommandName(argc, argv)),
    args(),
    _syntax(syntax)
{
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
}


//----------------------------------------------------------------------------
// Static function to extract the command name.
//----------------------------------------------------------------------------

std::string Options::CommandName(int argc, char* argv[])
{
    std::string command(argc < 1 ? "" : argv[0]);

    // Cleanup command name.
    size_t pos = command.find_last_of(":/\\");
    if (pos != std::string::npos) {
        command.erase(0, pos + 1);
    }
    pos = command.find_last_of(".");
    if (pos != std::string::npos) {
        command.resize(pos);
    }

    return command;
}


//----------------------------------------------------------------------------
// Error handling.
//----------------------------------------------------------------------------

[[noreturn]] void Options::usage() const
{
    std::cerr << std::endl << "Syntax: " << command << "  " << _syntax << std::endl << std::endl;
    ::exit(EXIT_FAILURE);
}

[[noreturn]] void Options::fatal(const std::string& message) const
{
    std::cerr << command << ": " << message << std::endl;
    ::exit(EXIT_FAILURE);
}
