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
// Format a WString literal, as used in a C/C++ source file.
//---------------------------------------------------------------------------

namespace {
    // WCHAR representation when inserted in strings literals.
    const std::map<wchar_t, const wchar_t*> wchar_literals {
        {L'\t', L"\\t"},
        {L'\n', L"\\n"},
        {L'\r', L"\\r"},
        {L'"',  L"\\\""},
        {L'\\', L"\\\\"},
    };
}

WString WStringLiteral(const wchar_t* value)
{
    if (value == nullptr) {
        return L"NULL";
    }
    else {
        WString str(L"L\"");
        for (; *value != 0; ++value) {
            const auto it = wchar_literals.find(*value);
            if (it != wchar_literals.end()) {
                str.append(it->second);
            }
            else if (*value >= L' ' && *value < 0x007F) {
                str.push_back(*value);
            }
            else {
                str.append(Format(L"\\x%04x", *value));
            }
        }
        str.push_back(L'"');
        return str;
    }
}


//---------------------------------------------------------------------------
// UTF-8 / UTF-16 conversions.
//---------------------------------------------------------------------------

WString ToUTF16(const std::string& str)
{
    if (str.empty()) {
        return WString();
    }
    else {
        // The UTF-16 string cannot have more characters than the UTF-8 one.
        WString out(str.size(), 0);
        const size_t size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), int(str.size()), &out[0], int(out.size()));
        out.resize(std::min(size, out.size()));
        return out;
    }
}

std::string ToUTF8(const WString& str)
{
    if (str.empty()) {
        return std::string();
    }
    else {
        // There is at most 4 bytes per Unicode character.
        std::string out(4 * str.size(), 0);
        const size_t size = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), int(str.size()), &out[0], int(out.size()), nullptr, nullptr);
        out.resize(std::min(size, out.size()));
        return out;
    }
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
