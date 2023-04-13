//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Get version information from binary files.
//
//----------------------------------------------------------------------------

#include "fileversion.h"
#include "winutils.h"
#include "kbdrc.h"


//-----------------------------------------------------------------------------
// Constructor. Specify where to report errors.
//-----------------------------------------------------------------------------

FileVersionInfo::FileVersionInfo(std::ostream* err) :
    Error(err),
    FileVersion1(0),
    FileVersion2(0),
    FileVersion3(0),
    FileVersion4(0),
    ProductVersion1(0),
    ProductVersion2(0),
    ProductVersion3(0),
    ProductVersion4(0),
    FileType(0),
    FileSubtype(0),
    CompanyName(),
    FileDescription(),
    FileVersion(),
    InternalName(),
    LegalCopyright(),
    OriginalFilename(),
    ProductName(),
    ProductVersion(),
    LayoutText(),
    BaseLanguage()
{
}


//-----------------------------------------------------------------------------
// Clear content.
//-----------------------------------------------------------------------------

void FileVersionInfo::clear()
{
    FileVersion1 = 0;
    FileVersion2 = 0;
    FileVersion3 = 0;
    FileVersion4 = 0;
    ProductVersion1 = 0;
    ProductVersion2 = 0;
    ProductVersion3 = 0;
    ProductVersion4 = 0;
    FileType = 0;
    FileSubtype = 0;
    CompanyName.clear();
    FileDescription.clear();
    FileVersion.clear();
    InternalName.clear();
    LegalCopyright.clear();
    OriginalFilename.clear();
    ProductName.clear();
    ProductVersion.clear();
    LayoutText.clear();
    BaseLanguage.clear();
}


//-----------------------------------------------------------------------------
// Load the FileVersionInfo from one file.
//-----------------------------------------------------------------------------

bool FileVersionInfo::load(const std::wstring& filename)
{
    clear();
    bool success = loadByName(filename);

    HMODULE hmod = LoadLibraryExW(filename.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE);
    if (hmod == nullptr) {
        const DWORD err = GetLastError();
        error("error opening " + filename + ": " + ErrorText(err));
        success = false;
    }
    else {
        success = loadByHandle(hmod) && success;
        FreeLibrary(hmod);
    }

    return success;
}

bool FileVersionInfo::load(HMODULE hmod)
{
    clear();
    bool success = loadByHandle(hmod);

    const std::wstring filename(ModuleFileName(GetCurrentProcess(), hmod));
    if (filename.empty()) {
        error(Format(L"Cannot get file name for handle 0x%08llX", uint64_t(hmod)));
        success = false;
    }
    else {
        success = loadByName(filename) && success;
    }

    return success;
}


//-----------------------------------------------------------------------------
// Individual loading functions.
//-----------------------------------------------------------------------------

bool FileVersionInfo::loadByHandle(HMODULE hmod)
{
    LayoutText = GetResourceString(hmod, WKL_RES_TEXT);
    BaseLanguage = GetResourceString(hmod, WKL_RES_LANG);
    return true;
}

bool FileVersionInfo::loadByName(const std::wstring& filename)
{
    // Get size of version info.
    DWORD handle = 0;
    const DWORD size = GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, filename.c_str(), &handle);
    if (size == 0) {
        const DWORD err = GetLastError();
        error("error reading version info from " + filename + ": " + ErrorText(err));
        return false;
    }

    // Get version info content.
    std::vector<uint8_t> buffer(size, 0);
    void* const data = &buffer[0];
    if (!GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, filename.c_str(), handle, size, data)) {
        const DWORD err = GetLastError();
        error("error reading version info from " + filename + ": " + ErrorText(err));
        return false;
    }

    // Get fixed version info (the integer values).
    void* value = nullptr;
    UINT length = 0;
    if (VerQueryValueW(data, L"\\", &value, &length) && value != nullptr && length >= sizeof(VS_FIXEDFILEINFO)) {
        VS_FIXEDFILEINFO* fixed = reinterpret_cast<VS_FIXEDFILEINFO*>(value);
        FileVersion1 = HIWORD(fixed->dwFileVersionMS);
        FileVersion2 = LOWORD(fixed->dwFileVersionMS);
        FileVersion3 = HIWORD(fixed->dwFileVersionLS);
        FileVersion4 = LOWORD(fixed->dwFileVersionLS);
        ProductVersion1 = HIWORD(fixed->dwProductVersionMS);
        ProductVersion2 = LOWORD(fixed->dwProductVersionMS);
        ProductVersion3 = HIWORD(fixed->dwProductVersionLS);
        ProductVersion4 = LOWORD(fixed->dwProductVersionLS);
        FileType = fixed->dwFileType;
        FileSubtype = fixed->dwFileSubtype;
    }

    // Get string values.
    CompanyName      = getVersionString(data, L"CompanyName");
    FileDescription  = getVersionString(data, L"FileDescription");
    FileVersion      = getVersionString(data, L"FileVersion");
    InternalName     = getVersionString(data, L"InternalName");
    LegalCopyright   = getVersionString(data, L"LegalCopyright");
    OriginalFilename = getVersionString(data, L"OriginalFilename");
    ProductName      = getVersionString(data, L"ProductName");
    ProductVersion   = getVersionString(data, L"ProductVersion");

    return true;
}

std::wstring FileVersionInfo::getVersionString(void* data, const std::wstring& name)
{
    void* value = nullptr;
    UINT length = 0;

    // Try various languages.
    static const wchar_t* const languages[] = {L"000004B0", L"000004E4", L"040904E4", nullptr};

    for (const wchar_t* const* lang = languages; *lang != nullptr; ++lang) {
        const std::wstring path(L"\\StringFileInfo\\" + std::wstring(*lang) + L"\\" + name);
        if (VerQueryValueW(data, path.c_str(), &value, &length) && value != nullptr && length > 0) {
            // The returned length is in (w)characters, not bytes.
            const wchar_t* str = reinterpret_cast<const wchar_t*>(value);
            while (length > 0 && str[length - 1] == '\0') {
                length--;

            }
            return std::wstring(str, length);
        }
    }
    return std::wstring();
}
