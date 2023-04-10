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

// Convenience types.
typedef std::vector<std::string> StringVector;
typedef std::list<std::string> StringList;

StringVector operator+(const StringVector&, const std::string&);
StringVector operator+(const std::string&, const StringVector&);
StringList operator+(const StringList&, const std::string&);
StringList operator+(const std::string&, const StringList&);

// Format a C++ string in a printf-way.
std::string Format(const char* fmt, ...);

// Length of a WSTRING. Size in bytes of it (including trailing null).
size_t WstringLength(const WCHAR*);
size_t WstringSize(const WCHAR*);

// Case conversions.
std::string ToLower(const std::string&);
std::string ToUpper(const std::string&);

// Check if a string starts or end with a string.
bool StartsWith(const std::string&, const std::string& prefix);
bool EndsWith(const std::string&, const std::string& suffix);

// Join a container of strings.
template <class CONTAINER>
std::string Join(const CONTAINER& container, const std::string& separator = ", ", bool noempty = false);

// Hexadecimal digit.
char Hexa(int nibble);

// Hexadecimal dump.
void PrintHexa(std::ostream& out, const void* addr, size_t size, const std::string& margin = "", bool show_addr = false);

// Zero a memory area.
inline void Zero(void* base, size_t size) { ::memset(base, 0, size); }

// Check if a memory area is not empty and full of zeroes.
bool IsZero(const void* base, size_t size);
bool IsZero(const void* base, const void* end);

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
