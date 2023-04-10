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
#include "strutils.h"

class Registry
{
public:
    // Constructor. Specify where to report errors.
    Registry(std::ostream* err = nullptr) : _err(err) {}

    // Check if a registry key or value exists.
    bool keyExists(const std::string& key) { return valueExists(key, ""); }
    bool valueExists(const std::string& key, const std::string& value_name);

    // Get a value in a registry key as a string.
    std::string getValue(const std::string& key, const std::string& value_name);

    // Get all value names and subkeys in a key.
    bool getSubKeys(const std::string& key, StringList& subkeys);
    bool getValueNames(const std::string& key, StringList& names);

    // Set the value of a registry key.
    bool setValue(const std::string& key, const std::string& value_name, const std::string& value, bool expandable = false);
    bool setValue(const std::string& key, const std::string& value_name, ::DWORD value);

    // Delete a value of a registry key.
    bool deleteValue(const std::string& key, const std::string& value_name);

    // Create a registry key.
    bool createKey(const std::string& key, bool is_volatile = false);

    // Delete a registry key.
    bool deleteKey(const std::string& key);

    // Get the root key of a registry path.
    bool splitKey(const std::string& key, ::HKEY& root_key, std::string& subkey);
    bool splitKey(const std::string& key, ::HKEY& root_key, std::string& midkey, std::string& final_key);

private:
    std::ostream* _err;

    // Report an error.
    void error(const std::string& message);

    // Open a registry key.
    bool openKey(::HKEY root, const std::string& key, ::REGSAM sam, ::HKEY& handle);
};
