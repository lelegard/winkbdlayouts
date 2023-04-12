//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Get version information from binary files.
//
//----------------------------------------------------------------------------

#pragma once
#include "strutils.h"

class FileVersionInfo
{
public:
    // Constructor. Specify where to report errors.
    FileVersionInfo(std::ostream* err = nullptr);

    // Standard version information integers (from VS_FIXEDFILEINFO).
    uint16_t FileVersion1;
    uint16_t FileVersion2;
    uint16_t FileVersion3;
    uint16_t FileVersion4;
    uint16_t ProductVersion1;
    uint16_t ProductVersion2;
    uint16_t ProductVersion3;
    uint16_t ProductVersion4;
    uint32_t FileType;       // e.g. VFT_DRIVER
    uint32_t FileSubtype;    // e.g. VFT2_DRV_KEYBOARD

    // Standard version information strings.
    std::wstring CompanyName;
    std::wstring FileDescription;
    std::wstring FileVersion;
    std::wstring InternalName;
    std::wstring LegalCopyright;
    std::wstring OriginalFilename;
    std::wstring ProductName;
    std::wstring ProductVersion;

    // Additional string resources for keyboard layouts.
    std::wstring LayoutText;
    std::wstring BaseLanguage;  // lower 4 hexa digits of id.

    // Load the information from one file.
    bool load(const std::string& filename);
    bool load(HMODULE = nullptr); // default to current exe

    // Clear content.
    void clear();

private:
    std::ostream* _err;

    // Report an error.
    void error(const std::string& message);

    // Individual loading functions.
    bool loadByName(const std::string& filename);
    bool loadByHandle(HMODULE);
    std::wstring getVersionString(void* data, const std::wstring& name);
};
