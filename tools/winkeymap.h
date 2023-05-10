//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Description of a Windows key in a keyboard.
//
//----------------------------------------------------------------------------

#pragma once
#include "strutils.h"

// Description of one virtual key.
class VirtualKey
{
public:
    VirtualKey();    // Constructor.
    uint16_t vk;     // Virtual key, without modifier. Zero means unused.
    WString  wc;     // Unicode characters. Index in string is a bitmask of KBDSHIFT, KBDCTRL, KBDALT. Zero means unused.
};

// Description of one key
class WinKey
{
public:
    WinKey();        // Constructor.
    uint16_t   sc;   // Scan code. Zero means unused.
    VirtualKey vk;   // Virtual key.
    VirtualKey evk;  // Extended virtual key.
};

// A vector of WinKey. Index in the vector is a scan code.
typedef std::vector<WinKey> WinKeyVector;

// Description of a complete keyboard.
class WinKeyMap
{
public:
    // Constructor.
    WinKeyMap(const KBDTABLES*);

    // Convert a "modifier number" (index in wch[] of VK_TO_WCHARS)
    // into a bitmak of KBDSHIFT, KBDCTRL, KBDALT (0 to 7).
    // Return SHFT_INVALID (>7) if the nodifier number is invalid.
    size_t modNumberToModMask(size_t modnum)
    {
        return modnum < _mods.size() ? _mods[modnum] : SHFT_INVALID;
    }

    // Get a map of all scan codes in a keymap.
    void buildKeyMap(WinKeyVector&);

private:
    const KBDTABLES*    _tables;
    std::vector<size_t> _mods;   // Modifier masks, indexed by "modifier number".
};
