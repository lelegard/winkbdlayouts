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

// We use wide strings only.
typedef std::wstring WString;

// Format a C++ string in a printf-way.
// Use "%s" for wchar_t* arguments and "%S" for char* arguments.
WString Format(const wchar_t* fmt, ...);

// Length of a string. Size in bytes of it (including trailing null).
size_t StringLength(const wchar_t*);
size_t StringSize(const wchar_t*);

// Case conversions.
WString ToLower(const WString&);
WString ToUpper(const WString&);

// Remove leading and trailing spaces in a string.
void Trim(WString& str, bool begin = true, bool end = true);
WString Trimmed(const WString& str, bool begin = true, bool end = true);

// Check if a string starts or end with a string.
bool StartsWith(const WString&, const WString& prefix);
bool EndsWith(const WString&, const WString& suffix);

// Format a WString literal, as used in a C/C++ source file.
WString WStringLiteral(const wchar_t*);
inline WString WStringLiteral(const WString& s) { return WStringLiteral(s.c_str()); }

// Decode a string as an integer. Return 0 on error.
inline int ToInt(const WString& str) { return _wtoi(str.c_str()); }

// Decode an hexa value with error checking.
template <typename INT_T, typename std::enable_if<std::is_integral<INT_T>::value, int>::type = 0>
bool FromHexa(INT_T& value, const WString& str);

// Hexadecimal digit.
char Hexa(int nibble);

// Hexadecimal dump.
void PrintHexa(std::ostream& out, const void* addr, size_t size, const WString& margin = L"", bool show_addr = false);

// Zero a memory area.
inline void Zero(void* base, size_t size) { memset(base, 0, size); }

// Check if a memory area is not empty and full of zeroes.
bool IsZero(const void* base, size_t size);
bool IsZero(const void* base, const void* end);

// The UTF-8 Byte Order Mark
#define UTF8_BOM "\xEF\xBB\xBF"

// UTF-8 / UTF-16 conversions.
WString ToUTF16(const std::string&);
std::string ToUTF8(const WString&);

inline std::ostream& operator<<(std::ostream& stream, const WString& s) { return stream << ToUTF8(s); }
inline std::ostream& operator<<(std::ostream& stream, const wchar_t* s) { return stream << ToUTF8(s); }
inline WString operator+(const std::string& s1, const WString& s2) { return ToUTF16(s1) + s2; }
inline WString operator+(const WString& s1, const std::string& s2) { return s1 + ToUTF16(s2); }
inline WString operator+(const char* s1, const WString& s2) { return ToUTF16(s1) + s2; }
inline WString operator+(const WString& s1, const char* s2) { return s1 + ToUTF16(s2); }
inline WString operator+(const std::string& s1, const wchar_t* s2) { return ToUTF16(s1) + s2; }
inline WString operator+(const wchar_t* s1, const std::string& s2) { return s1 + ToUTF16(s2); }

// String container types.
typedef std::vector<WString> WStringVector;
typedef std::list<WString>   WStringList;
typedef std::set<WString>    WStringSet;

template <class CONTAINER, class STRING = typename CONTAINER::value_type>
CONTAINER operator+(const CONTAINER& c, const STRING& s);

template <class CONTAINER, class STRING = typename CONTAINER::value_type>
CONTAINER operator+(const STRING& s, const CONTAINER& c);

// Join a container of strings as one single string.
template <class CONTAINER, typename std::enable_if<std::is_same<typename CONTAINER::value_type, WString>::value, int>::type = 0>
WString Join(const CONTAINER& container, const WString& separator, bool noempty = false);

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

template <class CONTAINER, typename std::enable_if<std::is_same<typename CONTAINER::value_type, WString>::value, int>::type>
WString Join(const CONTAINER& container, const WString& separator, bool noempty)
{
    WString res;
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

template <typename INT_T, typename std::enable_if<std::is_integral<INT_T>::value, int>::type>
bool FromHexa(INT_T& value, const WString& str)
{
    value = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        const wchar_t c = str[i];
        if (c >= L'0' && c <= L'9') {
            value = (value << 4) | (c - L'0');
        }
        else if (c >= L'A' && c <= L'F') {
            value = (value << 4) | (10 + c - L'A');
        }
        else if (c >= L'a' && c <= L'f') {
            value = (value << 4) | (10 + c - L'a');
        }
        else if (i == 1 && value == 0 && (c == L'x' || c == L'X')) {
            // Ignore optional prefix 0x or 0X
        }
        else {
            return false;
        }
    }
    return true;
}
