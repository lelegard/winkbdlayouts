//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Description of a Windows key in a keyboard.
//
//----------------------------------------------------------------------------

#include "winkeymap.h"


//----------------------------------------------------------------------------
// Constructors.
//----------------------------------------------------------------------------

VirtualKey::VirtualKey() :
    vk(0),
    wc(8, L'\0')
{
}

WinKey::WinKey() :
    sc(0),
    vk(),
    evk()
{
}

WinKeyMap::WinKeyMap(const KBDTABLES* tables) :
    _tables(tables),
    _mods(8, SHFT_INVALID)
{
    // Build the conversion table from "modifier number" to modifier masks.
    if (_tables != nullptr && _tables->pCharModifiers != nullptr) {
        for (size_t i = 0; i <= _tables->pCharModifiers->wMaxModBits; ++i) {
            if (_tables->pCharModifiers->ModNumber[i] < _mods.size()) {
                _mods[_tables->pCharModifiers->ModNumber[i]] = i;
            }
        }
    }
}


//----------------------------------------------------------------------------
// Convert a "modifier number" (index in wch[] of VK_TO_WCHARS)
//----------------------------------------------------------------------------

size_t WinKeyMap::modNumberToModMask(size_t modnum)
{
    return modnum < _mods.size() ? _mods[modnum] : SHFT_INVALID;
}


//----------------------------------------------------------------------------
// Get a map of all scan codes in a keymap.
//----------------------------------------------------------------------------

void WinKeyMap::buildKeyMap(WinKeyVector& keys)
{
    keys.clear();
    if (_tables == nullptr || _tables->pVkToWcharTable == nullptr) {
        return;
    }

    // Build a multimap of virtual key => scan code.
    // The scan code can have attributes KBDEXT.
    std::multimap<uint16_t, uint16_t> vk2sc;
    if (_tables->pusVSCtoVK != nullptr) {
        for (uint16_t i = 0; i < _tables->bMaxVSCtoVK; ++i) {
            if ((_tables->pusVSCtoVK[i] & 0xFF) != VK__none_) {
                vk2sc.insert(std::make_pair(_tables->pusVSCtoVK[i] & 0xFF, i | (_tables->pusVSCtoVK[i] & KBDEXT)));
            }
        }
    }
    if (_tables->pVSCtoVK_E0 != nullptr) {
        for (const VSC_VK* p = _tables->pVSCtoVK_E0; p->Vsc != 0; ++p) {
            vk2sc.insert(std::make_pair(p->Vk & 0xFF, p->Vsc | (p->Vk & KBDEXT)));
        }
    }
    if (_tables->pVSCtoVK_E1 != nullptr) {
        for (const VSC_VK* p = _tables->pVSCtoVK_E1; p->Vsc != 0; ++p) {
            vk2sc.insert(std::make_pair(p->Vk & 0xFF, p->Vsc | (p->Vk & KBDEXT)));
        }
    }

    // Loop on all VK_TO_WCHARS tables.
    for (const VK_TO_WCHAR_TABLE* tab = _tables->pVkToWcharTable; tab->pVkToWchars != nullptr; tab++) {
        
        // Each VK_TO_WCHARS table has a different type. Consider the type with the largest number of elements.
        const VK_TO_WCHARS10* vtwc = reinterpret_cast<const VK_TO_WCHARS10*>(tab->pVkToWchars);
        const size_t wch_count = tab->nModifications;
        const size_t tab_entry_size = tab->cbSize;

        // Loop on all virtual keys in the VK_TO_WCHARS table.
        uint16_t previous_vk = VK__none_;
        while (vtwc->VirtualKey != 0) {
            const uint16_t vk = vtwc->VirtualKey != VK__none_ ? vtwc->VirtualKey : previous_vk;
            if (vk != VK__none_) {
                // Loop on all scan codes for this virtual key.
                for (auto it = vk2sc.lower_bound(vk); it != vk2sc.end() && it->first == vk; ++it) {
                    const uint16_t sc = it->second & 0xFF;
                    const bool extended = (it->second & KBDEXT) != 0;

                    // Make sure the keymap contains the scan code.
                    if (sc >= keys.size()) {
                        keys.resize(sc + 1);
                    }
                    keys[sc].sc = sc;

                    // The virtual key description to update.
                    VirtualKey& vkd(extended ? keys[sc].evk : keys[sc].vk);
                    vkd.vk = vk;

                    // Loop on all modified characters.
                    for (size_t i = 0; i < wch_count; ++i) {
                        const wchar_t wc = vtwc->wch[i];
                        const size_t modmask = modNumberToModMask(i);
                        if (modmask < vkd.wc.size() && wc != 0 && wc != WCH_NONE && wc != WCH_DEAD && wc != WCH_LGTR) {
                            vkd.wc[modmask] = wc;
                        }
                    }
                }
                previous_vk = vk;
            }
            // Move to next structure (variable size).
            vtwc = reinterpret_cast<const VK_TO_WCHARS10*>(reinterpret_cast<const char*>(vtwc) + tab_entry_size);
        }
    }
}
