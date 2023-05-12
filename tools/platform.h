//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Common header for Windows platform.
//
//----------------------------------------------------------------------------

#pragma once

#define _CRT_SECURE_NO_WARNINGS 1 // don't complain about string rtl.

#include <windows.h>
#include <psapi.h>
#include <kbd.h>

#if defined(min)
    #undef min
#endif
#if defined(max)
    #undef max
#endif

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

// Registry entry of all keyboard layouts.
#define REGISTRY_LAYOUT_KEY        L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts"
#define REGISTRY_USER_PRELOAD_KEY  L"HKEY_CURRENT_USER\\Keyboard Layout\\Preload"
#define REGISTRY_USER_SUBSTS_KEY   L"HKEY_CURRENT_USER\\Keyboard Layout\\Substitutes"
#define REGISTRY_LAYOUT_DISPLAY    L"Layout Display Name"
#define REGISTRY_LAYOUT_FILE       L"Layout File"
#define REGISTRY_LAYOUT_ID         L"Layout Id"
#define REGISTRY_LAYOUT_PROVIDER   L"Layout Provider"
#define REGISTRY_LAYOUT_TEXT       L"Layout Text"

// Entry point of all keyboard layout DLL's.
#define KBD_DLL_ENTRY_NAME "KbdLayerDescriptor"
