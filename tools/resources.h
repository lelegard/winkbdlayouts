//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Some utilities to get Windows resources from binary files.
//
//----------------------------------------------------------------------------

#pragma once
#include "strutils.h"

class Resources
{
public:
    // Constructor. Specify where to report errors.
    Resources(std::ostream* err = nullptr);

    // One resource.
    class Resource
    {
    public:
        std::string type;
        std::string name;
        std::string value;
    };

    // List of resources in one file.
    std::list<Resource> resources;

    // Load the resources from one file.
    bool load(const std::string& filename);
    bool load(::HMODULE = nullptr); // default to current process

private:
    std::ostream* _err;

    // Report an error.
    void error(const std::string& message);

    // Callbacks. The parameter is a Resource instance.
    static ::BOOL enumResourceType(::HMODULE hmod, ::LPSTR type, ::LONG_PTR param);
    static ::BOOL enumResourceName(::HMODULE hmod, ::LPCSTR type, ::LPSTR name, ::LONG_PTR param);
};
