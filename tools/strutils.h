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

// Case conversions.
std::string ToLower(const std::string&);
std::string ToUpper(const std::string&);

// Join a container of strings.
template <class CONTAINER>
std::string Join(const CONTAINER& container, const std::string& separator = ", ", bool noempty = false);

// Get the value of an environment variable.
std::string GetEnv(const std::string& name, const std::string& def = "");

// Transform an error code into an error message string.
std::string Error(::DWORD code = ::GetLastError());

// Lists of lists of strings. Each list of strings represents a line in a table.
typedef std::vector<std::string> GridLine;
typedef std::list<GridLine> Grid;

// Print a grid of strings. All columns are aligned on their maximum width.
void PrintGrid(std::ostream& out, const Grid& grid, const std::string& margin = "", size_t spacing = 1);

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
