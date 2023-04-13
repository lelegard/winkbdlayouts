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
#include <codecvt>


//---------------------------------------------------------------------------
// Format a C++ string in a printf-way.
//---------------------------------------------------------------------------

WString Format(const wchar_t* fmt, ...)
{
    va_list ap;

    // Get required output size.
    va_start(ap, fmt);
    int len = _vsnwprintf(nullptr, 0, fmt, ap);
    va_end(ap);

    if (len < 0) {
        return WString(); // error
    }

    // Actual formatting.
    WString buf(len + 1, '\0');
    va_start(ap, fmt);
    len = _vsnwprintf(&buf[0], buf.size(), fmt, ap);
    va_end(ap);

    buf.resize(std::min<size_t>(buf.size(), std::max(0, len)));
    return buf;
}


//---------------------------------------------------------------------------
// Length of a string. Size in bytes of it (including trailing null).
//---------------------------------------------------------------------------

size_t StringLength(const wchar_t* s)
{
    size_t len = 0;
    if (s != nullptr) {
        while (*s++ != 0) {
            len++;
        }
    }
    return len;
}

size_t StringSize(const wchar_t* s)
{
    return s == nullptr ? 0 : (StringLength(s) + 1) * sizeof(wchar_t);
}


//---------------------------------------------------------------------------
// Case conversions.
//---------------------------------------------------------------------------

WString ToLower(const WString& s)
{
    WString res(s);
    std::transform(res.begin(), res.end(), res.begin(), [](wchar_t c){ return std::tolower(c); });
    return res;
}

WString ToUpper(const WString& s)
{
    WString res(s);
    std::transform(res.begin(), res.end(), res.begin(), [](wchar_t c){ return std::toupper(c); });
    return res;
}


//---------------------------------------------------------------------------
// Check if a string starts or end with a string.
//---------------------------------------------------------------------------

bool StartsWith(const WString& s, const WString& prefix)
{
    return !prefix.empty() && s.length() >= prefix.length() && s.substr(0, prefix.length()) == prefix;
}

bool EndsWith(const WString& s, const WString& suffix)
{
    return !suffix.empty() && s.length() >= suffix.length() && s.substr(s.length() - suffix.length()) == suffix;
}


//---------------------------------------------------------------------------
// UTF-8 / UTF-16 conversions.
//---------------------------------------------------------------------------

WString ToUTF16(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

std::string ToUTF8(const WString& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
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

void PrintHexa(std::ostream& out, const void* addr, size_t size, const WString& margin, bool show_addr)
{
    const uint8_t* cur = reinterpret_cast<const uint8_t*>(addr);
    const uint8_t* end = cur + size;
    constexpr size_t bytes_per_line = 16;

    while (cur < end) {
        const size_t count = std::min<size_t>(bytes_per_line, end - cur);
        out << margin;
        if (show_addr) {
            out << Format(L"0x%08llX: ", size_t(cur));
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
