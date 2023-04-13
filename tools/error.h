//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Base class for objects which report their error.
//
//----------------------------------------------------------------------------

#pragma once
#include "strutils.h"

class Error
{
public:
    // Constructor. Specify where to report errors.
    Error(std::ostream* err = nullptr) : _err(err), _saved(nullptr) {}

protected:
    // Report an error.
    void error(const std::string& msg)  { if (_err != nullptr) { *_err << msg << std::endl; } }
    void error(const WString& msg) { if (_err != nullptr) { *_err << msg << std::endl; } }

    // Temporary mute errors.
    void muteErrors() { if (_err != nullptr) { _saved = _err; _err = nullptr; } }
    void restoreErrors() { _err = _saved; _saved = nullptr; }

private:
    std::ostream* _err;
    std::ostream* _saved;
};
