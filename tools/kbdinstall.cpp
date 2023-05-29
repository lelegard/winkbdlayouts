//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Keyboard layout DLL installation and uninstallation.
//
//----------------------------------------------------------------------------

#include "kbdinstall.h"
#include "registry.h"
#include "winutils.h"
#include "kbdrc.h"


//---------------------------------------------------------------------------
// Install a keyboard layout DLL.
//---------------------------------------------------------------------------

uint32_t InstallKeyboardLayout(Error& err, const WString& dll, uint16_t base_language, const WString& description)
{
    // We need valid base language and description.
    if (base_language == 0) {
        err.error(L"invalid base language for " + dll);
        return 0;
    }
    if (description.empty()) {
        err.error(L"empty description for " + dll);
        return 0;
    }

    // Reference DLL name to store in the registry.
    const WString filename(ToLower(FileName(dll)));
    const WString filepath(GetSystem32() + L"\\" + filename);

    // List of existing language ids with matching base language (last 4 digits).
    std::set<uint32_t> matching_lang_ids;
    uint32_t final_lang_id = 0;

    // List of existing layout ids (registry value "Layout Id"), 16-bit integers.
    std::set<uint16_t> all_layout_ids;
    uint16_t max_layout_id = 0;
    uint16_t final_layout_id = 0;

    // Enumerate keyboard layouts in registry.
    Registry reg(err);
    WStringList all_lang_ids;
    if (!reg.getSubKeys(REGISTRY_LAYOUT_KEY, all_lang_ids)) {
        return 0;
    }

    for (const auto& lang_id : all_lang_ids) {

        // Each entry is expected to be an hexa string.
        uint32_t id = 0;
        FromHexa(id, lang_id);
        const WString key(REGISTRY_LAYOUT_KEY L"\\" + lang_id);

        // Keep track of all layout ids. Ignore missing ids (use 0).
        const WString layout_id_str(reg.getValue(key, REGISTRY_LAYOUT_ID, L"", false));
        uint16_t layout_id = 0;
        FromHexa(layout_id, layout_id_str);
        all_layout_ids.insert(layout_id);
        max_layout_id = std::max(max_layout_id, layout_id);

        // Check if the lang id matches the base language of the new keyboard.
        if ((id & 0xFFFF) == base_language) {
            matching_lang_ids.insert(id);
            if (final_lang_id == 0 && filename == ToLower(reg.getValue(key, REGISTRY_LAYOUT_FILE, L"", true))) {
                err.verbose(filename + " already registered, replacing it");
                final_lang_id = id;
                final_layout_id = layout_id;
            }
        }
    }

    // Allocate a layout id if not already registered.
    if (final_layout_id == 0) {
        if (max_layout_id < 0xFFFF) {
            final_layout_id = max_layout_id + 1;
        }
        else if (all_layout_ids.size() == 0x10000) {
            err.error(L"too many keyboard layouts already registered, ignoring " + dll);
            return 0;
        }
        else {
            while (all_layout_ids.find(final_layout_id) != all_layout_ids.end()) {
                final_layout_id++;
            }
        }
    }

    // Allocate a lang id if not already registered.
    if (final_lang_id == 0) {
        // Find an unused id with base language.
        // Example: if lang = "040c", try "a001040c", "a003040c", "a003040c", 
        for (uint32_t up = 0xA0000000; up < 0xB0000000; up += 0x00010000) {
            final_lang_id = up | base_language;
            if (matching_lang_ids.find(final_lang_id) == matching_lang_ids.end()) {
                break; // unused language id
            }
        }
    }

    // Copy DLL file first.
    if (CopyFileW(dll.c_str(), filepath.c_str(), false)) {
        err.verbose(L"copied " + filepath);
    }
    else {
        const DWORD errcode = GetLastError();
        if (errcode != ERROR_SHARING_VIOLATION) {
            err.error(L"error copying " + dll + ": " + ErrorText(errcode));
            return 0;
        }
        // Failure is "cannot access the file because it is being used by another process".
        // This means that the keyboard DLL is currently in use.
        // Copy it into a temporary directory and move it on reboot.
        err.info(dll + L" currently in use, will be installed on reboot");
        const WString temppath(GetSystemTemp() + L"\\" + filename);
        if (!CopyFileW(dll.c_str(), temppath.c_str(), false)) {
            err.error(L"error copying " + dll + " in temp directory: " + ErrorText(errcode));
            return 0;
        }
        if (!MoveFileExW(temppath.c_str(), filepath.c_str(), MOVEFILE_DELAY_UNTIL_REBOOT | MOVEFILE_REPLACE_EXISTING)) {
            err.error(L"error registering " + temppath + " for copy on reboot: " + ErrorText(errcode));
            return 0;
        }
    }

    // Then register it in the registry.
    const WString key(REGISTRY_LAYOUT_KEY L"\\" + Format(L"%08x", final_lang_id));
    if (!reg.keyExists(key) && !reg.createKey(key)) {
        return 0;
    }
    reg.setValue(key, REGISTRY_LAYOUT_FILE, filename);
    reg.setValue(key, REGISTRY_LAYOUT_TEXT, description);
    reg.setValue(key, REGISTRY_LAYOUT_ID, Format(L"%04x", int(final_layout_id)));
    return final_lang_id;
}


//---------------------------------------------------------------------------
// Uninstall a keyboard layout DLL.
//---------------------------------------------------------------------------

bool UninstallKeyboardLayout(Error& err, const WString& dll)
{
    // Reference DLL name to search in the registry.
    const WString filename(ToLower(FileName(dll)));
    const WString filepath(GetSystem32() + L"\\" + filename);

    // Enumerate keyboard layouts in registry.
    Registry reg(err);
    WStringList all_lang_ids;
    bool success = !filename.empty() && reg.getSubKeys(REGISTRY_LAYOUT_KEY, all_lang_ids);

    // Remove all references to this keyboard in the registry
    if (success) {
        for (const auto& lang_id : all_lang_ids) {
            const WString key(REGISTRY_LAYOUT_KEY "\\" + lang_id);
            if (filename == ToLower(reg.getValue(key, REGISTRY_LAYOUT_FILE, L"", true))) {
                err.verbose(L"unregistering keyboard layout " + lang_id);
                success = reg.deleteKey(key) && success;
            }
        }
    }

    // Delete the DLL file.
    if (success) {
        success = DeleteFileW(filepath.c_str());
        if (success) {
            err.verbose(L"deleted " + filepath);
        }
        else {
            const DWORD errcode = GetLastError();
            err.error(L"error deleting " + filepath + ", " + ErrorText(errcode));
        }
    }

    return success;
}


//---------------------------------------------------------------------------
// Install a keyboard layout DLL from the WKL project.
// These keyboard DLL are supposed to contain special resource strings.
//---------------------------------------------------------------------------

uint32_t WKLInstallKeyboardLayout(Error& err, const WString& dll)
{
    // Get all expected resource strings from a WKL DLL.
    HMODULE hmod = LoadLibraryExW(dll.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE);
    if (hmod == nullptr) {
        const DWORD errcode = GetLastError();
        err.error(dll + ": " + ErrorText(errcode));
        return 0;
    }
    const WString dll_name(ToLower(FileName(dll)));
    const WString dll_text(GetResourceString(hmod, WKL_RES_TEXT));
    const WString dll_provider(GetResourceString(hmod, WKL_RES_PROVIDER));
    WString dll_lang_id(ToLower(GetResourceString(hmod, WKL_RES_LANG)));
    int base_language = 0;
    FromHexa(base_language, dll_lang_id);
    FreeLibrary(hmod);

    // A WKL DLL is expected to have resource strings: "WKL" as provider and non-null language id.
    if (dll_provider != L"WKL" || base_language == 0) {
        err.error(dll + " is not a valid WKL keyboard layout");
        return 0;
    }

    // Install the keyboard layout DLL.
    err.verbose("installing " + dll_name + " (" + dll_text + ")");
    const uint32_t lang_id = InstallKeyboardLayout(err, dll, base_language, dll_text);

    // Add specific WKL entries in the registry.
    if (lang_id != 0) {
        Registry reg(err);
        const WString key(REGISTRY_LAYOUT_KEY L"\\" + Format(L"%08x", lang_id));
        reg.setValue(key, REGISTRY_LAYOUT_PROVIDER, dll_provider);
        reg.setValue(key, REGISTRY_LAYOUT_DISPLAY, L"@%SystemRoot%\\system32\\" + dll_name + Format(L",-%d", WKL_RES_TEXT), true);
    }

    return lang_id;
}


//---------------------------------------------------------------------------
// Install one or more keyboard layout DLL's from the WKL project.
//---------------------------------------------------------------------------

bool WKLInstallAllKeyboardLayouts(Error& err, const WStringVector& paths)
{
    bool success = true;

    for (const auto& path : paths) {
        if (IsDirectory(path)) {
            WStringList files;
            SearchFiles(files, path, L"kbd*.dll");
            for (const auto& file : files) {
                success = WKLInstallKeyboardLayout(err, path + L"\\" + file) != 0 && success;
            }
        }
        else {
            success = WKLInstallKeyboardLayout(err, path) != 0 && success;
        }
    }

    return success;
}


//---------------------------------------------------------------------------
// Uninstall all WKL keyboard layout DLL's.
//---------------------------------------------------------------------------

bool WKLUninstallAllKeyboardLayouts(Error& err)
{
    bool success = true;

    // Enumerate keyboard layouts in registry and remove all WKL keyboads.
    Registry reg(err);
    WStringList all_lang_ids;
    if (reg.getSubKeys(REGISTRY_LAYOUT_KEY, all_lang_ids)) {
        for (const auto& lang_id : all_lang_ids) {
            const WString file(reg.getValue(REGISTRY_LAYOUT_KEY L"\\" + lang_id, REGISTRY_LAYOUT_FILE, L"", true));
            if (!file.empty() && GetResourceString(GetSystem32() + L"\\" + file, WKL_RES_PROVIDER) == L"WKL") {
                err.verbose("uninstalling " + file);
                success = UninstallKeyboardLayout(err, file) && success;
            }
        }
    }

    return success;
}
