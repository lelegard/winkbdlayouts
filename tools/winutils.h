//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Some Windows utilities.
//
//----------------------------------------------------------------------------

#pragma once
#include "strutils.h"

// Transform an error code into an error message string.
WString ErrorText(DWORD code = GetLastError());

// Get the value of an environment variable.
WString GetEnv(const WString& name, const WString& def = L"");

// Get the path of the System32 and system temp directories.
WString GetSystem32();
WString GetSystemTemp();

// Directory name, file name (without directory), file base name (without directory and prefix).
WString FullName(const WString&, bool include_dir = true, bool include_file = true);
WString DirName(const WString&);
WString FileName(const WString&);
WString FileBaseName(const WString&);

// Check if a file or directory exists
bool FileExists(const WString&);

// Check if a path exists and is a directory
bool IsDirectory(const WString&);

// Search files matching a wildcard in a directory.
bool SearchFiles(WStringList& files, const WString& directory, const WString& pattern);

// Get the file name of a module in a process.
WString ModuleFileName(HANDLE process, HMODULE module);

// Get current executable.
inline WString GetCurrentProgram() { return ModuleFileName(GetCurrentProcess(), nullptr); }

// Get a resource string in a module.
WString GetResourceString(const WString& filename, int resource_index);
WString GetResourceString(HMODULE module, int resource_index);

// Check if current process is admin.
bool IsAdmin();

// Restart current program as admin.
bool RestartAsAdmin(const WStringVector& args = WStringVector(), bool wait_process = false);

// Setting the Windows console (DOS or PowerShell) to UTF-8 mode.
// The constructor of an instance of this class saves the state of the console
// and switches to UTF-8. The destructor restores the previous state.
class ConsoleState
{
public:
    ConsoleState();
    ~ConsoleState();
private:
    const UINT _input_cp;
    const UINT _output_cp;
};
