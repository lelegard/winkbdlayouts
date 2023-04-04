//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Base class for command line options.
//
//----------------------------------------------------------------------------

#pragma once
#include "platform.h"

class Options
{
public:
    // Constructor.
    Options(int argc, char* argv[], const std::string syntax);

    // Command name and arguments.
    const std::string command;
    std::vector<std::string> args;

    // Print help and exits.
    [[noreturn]] void usage() const;

    // Print a fatal error and exit.
    [[noreturn]] void fatal(const std::string& message) const;
    
private:
    const std::string _syntax;
    static std::string CommandName(int argc, char* argv[]);
};
