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
#include "error.h"

class Options : public Error
{
public:
    // Constructor.
    Options(int argc, wchar_t* argv[], const WString syntax);

    // Command name and arguments.
    const WString command;
    WStringVector args;

    // Set an output file or use std::cout.
    void setOutput(const WString& filename);
    void closeOutput();
    std::ostream& out() { return *_out; }
    
    // Prompt when exit() is called.
    void setPromptOnExit(bool on) { _prompt_on_exit = on; }

    // Exit process, prompt for user input if setPromptOnExit(true) was called.
    [[noreturn]] virtual void exit(int status = EXIT_SUCCESS) override;

    // Print help and exits.
    [[noreturn]] void usage();

private:
    const WString _syntax;
    std::ofstream _outfile;
    std::ostream* _out;
    bool          _prompt_on_exit;
};
