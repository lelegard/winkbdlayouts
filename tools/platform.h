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
#include <string>
#include <vector>
#include <list>
#include <map>
