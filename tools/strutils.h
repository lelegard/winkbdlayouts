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
// Use "%s" for wchar_t* arguments and "%S" for char* arguments.
std::wstring Format(const wchar_t* fmt, ...);

// Length of a string. Size in bytes of it (including trailing null).
size_t StringLength(const wchar_t*);
size_t StringSize(const wchar_t*);

// Case conversions.
std::wstring ToLower(const std::wstring&);
std::wstring ToUpper(const std::wstring&);

// Check if a string starts or end with a string.
bool StartsWith(const std::wstring&, const std::wstring& prefix);
bool EndsWith(const std::wstring&, const std::wstring& suffix);

// Decode a string as an integer.
inline int ToInt(const std::wstring& str) { return _wtoi(str.c_str()); }

// Hexadecimal digit.
char Hexa(int nibble);

// Hexadecimal dump.
void PrintHexa(std::ostream& out, const void* addr, size_t size, const std::wstring& margin = L"", bool show_addr = false);

// Zero a memory area.
inline void Zero(void* base, size_t size) { memset(base, 0, size); }

// Check if a memory area is not empty and full of zeroes.
bool IsZero(const void* base, size_t size);
bool IsZero(const void* base, const void* end);

// UTF-8 / UTF-16 conversions.
std::wstring ToUTF16(const std::string&);
std::string ToUTF8(const std::wstring&);

inline std::ostream& operator<<(std::ostream& stream, const std::wstring& s) { return stream << ToUTF8(s); }
inline std::ostream& operator<<(std::ostream& stream, const wchar_t* s) { return stream << ToUTF8(s); }
inline std::wstring operator+(const std::string& s1, const std::wstring& s2) { return ToUTF16(s1) + s2; }
inline std::wstring operator+(const std::wstring& s1, const std::string& s2) { return s1 + ToUTF16(s2); }
inline std::wstring operator+(const char* s1, const std::wstring& s2) { return ToUTF16(s1) + s2; }
inline std::wstring operator+(const std::wstring& s1, const char* s2) { return s1 + ToUTF16(s2); }
inline std::wstring operator+(const std::string& s1, const wchar_t* s2) { return ToUTF16(s1) + s2; }
inline std::wstring operator+(const wchar_t* s1, const std::string& s2) { return s1 + ToUTF16(s2); }

// String container types.
typedef std::vector<std::wstring> WStringVector;
typedef std::list<std::wstring>   WStringList;

template <class CONTAINER, class STRING = typename CONTAINER::value_type>
CONTAINER operator+(const CONTAINER& c, const STRING& s);

template <class CONTAINER, class STRING = typename CONTAINER::value_type>
CONTAINER operator+(const STRING& s, const CONTAINER& c);

// Join a container of strings as one single string.
template <class CONTAINER, typename std::enable_if<std::is_same<typename CONTAINER::value_type, std::wstring>::value, int>::type = 0>
std::wstring Join(const CONTAINER& container, const std::wstring& separator, bool noempty = false);

//----------------------------------------------------------------------------
// Expansions of templates
//----------------------------------------------------------------------------

template <class CONTAINER, class STRING>
CONTAINER operator+(const CONTAINER& c, const STRING& s)
{
    CONTAINER res(c);
    res.push_back(s);
    return res;
}

template <class CONTAINER, class STRING>
CONTAINER operator+(const STRING& s, const CONTAINER& c)
{
    CONTAINER res;
    res.push_back(s);
    res.insert(res.end(), c.begin(), c.end());
    return res;
}

template <class CONTAINER, typename std::enable_if<std::is_same<typename CONTAINER::value_type, std::wstring>::value, int>::type>
std::wstring Join(const CONTAINER& container, const std::wstring& separator, bool noempty)
{
    std::wstring res;
    for (const auto& s : container) {
        if (!noempty || !s.empty()) {
            if (!res.empty()) {
                res.append(separator);
            }
            res.append(s);
        }
    }
    return res;
}
