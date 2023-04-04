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


//----------------------------------------------------------------------------
// File name (without directory), file base name (without directory and prefix).
//----------------------------------------------------------------------------

std::string FileName(const std::string& name)
{
    const size_t pos = name.find_last_of(":/\\");
    return pos == std::string::npos ? name : name.substr(pos + 1);
}

std::string FileBaseName(const std::string& name)
{
    const std::string filename(FileName(name));
    const size_t pos = filename.find_last_of(".");
    return pos == std::string::npos ? filename : filename.substr(0, pos);
}


//----------------------------------------------------------------------------
// Transform an error code into an error message string.
//----------------------------------------------------------------------------

std::string Error(::DWORD code)
{
    std::string message(1024, ' ');
    size_t length = ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, 0, &message[0], ::DWORD(message.size()), nullptr);
    message.resize(std::min(length, message.size()));

    while (!message.empty() && std::isspace(message.back())) {
        message.pop_back();
    }

    if (!message.empty()) {
        return message;
    }
    else {
        // Message is not found.
        return Format("System error %d (0x%X)", code, code);
    }
}


//---------------------------------------------------------------------------
// Get the value of an environment variable.
//---------------------------------------------------------------------------

std::string GetEnv(const std::string& name, const std::string& def)
{
    std::string value(1024, ' ');
    ::DWORD size = ::GetEnvironmentVariableA(name.c_str(), &value[0], ::DWORD(value.size()));
    if (size >= ::DWORD(value.size())) {
        value.resize(size_t(size + 1));
        size = ::GetEnvironmentVariableA(name.c_str(), &value[0], ::DWORD(value.size()));
    }
    value.resize(std::max<size_t>(0, std::min<size_t>(value.size(), size)));
    return value.empty() ? def : value;
}


//---------------------------------------------------------------------------
// Print a grid of strings. All columns are aligned on their maximum width.
//---------------------------------------------------------------------------

void PrintGrid(std::ostream& out, const Grid& grid, const std::string& margin, size_t spacing)
{
    std::vector<size_t> widths;

    // Compute columns widths.
    for (const auto& row : grid) {
        for (size_t i = 0; i < row.size(); ++i) {
            const size_t w = row[i].length();
            if (widths.size() <= i) {
                widths.push_back(w);
            }
            else if (widths[i] < w) {
                widths[i] = w;
            }
        }
    }

    // Then print the grid.
    for (const auto& row : grid) {
        out << margin;
        for (size_t i = 0; i < row.size(); ++i) {
            out << row[i];
            if (i < row.size() - 1) {
                out << std::string(widths[i] - row[i].length() + spacing, ' ');
            }
        }
        out << std::endl;
    }
}
