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

// Get the value of an environment variable.
std::string GetEnv(const std::string& name, const std::string& def = "");

// File name (without directory), file base name (without directory and prefix).
std::string FileName(const std::string& name);
std::string FileBaseName(const std::string& name);

// Transform an error code into an error message string.
std::string Error(::DWORD code = ::GetLastError());

// Get the file name of a module in a process.
std::string ModuleFileName(::HANDLE process, ::HMODULE module);

// Get current executable.
inline std::string GetCurrentProgram() { return ModuleFileName(::GetCurrentProcess(), nullptr); }

// Check if current process is admin.
bool IsAdmin();

// Restart current program as admin.
bool RestartAsAdmin(const StringVector& args = StringVector(), bool wait_process = false);
