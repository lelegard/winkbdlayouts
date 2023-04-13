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
    bool keyExists(const WString& key) { return valueExists(key, L""); }
    bool valueExists(const WString& key, const WString& value_name);

    // Get a value in a registry key as a string. Can automatically expand environment variables.
    WString getValue(const WString& key, const WString& value_name, bool expand = false);

    // Get all value names and subkeys in a key.
    bool getSubKeys(const WString& key, WStringList& subkeys);
    bool getValueNames(const WString& key, WStringList& names);

    // Set the value of a registry key.
    bool setValue(const WString& key, const WString& value_name, const WString& value, bool expandable = false);
    bool setValue(const WString& key, const WString& value_name, DWORD value);

    // Delete a value of a registry key.
    bool deleteValue(const WString& key, const WString& value_name);

    // Create a registry key.
    bool createKey(const WString& key, bool is_volatile = false);

    // Delete a registry key.
    bool deleteKey(const WString& key);

    // Get the root key of a registry path.
    bool splitKey(const WString& key, HKEY& root_key, WString& subkey);
    bool splitKey(const WString& key, HKEY& root_key, WString& midkey, WString& final_key);

private:
    // Open a registry key.
    bool openKey(HKEY root, const WString& key, REGSAM sam, HKEY& handle);
};
