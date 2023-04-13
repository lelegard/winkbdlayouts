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
std::wstring ErrorText(DWORD code = GetLastError());

// Get the value of an environment variable.
std::wstring GetEnv(const std::wstring& name, const std::wstring& def = L"");

// Directory name, file name (without directory), file base name (without directory and prefix).
std::wstring FullName(const std::wstring&, bool include_dir = true, bool include_file = true);
std::wstring DirName(const std::wstring&);
std::wstring FileName(const std::wstring&);
std::wstring FileBaseName(const std::wstring&);

// Check if a file or directory exists
bool FileExists(const std::wstring&);

// Check if a path exists and is a directory
bool IsDirectory(const std::wstring&);

// Search files matching a wildcard in a directory.
bool SearchFiles(WStringList& files, const std::wstring& directory, const std::wstring& pattern);

// Get the file name of a module in a process.
std::wstring ModuleFileName(HANDLE process, HMODULE module);

// Get current executable.
inline std::wstring GetCurrentProgram() { return ModuleFileName(GetCurrentProcess(), nullptr); }

// Get a resource string in a module.
std::wstring GetResourceString(const std::wstring& filename, int resource_index);
std::wstring GetResourceString(HMODULE module, int resource_index);

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
