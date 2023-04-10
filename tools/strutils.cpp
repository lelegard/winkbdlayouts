//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Some string utilities.
//
//----------------------------------------------------------------------------

#include "strutils.h"
#include <cstdarg>
#include <algorithm>


//---------------------------------------------------------------------------
// Operators on string containers.
//---------------------------------------------------------------------------

StringVector operator+(const StringVector& v, const std::string& s)
{
    StringVector res(v);
    res.push_back(s);
    return res;
}

StringVector operator+(const std::string& s, const StringVector& v)
{
    StringVector res;
    res.push_back(s);
    res.insert(res.end(), v.begin(), v.end());
    return res;
}

StringList operator+(const StringList& l, const std::string& s)
{
    StringList res(l);
    res.push_back(s);
    return res;
}

StringList operator+(const std::string& s, const StringList& l)
{
    StringList res;
    res.push_back(s);
    res.insert(res.end(), l.begin(), l.end());
    return res;
}


//---------------------------------------------------------------------------
// Format a C++ string in a printf-way.
//---------------------------------------------------------------------------

std::string Format(const char* fmt, ...)
{
    va_list ap;

    // Get required output size.
    va_start(ap, fmt);
    int len = ::vsnprintf(nullptr, 0, fmt, ap);
    va_end(ap);

    if (len < 0) {
        return std::string(); // error
    }

    // Actual formatting.
    std::string buf(len + 1, '\0');
    va_start(ap, fmt);
    len = ::vsnprintf(&buf[0], buf.size(), fmt, ap);
    va_end(ap);

    buf.resize(std::max(0, len));
    return buf;
}


//----------------------------------------------------------------------------
// Length of a WSTRING. Size in bytes of it (including trailing null).
//----------------------------------------------------------------------------

size_t WstringLength(const WCHAR* s)
{
    if (s == nullptr) {
        return 0;
    }
    else {
        size_t len = 0;
        while (*s++ != 0) {
            len++;
        }
        return len;
    }
}

size_t WstringSize(const WCHAR* s)
{
    return s == nullptr ? 0 : (WstringLength(s) + 1) * sizeof(WCHAR);
}


//----------------------------------------------------------------------------
// Case conversions.
//----------------------------------------------------------------------------

std::string ToLower(const std::string& s)
{
    std::string res(s);
    std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c){ return std::tolower(c); });
    return res;
}

std::string ToUpper(const std::string& s)
{
    std::string res(s);
    std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c){ return std::toupper(c); });
    return res;
}


//---------------------------------------------------------------------------
// Check if a string starts or end with a string.
//---------------------------------------------------------------------------

bool StartsWith(const std::string& s, const std::string& prefix)
{
    return !prefix.empty() && s.length() >= prefix.length() && s.substr(0, prefix.length()) == prefix;
}

bool EndsWith(const std::string& s, const std::string& suffix)
{
    return !suffix.empty() && s.length() >= suffix.length() && s.substr(s.length() - suffix.length()) == suffix;
}


//---------------------------------------------------------------------------
// Check if a memory area is not empty and full of zeroes.
//---------------------------------------------------------------------------

bool IsZero(const void* base, size_t size)
{
    const char* p = reinterpret_cast<const char*>(base);
    return IsZero(p, p + size);
}

bool IsZero(const void* base, const void* end)
{
    if (base == nullptr || end == nullptr || end <= base) {
        return false;
    }
    else {
        for (const char* p = reinterpret_cast<const char*>(base); p < reinterpret_cast<const char*>(end); ++p) {
            if (*p != 0) {
                return false;
            }
        }
        return true;
    }
}


//---------------------------------------------------------------------------
// Hexadecimal dump.
//---------------------------------------------------------------------------

char Hexa(int nibble)
{
    int n = nibble & 0x0F;
    return (n < 10 ? '0' : 'A' - 10) + n;
}

void PrintHexa(std::ostream& out, const void* addr, size_t size, const std::string& margin, bool show_addr)
{
    const uint8_t* cur = reinterpret_cast<const uint8_t*>(addr);
    const uint8_t* end = cur + size;
    constexpr size_t bytes_per_line = 16;

    while (cur < end) {
        const size_t count = std::min<size_t>(bytes_per_line, end - cur);
        out << margin;
        if (show_addr) {
            out << Format("0x%08llX: ", size_t(cur));
        }
        for (size_t i = 0; i < count; ++i) {
            out << Hexa(cur[i] >> 4) << Hexa(cur[i]) << ' ';
        }
        out << std::string(2 + 3 * (bytes_per_line - count), ' ');
        for (size_t i = 0; i < count; ++i) {
            out << char(cur[i] >= ' ' && cur[i] < 0x7F ? cur[i] : '.');
        }
        out << std::endl;
        cur += count;
    }
}
