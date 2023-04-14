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
#include "winutils.h"


//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

Options::Options(int argc, wchar_t* argv[], const WString syntax) :
    Error(argc < 1 ? L"" : FileBaseName(argv[0]) + ": ", &std::cerr),
    command(argc < 1 ? L"" : FileBaseName(argv[0])),
    args(),
    _syntax(syntax),
    _outfile(),
    _out(&std::cout),
    _prompt_on_exit(false)
{
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
}


//----------------------------------------------------------------------------
// Set an output file or use std::cout.
//----------------------------------------------------------------------------

void Options::setOutput(const WString& filename)
{
    closeOutput();
    if (!filename.empty()) {
        _outfile.open(filename);
        if (!_outfile) {
            fatal("cannot create output file " + filename);
        }
        _out = &_outfile;
    }
}

void Options::closeOutput()
{
    if (_out == &_outfile) {
        _outfile.close();
        _out = &std::cout;
    }
}


//----------------------------------------------------------------------------
// Error handling.
//----------------------------------------------------------------------------

[[noreturn]] void Options::usage()
{
    std::cerr << std::endl << "Syntax: " << command << " " << _syntax << std::endl << std::endl;
    ::exit(EXIT_FAILURE);
}

//----------------------------------------------------------------------------
// Exit process, prompt for user input if setPromptOnExit(true) was called.
//----------------------------------------------------------------------------

[[noreturn]] void Options::exit(int status)
{
    closeOutput();
    if (_prompt_on_exit) {
        char c;
        std::cout << "Press return to exit: " << std::flush;
        std::cin.getline(&c, 1);
    }
    Error::exit(status);
}
