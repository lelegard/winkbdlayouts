//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Some registry utilities.
//
//----------------------------------------------------------------------------

#pragma once
#include "error.h"

class Registry : public Error
{
public:
    // Constructor. Specify where to report errors.
    Registry(std::ostream* err = nullptr) : Error(err) {}

    // Check if a registry key or value exists.
    bool keyExists(const std::wstring& key) { return valueExists(key, L""); }
    bool valueExists(const std::wstring& key, const std::wstring& value_name);

    // Get a value in a registry key as a string. Can automatically expand environment variables.
    std::wstring getValue(const std::wstring& key, const std::wstring& value_name, bool expand = false);

    // Get all value names and subkeys in a key.
    bool getSubKeys(const std::wstring& key, WStringList& subkeys);
    bool getValueNames(const std::wstring& key, WStringList& names);

    // Set the value of a registry key.
    bool setValue(const std::wstring& key, const std::wstring& value_name, const std::wstring& value, bool expandable = false);
    bool setValue(const std::wstring& key, const std::wstring& value_name, DWORD value);

    // Delete a value of a registry key.
    bool deleteValue(const std::wstring& key, const std::wstring& value_name);

    // Create a registry key.
    bool createKey(const std::wstring& key, bool is_volatile = false);

    // Delete a registry key.
    bool deleteKey(const std::wstring& key);

    // Get the root key of a registry path.
    bool splitKey(const std::wstring& key, HKEY& root_key, std::wstring& subkey);
    bool splitKey(const std::wstring& key, HKEY& root_key, std::wstring& midkey, std::wstring& final_key);

private:
    // Open a registry key.
    bool openKey(HKEY root, const std::wstring& key, REGSAM sam, HKEY& handle);
};
