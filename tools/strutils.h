//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Some string utilities.
//
//----------------------------------------------------------------------------

#pragma once
#include "platform.h"

// Format a C++ string in a printf-way.
std::string Format(const char* fmt, ...);

// Length of a WSTRING. Size in bytes of it (including trailing null).
size_t WstringLength(const WCHAR*);
size_t WstringSize(const WCHAR*);

// Case conversions.
std::string ToLower(const std::string&);
std::string ToUpper(const std::string&);

// Join a container of strings.
template <class CONTAINER>
std::string Join(const CONTAINER& container, const std::string& separator = ", ", bool noempty = false);

// Get the value of an environment variable.
std::string GetEnv(const std::string& name, const std::string& def = "");

// File name (without directory), file base name (without directory and prefix).
std::string FileName(const std::string& name);
std::string FileBaseName(const std::string& name);

// Transform an error code into an error message string.
std::string Error(::DWORD code = ::GetLastError());

// Lists of lists of strings. Each list of strings represents a line in a table.
typedef std::vector<std::string> GridLine;
typedef std::list<GridLine> Grid;

// Print a grid of strings. All columns are aligned on their maximum width.
void PrintGrid(std::ostream& out, const Grid& grid, const std::string& margin = "", size_t spacing = 1);

// Hexadecimal digit.
char Hexa(int nibble);

// Hexadecimal dump.
void PrintHexa(std::ostream& out, const void* addr, size_t size, const std::string& margin = "", bool show_addr = false);

//----------------------------------------------------------------------------
// Template expansions...
//----------------------------------------------------------------------------

template <class CONTAINER>
std::string Join(const CONTAINER& container, const std::string& separator, bool noempty)
{
    std::string res;
    for (auto iter = container.begin(); iter != container.end(); ++iter) {
        if (!noempty || !iter->empty()) {
            if (!res.empty()) {
                res.append(separator);
            }
            res.append(*iter);
        }
    }
    return res;
}
