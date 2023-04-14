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
#include "error.h"


//----------------------------------------------------------------------------
// Constructor. Specify where to report errors.
//----------------------------------------------------------------------------

Error::Error(const WString& prefix, std::ostream* err) :
    _prefix(prefix),
    _err(err),
    _saved(nullptr)
{
}


//----------------------------------------------------------------------------
// Report an error.
//----------------------------------------------------------------------------

void Error::error(const std::string& msg) const
{
    if (_err != nullptr) {
        *_err << _prefix << msg << std::endl;
    }
}

void Error::error(const WString& msg) const
{
    if (_err != nullptr) {
        *_err << _prefix << msg << std::endl;
    }
}

[[noreturn]] void Error::fatal(const WString& message)
{
    error(message);
    exit(EXIT_FAILURE);
}

[[noreturn]] void Error::exit(int status)
{
    ::exit(status);
}


//----------------------------------------------------------------------------
// Temporary mute errors.
//----------------------------------------------------------------------------

void Error::muteErrors()
{
    if (_err != nullptr) {
        _saved = _err;
        _err = nullptr;
    }
}

void Error::restoreErrors()
{
    if (_saved != nullptr) {
        _err = _saved;
        _saved = nullptr;
    }
}
