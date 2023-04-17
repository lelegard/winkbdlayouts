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
    Error(const WString& prefix = WString(), std::ostream* err = nullptr);

    // Report a message.
    virtual void error(const std::string& msg) const;
    virtual void error(const WString& msg) const;
    virtual void warning(const std::string& msg) const;
    virtual void warning(const WString& msg) const;
    virtual void info(const std::string& msg) const;
    virtual void info(const WString& msg) const;
    virtual void verbose(const std::string& msg) const;
    virtual void verbose(const WString& msg) const;

    // Set verbose mode on/off.
    void setVerbose(bool on) { _verbose = on; }
    bool verbose() const { return _verbose; }

    // Temporary mute errors.
    virtual void muteErrors();
    virtual void restoreErrors();

    // Print a fatal error and exit.
    [[noreturn]] virtual void fatal(const WString& message);

    // Exit process, prompt for user input if setPromptOnExit(true) was called.
    [[noreturn]] virtual void exit(int status = EXIT_SUCCESS);

private:
    WString       _prefix;
    std::ostream* _err;
    std::ostream* _saved;
    bool          _verbose;
};
