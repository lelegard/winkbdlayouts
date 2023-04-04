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
#include "strutils.h"


//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

Options::Options(int argc, char* argv[], const std::string syntax) :
    command(argc < 1 ? "" : FileBaseName(argv[0])),
    args(),
    _syntax(syntax)
{
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
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
