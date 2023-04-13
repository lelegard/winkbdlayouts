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
#include "error.h"

class FileVersionInfo : public Error
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
    WString CompanyName;
    WString FileDescription;
    WString FileVersion;
    WString InternalName;
    WString LegalCopyright;
    WString OriginalFilename;
    WString ProductName;
    WString ProductVersion;

    // Additional string resources for keyboard layouts.
    WString LayoutText;
    WString BaseLanguage;  // lower 4 hexa digits of id.

    // Load the information from one file.
    bool load(const WString& filename);
    bool load(HMODULE = nullptr); // default to current exe

    // Clear content.
    void clear();

private:
    // Individual loading functions.
    bool loadByName(const WString& filename);
    bool loadByHandle(HMODULE);
    WString getVersionString(void* data, const WString& name);
};
