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
    const WString filepath(GetEnv(L"SystemRoot", L"C:\\Windows") + L"\\System32\\" + filename);

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
        err.error(L"error copying " + dll + ": " + ErrorText(errcode));
        return 0;
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
    const WString filepath(GetEnv(L"SystemRoot", L"C:\\Windows") + L"\\System32\\" + filename);

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
