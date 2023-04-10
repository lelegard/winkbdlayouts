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
// Report an error.
//-----------------------------------------------------------------------------

void Registry::error(const std::string& message)
{
    if (_err != nullptr) {
        *_err << message << std::endl;
    }
}


//-----------------------------------------------------------------------------
// Return the root key of a registry path.
//-----------------------------------------------------------------------------

bool Registry::splitKey(const std::string& key, ::HKEY& root_key, std::string& subkey)
{
    // Get end if root key name.
    const size_t sep = key.find('\\');

    // Root key name and subkey name.
    std::string root;
    if (sep == std::string::npos) {
        root = key;
        subkey.clear();
    }
    else {
        root = key.substr(0, sep);
        subkey = key.substr(sep + 1);
    }

    // Resolve root key handle.
    if (root == "HKEY_CLASSES_ROOT" || root == "HKCR") {
        root_key = HKEY_CLASSES_ROOT;
    }
    else if (root == "HKEY_CURRENT_USER" || root == "HKCU") {
        root_key = HKEY_CURRENT_USER;
    }
    else if (root == "HKEY_LOCAL_MACHINE" || root == "HKLM") {
        root_key = HKEY_LOCAL_MACHINE;
    }
    else if (root == "HKEY_USERS" || root == "HKU") {
        root_key = HKEY_USERS;
    }
    else if (root == "HKEY_CURRENT_CONFIG" || root == "HKCC") {
        root_key = HKEY_CURRENT_CONFIG;
    }
    else if (root == "HKEY_PERFORMANCE_DATA" || root == "HKPD") {
        root_key = HKEY_PERFORMANCE_DATA;
    }
    else {
        error("invalid root key \"" + root + "\"");
        root_key = NULL;
        return false;
    }
    return true;
}

bool Registry::splitKey(const std::string& key, ::HKEY& root_key, std::string& midkey, std::string& final_key)
{
    midkey.clear();
    const bool ok = splitKey(key, root_key, final_key);
    if (ok) {
        const size_t sep = final_key.rfind('\\');
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

bool Registry::valueExists(const std::string& key, const std::string& value_name)
{
    ::HKEY root;
    std::string subkey;

    // Split the key without error reporting.
    std::ostream* saved = _err;
    bool exists = splitKey(key, root, subkey);
    _err = saved;

    if (exists) {
        ::HKEY hkey;
        ::LONG hr = ::RegOpenKeyExA(root, subkey.c_str(), 0, KEY_QUERY_VALUE, &hkey);
        exists = hr == ERROR_SUCCESS;
        if (exists) {
            if (!value_name.empty()) {
                ::DWORD type = 0;
                ::DWORD size = 0;
                hr = ::RegQueryValueExA(hkey, value_name.c_str(), nullptr, &type, nullptr, &size);
                exists = hr == ERROR_SUCCESS || hr == ERROR_MORE_DATA;
            }
            ::RegCloseKey(hkey);
        }
    }
    return exists;
}


//-----------------------------------------------------------------------------
// Open a registry key.
//-----------------------------------------------------------------------------

bool Registry::openKey(::HKEY root, const std::string& key, ::REGSAM sam, ::HKEY& handle)
{
    ::LONG hr = ::RegOpenKeyExA(root, key.c_str(), 0, sam, &handle);
    if (hr == ERROR_SUCCESS) {
        return true;
    }
    else {
        error("error opening key " + key + ": " + Error(hr));
        return false;
    }
}


//-----------------------------------------------------------------------------
// Get a value in a registry key as a string.
//-----------------------------------------------------------------------------

std::string Registry::getValue(const std::string& key, const std::string& value_name)
{
    // Split name
    ::HKEY root, hkey;
    std::string subkey;
    if (!splitKey(key, root, subkey) || !openKey(root, subkey, KEY_READ, hkey)) {
        return std::string();
    }

    // Query the the size of the value in the key. By giving a NULL address
    // to lpData, RegQueryValueEx simply returns the size of the value.
    ::DWORD type = 0;
    ::DWORD size = 0;
    ::LONG hr = ::RegQueryValueExA(hkey, value_name.c_str(), nullptr, &type, nullptr, &size);
    if ((hr != ERROR_SUCCESS && hr != ERROR_MORE_DATA) || size <= 0) {
        error("error querying " + key + "\\" + value_name + ": " + Error(hr));
        ::RegCloseKey(hkey);
        return std::string();
    }

    // Allocate new buffer and actually get the value
    std::vector<::BYTE> buf(size + 10);
    size = ::DWORD(buf.size());
    hr = ::RegQueryValueExA(hkey, value_name.c_str(), nullptr, &type, &buf[0], &size);
    ::RegCloseKey(hkey);
    if (hr != ERROR_SUCCESS) {
        size = 0;
        type = REG_NONE;
    }
    size = std::max<::DWORD>(0, std::min(size, ::DWORD(buf.size() - 1)));
    buf[size] = 0; // if improperly terminated string

    // Convert value to a string
    std::string value;
    switch (type) {
        case REG_SZ:
        case REG_MULTI_SZ:
        case REG_EXPAND_SZ: {
            // There is at least one nul-terminated string in. If the type is REG_MULTI_SZ,
            // there are several nul-terminated strings, ending with two nuls, but we keep only the first string.
            value = reinterpret_cast<char*>(buf.data());
            break;
        }
        case REG_DWORD: {
            value = Format("%u", *reinterpret_cast<const ::DWORD*>(buf.data()));
            break;
        }
        case REG_DWORD_BIG_ENDIAN: {
            ::DWORD w = *reinterpret_cast<const ::DWORD*>(buf.data());
            value = Format("%u", ((w & 0x000000FF) << 24) | ((w & 0x0000FF00) << 8) | ((w & 0x00FF0000) >> 8) | ((w & 0xFF000000) >> 24));
            break;
        }
    }
    return value;
}


//-----------------------------------------------------------------------------
// Get all value names in a key.
//-----------------------------------------------------------------------------

bool Registry::getValueNames(const std::string& key, StringList& names)
{
    names.clear();

    // Split name
    ::HKEY root, hkey;
    std::string subkey;
    if (!splitKey(key, root, subkey) || !openKey(root, subkey, KEY_QUERY_VALUE, hkey)) {
        return false;
    }

    // Enumerate all value names.
    bool success = true;
    for (::DWORD index = 0; success; index++) {
        char name[2048];
        ::DWORD size = ::DWORD(sizeof(name));
        ::LSTATUS hr = ::RegEnumValueA(hkey, index, name, &size, nullptr, nullptr, nullptr, nullptr);
        if (hr == ERROR_NO_MORE_ITEMS) {
            break;
        }
        success = hr == ERROR_SUCCESS;
        if (!success) {
            error("error iterating " + key + ": " + Error(hr));
        }
        names.push_back(name);
    }

    ::RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Get all subkeys in a key.
//-----------------------------------------------------------------------------

bool Registry::getSubKeys(const std::string& key, StringList& subkeys)
{
    subkeys.clear();

    // Split name
    ::HKEY root, hkey;
    std::string subkey;
    if (!splitKey(key, root, subkey) || !openKey(root, subkey, KEY_ENUMERATE_SUB_KEYS, hkey)) {
        return false;
    }

    // Enumerate all subkeys.
    bool success = true;
    for (::DWORD index = 0; success; index++) {
        char name[1024];
        ::DWORD size = ::DWORD(sizeof(name));
        ::LSTATUS hr = ::RegEnumKeyExA(hkey, index, name, &size, nullptr, nullptr, nullptr, nullptr);
        if (hr == ERROR_NO_MORE_ITEMS) {
            break;
        }
        success = hr == ERROR_SUCCESS;
        if (!success) {
            error("error iterating " + key + ": " + Error(hr));
        }
        subkeys.push_back(name);
    }

    ::RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Set value of a registry key.
//-----------------------------------------------------------------------------

bool Registry::setValue(const std::string& key, const std::string& value_name, const std::string& value, bool expandable)
{
    // Split name
    ::HKEY root, hkey;
    std::string subkey;
    if (!splitKey(key, root, subkey) || !openKey(root, subkey, KEY_WRITE, hkey)) {
        return false;
    }

    // Set the value
    ::LSTATUS hr = ::RegSetValueExA(hkey, value_name.c_str(), 0, expandable ? REG_EXPAND_SZ : REG_SZ,
                                    reinterpret_cast<const ::BYTE*>(value.c_str()),
                                   ::DWORD(value.length() + 1)); // include terminating nul

    const bool success = hr == ERROR_SUCCESS;
    if (!success) {
        error("error setting " + key + "\\" + value_name + ": " + Error(hr));
    }
    ::RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Set value of a registry key.
//-----------------------------------------------------------------------------

bool Registry::setValue(const std::string& key, const std::string& value_name, ::DWORD value)
{
    // Split name
    ::HKEY root, hkey;
    std::string subkey;
    if (!splitKey(key, root, subkey) || !openKey(root, subkey, KEY_WRITE, hkey)) {
        return false;
    }

    // Set the value
    ::LONG hr = ::RegSetValueExA(hkey, value_name.c_str(), 0, REG_DWORD,
                                 reinterpret_cast<const ::BYTE*>(&value),
                                 sizeof(value));

    const bool success = hr == ERROR_SUCCESS;
    if (!success) {
        error("error setting " + key + "\\" + value_name + ": " + Error(hr));
    }
    ::RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Delete a value of a registry key.
//-----------------------------------------------------------------------------

bool Registry::deleteValue(const std::string& key, const std::string& value_name)
{
    // Split name
    ::HKEY root, hkey;
    std::string subkey;
    if (!splitKey(key, root, subkey) || !openKey(root, subkey, KEY_SET_VALUE, hkey)) {
        return false;
    }

    // Delete the value
    const ::LONG hr = ::RegDeleteValueA(hkey, value_name.c_str());
    const bool success = hr == ERROR_SUCCESS;
    if (!success) {
        error("error deleting " + key + "\\" + value_name + ": " + Error(hr));
    }
    ::RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Create a registry key.
//-----------------------------------------------------------------------------

bool Registry::createKey(const std::string& key, bool is_volatile)
{
    // Split name
    ::HKEY root, hkey;
    std::string midkey, newkey;
    if (!splitKey(key, root, midkey, newkey) || !openKey(root, midkey, KEY_CREATE_SUB_KEY | KEY_READ, hkey)) {
        return false;
    }

    // Create the key
    ::HKEY hnewkey;
    const ::LONG hr = ::RegCreateKeyExA(hkey, newkey.c_str(), 0, nullptr, 
                                        is_volatile ? REG_OPTION_VOLATILE : REG_OPTION_NON_VOLATILE,
                                        0, nullptr, &hnewkey, nullptr);

    const bool success = hr == ERROR_SUCCESS;
    if (success) {
        ::RegCloseKey(hnewkey);
    }
    else {
        error("error creating " + key + ": " + Error(hr));
    }
    ::RegCloseKey(hkey);
    return success;
}


//-----------------------------------------------------------------------------
// Delete a registry key.
//-----------------------------------------------------------------------------

bool Registry::deleteKey(const std::string& key)
{
    // Split name
    ::HKEY root, hkey;
    std::string midkey, newkey;
    if (!splitKey(key, root, midkey, newkey) || !openKey(root, midkey, KEY_WRITE, hkey)) {
        return false;
    }

    // Delete the key
    const ::LONG hr = ::RegDeleteKeyA(hkey, newkey.c_str());
    const bool success = hr == ERROR_SUCCESS;
    if (!success) {
        error("error deleting " + key + ": " + Error(hr));
    }
    ::RegCloseKey(hkey);
    return success;
}
