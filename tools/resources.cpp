//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Some utilities to get Windows resources from binary files.
//
//----------------------------------------------------------------------------

#include "resources.h"
#include "winutils.h"


//-----------------------------------------------------------------------------
// Constructor. Specify where to report errors.
//-----------------------------------------------------------------------------

Resources::Resources(std::ostream* err) :
    resources(),
    _err(err)
{
}


//-----------------------------------------------------------------------------
// Report an error.
//-----------------------------------------------------------------------------

void Resources::error(const std::string& message)
{
    if (_err != nullptr) {
        *_err << message << std::endl;
    }
}


//-----------------------------------------------------------------------------
// Load the resources from one file.
//-----------------------------------------------------------------------------

bool Resources::load(const std::string& filename)
{
    ::HMODULE hmod = ::LoadLibraryExA(filename.c_str(), nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
    if (hmod == nullptr) {
        const ::DWORD err = ::GetLastError();
        error("error opening " + filename + ": " + Error(err));
        resources.clear();
        return false;
    }
    else {
        const bool success = load(hmod);
        ::FreeLibrary(hmod);
        return success;
    }
}

bool Resources::load(::HMODULE hmod)
{
    resources.clear();
    if (!::EnumResourceTypesA(hmod, Resources::enumResourceType, ::LONG_PTR(this))) {
        const ::DWORD err = ::GetLastError();
        error("error enumerating resource types: " + Error(err));
        return false;
    }
    if (!::EnumResourceNamesA(hmod, RT_STRING, Resources::enumResourceName, ::LONG_PTR(this))) {
        const ::DWORD err = ::GetLastError();
        error("error enumerating resource names: " + Error(err));
        return false;
    }
    return true;
}


//-----------------------------------------------------------------------------
// Called for each resource type.
//-----------------------------------------------------------------------------

::BOOL Resources::enumResourceType(::HMODULE hmod, ::LPSTR type, ::LONG_PTR param)
{
    Resources* instance = reinterpret_cast<Resources*>(param);

    if (IS_INTRESOURCE(type)) {
        std::cout << "Type: " << uintptr_t(type) << std::endl;
    }
    else {
        std::cout << "Type: \"" << type << "\"" << std::endl;
    }
    ::EnumResourceNamesA(hmod, type, Resources::enumResourceName, param);

    if (instance != nullptr) {
        //@@@@
    }
    return true;
}


//-----------------------------------------------------------------------------
// Called for each resource name.
//-----------------------------------------------------------------------------

::BOOL Resources::enumResourceName(::HMODULE hmod, ::LPCSTR type, ::LPSTR name, ::LONG_PTR param)
{
    if (IS_INTRESOURCE(name)) {
        std::cout << "Name: " << uintptr_t(name) << std::endl;
    }
    else {
        std::cout << "Name: \"" << name << "\"" << std::endl;
    }
    return true;
}
