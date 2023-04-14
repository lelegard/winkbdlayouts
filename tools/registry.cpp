//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Some registry utilities.
//
//----------------------------------------------------------------------------

#include "registry.h"
#include "winutils.h"


//-----------------------------------------------------------------------------
// Return the root key of a registry path.
//-----------------------------------------------------------------------------

bool Registry::splitKey(const WString& key, HKEY& root_key, WString& subkey)
{
    // Get end if root key name.
    const size_t sep = key.find(L'\\');

    // Root key name and subkey name.
    WString root;
    if (sep == std::string::npos) {
        root = key;
        subkey.clear();
    }
    else {
        root = key.substr(0, sep);
        subkey = key.substr(sep + 1);
    }

    // Resolve root key handle.
    if (root == L"HKEY_CLASSES_ROOT" || root == L"HKCR") {
        root_key = HKEY_CLASSES_ROOT;
    }
    else if (root == L"HKEY_CURRENT_USER" || root == L"HKCU") {
        root_key = HKEY_CURRENT_USER;
    }
    else if (root == L"HKEY_LOCAL_MACHINE" || root == L"HKLM") {
        root_key = HKEY_LOCAL_MACHINE;
    }
    else if (root == L"HKEY_USERS" || root == L"HKU") {
        root_key = HKEY_USERS;
    }
    else if (root == L"HKEY_CURRENT_CONFIG" || root == L"HKCC") {
        root_key = HKEY_CURRENT_CONFIG;
    }
    else if (root == L"HKEY_PERFORMANCE_DATA" || root == L"HKPD") {
        root_key = HKEY_PERFORMANCE_DATA;
    }
    else {
        _err.error("invalid root key \"" + root + "\"");
        root_key = NULL;
        return false;
    }
    return true;
}

bool Registry::splitKey(const WString& key, HKEY& root_key, WString& midkey, WString& final_key)
{
    midkey.clear();
    const bool ok = splitKey(key, root_key, final_key);
    if (ok) {
        const size_t sep = final_key.rfind(L'\\');
        if (sep != std::string::npos) {
            midkey = final_key.substr(0, sep);
            final_key.erase(0, sep + 1);
        }
    }
    return ok;
}


//-----------------------------------------------------------------------------
// Check if a registry value exists.
//-----------------------------------------------------------------------------

bool Registry::valueExists(const WString& key, const WString& value_name)
{
    HKEY root;
    WString subkey;

    // Split the key without error reporting.
    _err.muteErrors();
    bool exists = splitKey(key, root, subkey);
    _err.restoreErrors();

    if (exists) {
        HKEY hkey;
        LONG hr = RegOpenKeyExW(root, subkey.c_str(), 0, KEY_QUERY_VALUE, &hkey);
        exists = hr == ERROR_SUCCESS;
        if (exists) {
            if (!value_name.empty()) {
                DWORD type = 0;
                DWORD size = 0;
                hr = RegQueryValueExW(hkey, value_name.c_str(), nullptr, &type, nullptr, &size);
                exists = hr == ERROR_SUCCESS || hr == ERROR_MORE_DATA;
            }
            RegCloseKey(hkey);
        }
    }
    return exists;
}


//-----------------------------------------------------------------------------
// Open a registry key.
//-----------------------------------------------------------------------------

bool Registry::openKey(HKEY root, const WString& key, REGSAM sam, HKEY& handle)
{
    LONG hr = RegOpenKeyExW(root, key.c_str(), 0, sam, &handle);
    if (hr == ERROR_SUCCESS) {
        return true;
    }
    else {
        _err.error("error opening key " + key + ": " + ErrorText(hr));
        return false;
    }
}


//-----------------------------------------------------------------------------
// Get a value in a registry key as a string.
//-----------------------------------------------------------------------------

WString Registry::getValue(const WString& key, const WString& value_name, bool expand)
{
    // Split name
    HKEY root;
    WString subkey;
    if (!splitKey(key, root, subkey)) {
        return WString();
    }

    // RegGetValue flags.
    const DWORD flags = RRF_RT_ANY | (expand ? 0 : RRF_NOEXPAND);

    // Query the the size in bytes of the value in the key.
    DWORD type = 0;
    DWORD size = 0;
    LONG hr = RegGetValueW(root, subkey.c_str(), value_name.c_str(), flags, &type, nullptr, &size);
    if ((hr != ERROR_SUCCESS && hr != ERROR_MORE_DATA) || size <= 0) {
        _err.error("error querying " + key + L"\\" + value_name + ": " + ErrorText(hr));
        return WString();
    }

    // Allocate new buffer and actually get the value
    std::vector<uint8_t> buf(size + 10);
    size = DWORD(buf.size());
    hr = RegGetValueW(root, subkey.c_str(), value_name.c_str(), flags, &type, &buf[0], &size);
    if (hr != ERROR_SUCCESS) {
        size = 0;
        type = REG_NONE;
    }
    size = std::max<DWORD>(0, std::min(size, DWORD(buf.size() - 2)));
    buf[size] = buf[size+1] = 0; // if improperly terminated string

    // Convert value to a string
    WString value;
    switch (type) {
        case REG_SZ:
        case REG_MULTI_SZ:
        case REG_EXPAND_SZ: {
            // There is at least one nul-terminated string in. If the type is REG_MULTI_SZ,
            // there are several nul-terminated strings, ending with two nuls, but we keep only the first string.
            value = reinterpret_cast<wchar_t*>(buf.data());
            break;
        }
        case REG_DWORD: {
            value = Format(L"%u", *reinterpret_cast<const DWORD*>(buf.data()));
            break;
        }
        case REG_DWORD_BIG_ENDIAN: {
            DWORD w = *reinterpret_cast<const DWORD*>(buf.data());
            w = ((w & 0x000000FF) << 24) | ((w & 0x0000FF00) << 8) | ((w & 0x00FF0000) >> 8) | ((w & 0xFF000000) >> 24);
            value = Format(L"%u", w);
            break;
        }
    }

    // Expand resource strings.
    if (expand && StartsWith(value, L"@")) {
        const size_t sep = value.find(L",-");
        if (sep != WString::npos) {
            int index = -1;
            char dummy = 0;
            const std::string istr(ToUTF8(value.c_str() + sep + 2));
            if (sscanf(istr.c_str(), "%d%c", &index, &dummy) == 1) {
                const WString res(GetResourceString(value.substr(1, sep - 1), index));
                if (!res.empty()) {
                    value = res;
                }
            }
        }
    }

    return value;
}


//-----------------------------------------------------------------------------
// Get all value names in a key.
//-----------------------------------------------------------------------------

bool Registry::getValueNames(const WString& key, WStringList& names)
{
    names.clear();

    // Split name
    HKEY root, hkey;
    WString subkey;
    if (!splitKey(key, root, subkey) || !openKey(root, subkey, KEY_QUERY_VALUE, hkey)) {
        return false;
    }

    // Enumerate all value names.
    bool success = true;
    for (DWORD index = 0; success; index++) {
        WString name(2048, L'\0');
        DWORD size = DWORD(name.size());
        LSTATUS hr = RegEnumValueW(hkey, index, &name[0], &size, nullptr, nullptr, nullptr, nullptr);
        if (hr == ERROR_NO_MORE_ITEMS) {
            break;
        }
        success = hr == ERROR_SUCCESS;
        if (!success) {
            _err.error("error iterating " + key + ": " + ErrorText(hr));
        }
        name.resize(std::min<size_t>(size, name.size()));
        names.push_back(name);
    }

    RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Get all subkeys in a key.
//-----------------------------------------------------------------------------

bool Registry::getSubKeys(const WString& key, WStringList& subkeys)
{
    subkeys.clear();

    // Split name
    HKEY root, hkey;
    WString subkey;
    if (!splitKey(key, root, subkey) || !openKey(root, subkey, KEY_ENUMERATE_SUB_KEYS, hkey)) {
        return false;
    }

    // Enumerate all subkeys.
    bool success = true;
    for (DWORD index = 0; success; index++) {
        WString name(2048, L'\0');
        DWORD size = DWORD(name.size());
        LSTATUS hr = RegEnumKeyExW(hkey, index, &name[0], &size, nullptr, nullptr, nullptr, nullptr);
        if (hr == ERROR_NO_MORE_ITEMS) {
            break;
        }
        success = hr == ERROR_SUCCESS;
        if (!success) {
            _err.error("error iterating " + key + ": " + ErrorText(hr));
        }
        name.resize(std::min<size_t>(size, name.size()));
        subkeys.push_back(name);
    }

    RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Set value of a registry key.
//-----------------------------------------------------------------------------

bool Registry::setValue(const WString& key, const WString& value_name, const WString& value, bool expandable)
{
    // Split name
    HKEY root;
    WString subkey;
    if (!splitKey(key, root, subkey)) {
        return false;
    }

    // Set the value
    LSTATUS hr = RegSetKeyValueW(root, subkey.c_str(), value_name.c_str(), expandable ? REG_EXPAND_SZ : REG_SZ,
                                 reinterpret_cast<const BYTE*>(value.c_str()),
                                 DWORD((value.length() + 1) * sizeof(wchar_t))); // include terminating nul

    const bool success = hr == ERROR_SUCCESS;
    if (!success) {
        _err.error("error setting " + key + L"\\" + value_name + ": " + ErrorText(hr));
    }
    return success;
}


//-----------------------------------------------------------------------------
// Set value of a registry key.
//-----------------------------------------------------------------------------

bool Registry::setValue(const WString& key, const WString& value_name, DWORD value)
{
    HKEY root;
    WString subkey;
    if (!splitKey(key, root, subkey)) {
        return false;
    }

    // Set the value
    LSTATUS hr = RegSetKeyValueW(root, subkey.c_str(), value_name.c_str(), REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value));
    const bool success = hr == ERROR_SUCCESS;
    if (!success) {
        _err.error("error setting " + key + L"\\" + value_name + ": " + ErrorText(hr));
    }
    return success;
}


//-----------------------------------------------------------------------------
// Delete a value of a registry key.
//-----------------------------------------------------------------------------

bool Registry::deleteValue(const WString& key, const WString& value_name)
{
    // Split name
    HKEY root, hkey;
    WString subkey;
    if (!splitKey(key, root, subkey) || !openKey(root, subkey, KEY_SET_VALUE, hkey)) {
        return false;
    }

    // Delete the value
    const LONG hr = RegDeleteValueW(hkey, value_name.c_str());
    const bool success = hr == ERROR_SUCCESS;
    if (!success) {
        _err.error("error deleting " + key + L"\\" + value_name + ": " + ErrorText(hr));
    }
    RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Create a registry key.
//-----------------------------------------------------------------------------

bool Registry::createKey(const WString& key, bool is_volatile)
{
    // Split name
    HKEY root, hkey;
    WString midkey, newkey;
    if (!splitKey(key, root, midkey, newkey) || !openKey(root, midkey, KEY_CREATE_SUB_KEY | KEY_READ, hkey)) {
        return false;
    }

    // Create the key
    HKEY hnewkey = nullptr;
    const DWORD options = is_volatile ? REG_OPTION_VOLATILE : REG_OPTION_NON_VOLATILE;
    const LONG hr = RegCreateKeyExW(hkey, newkey.c_str(), 0, nullptr, options, 0, nullptr, &hnewkey, nullptr);

    const bool success = hr == ERROR_SUCCESS;
    if (success) {
        RegCloseKey(hnewkey);
    }
    else {
        _err.error("error creating " + key + ": " + ErrorText(hr));
    }
    RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Delete a registry key.
//-----------------------------------------------------------------------------

bool Registry::deleteKey(const WString& key)
{
    // Split name
    HKEY root, hkey;
    WString midkey, endkey;
    if (!splitKey(key, root, midkey, endkey) || !openKey(root, midkey, KEY_WRITE, hkey)) {
        return false;
    }

    // Delete the key
    const LONG hr = RegDeleteKeyW(hkey, endkey.c_str());
    const bool success = hr == ERROR_SUCCESS;
    if (!success) {
        _err.error("error deleting " + key + ": " + ErrorText(hr));
    }
    RegCloseKey(hkey);
    return success;
}
