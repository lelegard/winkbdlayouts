//---------------------------------------------------------------------------
// United States ISO/RU Apple Keyboard Layout (WKL)
// Automatically generated from kbdprlur.dll
//---------------------------------------------------------------------------

#define KBD_TYPE 4

#include <windows.h>
#include <kbd.h>
#include <dontuse.h>

//---------------------------------------------------------------------------
// Associate a virtual key with a modifier bitmask
//---------------------------------------------------------------------------

static VK_TO_BIT vk_to_bits[] = {
    {VK_SHIFT,   KBDSHIFT},
    {VK_CONTROL, KBDCTRL},
    {VK_MENU,    KBDALT},
    {0,          0}
};

//---------------------------------------------------------------------------
// Map character modifier bits to modification number
//---------------------------------------------------------------------------

static MODIFIERS char_modifiers = {
    .pVkToBit    = vk_to_bits,
    .wMaxModBits = 7,
    .ModNumber   = {
        0,            // 000 = <none>
        1,            // 001 = SHIFT
        2,            // 010 = ALT
        SHFT_INVALID, // 011 = SHIFT ALT
        SHFT_INVALID, // 100 = CTRL
        SHFT_INVALID, // 101 = SHIFT CTRL
        3,            // 110 = CTRL ALT
    }
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 3 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS3 vk_to_wchar3[] = {
    {VK_BACK,   0x00, {0x0008, 0x0008, 0x007F}},
    {VK_ESCAPE, 0x00, {0x001B, 0x001B, 0x001B}},
    {VK_RETURN, 0x00, {L'\r',  L'\r',  L'\n'}},
    {VK_CANCEL, 0x00, {0x0003, 0x0003, 0x0003}},
    {0,         0,    0,       0,      0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 5 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS5 vk_to_wchar5[] = {
    {'1',           0x00,   {L'1',     L'!',     WCH_NONE, 0x00A1,   0x2044}},
    {'2',           0x00,   {L'2',     L'@',     WCH_NONE, 0x2122,   0x20AC}},
    {'3',           0x00,   {L'3',     L'#',     WCH_NONE, 0x00A3,   0x2039}},
    {'4',           0x00,   {L'4',     L'$',     WCH_NONE, 0x00A2,   0x203A}},
    {'5',           0x00,   {L'5',     L'%',     WCH_NONE, 0x221E,   0xFB01}},
    {'6',           0x00,   {L'6',     L'^',     WCH_NONE, 0x00A7,   0xFB02}},
    {'7',           0x00,   {L'7',     L'&',     WCH_NONE, 0x00B6,   0x2021}},
    {'8',           0x00,   {L'8',     L'*',     WCH_NONE, 0x2022,   0x00B0}},
    {'9',           0x00,   {L'9',     L'(',     WCH_NONE, 0x00AA,   0x00B7}},
    {'0',           0x00,   {L'0',     L')',     WCH_NONE, 0x00BA,   0x201A}},
    {VK_OEM_MINUS,  0x00,   {L'-',     L'_',     WCH_NONE, 0x2013,   0x2014}},
    {VK_OEM_PLUS,   0x00,   {L'=',     L'+',     WCH_NONE, 0x2260,   0x00B1}},
    {'Q',           SGCAPS, {L'q',     L'Q',     WCH_NONE, 0x0153,   0x0152}},
    {'Q',           0x00,   {L'Q',     L'Q',     0x0000,   0x0000,   0x0000}},
    {'W',           SGCAPS, {L'w',     L'W',     WCH_NONE, 0x2211,   0x201E}},
    {'W',           0x00,   {L'W',     L'W',     0x0000,   0x0000,   0x0000}},
    {'E',           SGCAPS, {L'e',     L'E',     WCH_NONE, WCH_DEAD, 0x00B4}},
    {VK__none_,     0x00,   {L'E',     L'E',     WCH_NONE, 0x00B4,   WCH_NONE}},
    {'R',           SGCAPS, {L'r',     L'R',     WCH_NONE, 0x00AE,   0x2030}},
    {'R',           0x00,   {L'R',     L'R',     0x0000,   0x0000,   0x0000}},
    {'T',           SGCAPS, {L't',     L'T',     WCH_NONE, 0x2020,   0x02C7}},
    {'T',           0x00,   {L'T',     L'T',     0x0000,   0x0000,   0x0000}},
    {'Y',           SGCAPS, {L'y',     L'Y',     WCH_NONE, 0x00A5,   0x00C1}},
    {'Y',           0x00,   {L'Y',     L'Y',     0x0000,   0x0000,   0x0000}},
    {'U',           SGCAPS, {L'u',     L'U',     WCH_NONE, WCH_DEAD, 0x00A8}},
    {VK__none_,     0x00,   {L'U',     L'U',     WCH_NONE, 0x00A8,   WCH_NONE}},
    {'I',           SGCAPS, {L'i',     L'I',     WCH_NONE, WCH_DEAD, 0x02C6}},
    {VK__none_,     0x00,   {L'I',     L'I',     WCH_NONE, L'^',     WCH_NONE}},
    {'O',           SGCAPS, {L'o',     L'O',     WCH_NONE, 0x00F8,   0x00D8}},
    {'O',           0x00,   {L'O',     L'O',     0x0000,   0x0000,   0x0000}},
    {'P',           SGCAPS, {L'p',     L'P',     WCH_NONE, 0x03C0,   0x220F}},
    {'P',           0x00,   {L'P',     L'P',     0x0000,   0x0000,   0x0000}},
    {VK_OEM_4,      0x00,   {L'[',     L'{',     WCH_NONE, 0x201C,   0x201D}},
    {VK_OEM_6,      0x00,   {L']',     L'}',     WCH_NONE, 0x2018,   0x2019}},
    {'A',           SGCAPS, {L'a',     L'A',     WCH_NONE, 0x00E5,   0x00C5}},
    {'A',           0x00,   {L'A',     L'A',     0x0000,   0x0000,   0x0000}},
    {'S',           SGCAPS, {L's',     L'S',     WCH_NONE, 0x00DF,   0x00CD}},
    {'S',           0x00,   {L'S',     L'S',     0x0000,   0x0000,   0x0000}},
    {'D',           SGCAPS, {L'd',     L'D',     WCH_NONE, 0x2202,   0x00CE}},
    {'D',           0x00,   {L'D',     L'D',     0x0000,   0x0000,   0x0000}},
    {'F',           SGCAPS, {L'f',     L'F',     WCH_NONE, 0x0192,   0x00CF}},
    {'F',           0x00,   {L'F',     L'F',     0x0000,   0x0000,   0x0000}},
    {'G',           SGCAPS, {L'g',     L'G',     WCH_NONE, 0x00A9,   0x02DD}},
    {'G',           0x00,   {L'G',     L'G',     0x0000,   0x0000,   0x0000}},
    {'H',           SGCAPS, {L'h',     L'H',     WCH_NONE, 0x02D9,   0x00D3}},
    {'H',           0x00,   {L'H',     L'H',     0x0000,   0x0000,   0x0000}},
    {'J',           SGCAPS, {L'j',     L'J',     WCH_NONE, 0x2206,   0x00D4}},
    {'J',           0x00,   {L'J',     L'J',     0x0000,   0x0000,   0x0000}},
    {'K',           SGCAPS, {L'k',     L'K',     WCH_NONE, 0x02DA,   0x2225}},
    {'K',           0x00,   {L'K',     L'K',     0x0000,   0x0000,   0x0000}},
    {'L',           SGCAPS, {L'l',     L'L',     WCH_NONE, 0x00AC,   0x00D2}},
    {'L',           0x00,   {L'L',     L'L',     0x0000,   0x0000,   0x0000}},
    {VK_OEM_1,      0x00,   {L';',     L':',     WCH_NONE, 0x2026,   0x00DA}},
    {VK_OEM_3,      0x00,   {L'\'',    L'"',     WCH_NONE, 0x00E6,   0x00C6}},
    {VK_OEM_8,      0x00,   {0x00A7,   0x00B1,   WCH_NONE, 0x00A7,   0x00B1}},
    {VK_OEM_7,      0x00,   {L'\\',    L'|',     WCH_NONE, 0x00AB,   0x00BB}},
    {'Z',           SGCAPS, {L'z',     L'Z',     WCH_NONE, 0x03A9,   0x00B8}},
    {'Z',           0x00,   {L'Z',     L'Z',     0x0000,   0x0000,   0x0000}},
    {'X',           SGCAPS, {L'x',     L'X',     WCH_NONE, 0x2248,   0x02DB}},
    {'X',           0x00,   {L'X',     L'X',     0x0000,   0x0000,   0x0000}},
    {'C',           SGCAPS, {L'c',     L'C',     WCH_NONE, 0x00E7,   0x00C7}},
    {'C',           0x00,   {L'C',     L'C',     0x0000,   0x0000,   0x0000}},
    {'V',           SGCAPS, {L'v',     L'V',     WCH_NONE, 0x221A,   0x25CA}},
    {'V',           0x00,   {L'V',     L'V',     0x0000,   0x0000,   0x0000}},
    {'B',           SGCAPS, {L'b',     L'B',     WCH_NONE, 0x222B,   0x0131}},
    {'B',           0x00,   {L'B',     L'B',     0x0000,   0x0000,   0x0000}},
    {'N',           SGCAPS, {L'n',     L'N',     WCH_NONE, WCH_DEAD, 0x02DC}},
    {VK__none_,     0x00,   {L'N',     L'N',     WCH_NONE, L'~',     WCH_NONE}},
    {'M',           SGCAPS, {L'm',     L'M',     WCH_NONE, 0x00B5,   0x00C2}},
    {'M',           0x00,   {L'M',     L'M',     0x0000,   0x0000,   0x0000}},
    {VK_OEM_COMMA,  0x00,   {L',',     L'<',     WCH_NONE, 0x2264,   0x00AF}},
    {VK_OEM_PERIOD, 0x00,   {L'.',     L'>',     WCH_NONE, 0x2265,   0x02D8}},
    {VK_OEM_2,      0x00,   {L'/',     L'?',     WCH_NONE, 0x00F7,   0x00BF}},
    {VK_SPACE,      0x00,   {L' ',     L' ',     L' ',     0x00A0,   0x00A0}},
    {VK_OEM_5,      0x00,   {L'`',     L'~',     WCH_NONE, WCH_DEAD, L'`'}},
    {VK__none_,     0x00,   {WCH_NONE, WCH_NONE, WCH_NONE, L'`',     WCH_NONE}},
    {VK_DECIMAL,    0x00,   {L'.',     L'.',     WCH_NONE, WCH_NONE, WCH_NONE}},
    {0,             0,      0,         0,        0,        0,        0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 2 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS2 vk_to_wchar2[] = {
    {VK_TAB,      0x00, {L'\t', L'\t'}},
    {VK_ADD,      0x00, {L'+',  L'+'}},
    {VK_DIVIDE,   0x00, {L'/',  L'/'}},
    {VK_MULTIPLY, 0x00, {L'*',  L'*'}},
    {VK_SUBTRACT, 0x00, {L'-',  L'-'}},
    {VK_CLEAR,    0x00, {L'=',  L'='}},
    {0,           0,    0,      0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 1 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS1 vk_to_wchar1[] = {
    {VK_NUMPAD0, 0x00, {L'0'}},
    {VK_NUMPAD1, 0x00, {L'1'}},
    {VK_NUMPAD2, 0x00, {L'2'}},
    {VK_NUMPAD3, 0x00, {L'3'}},
    {VK_NUMPAD4, 0x00, {L'4'}},
    {VK_NUMPAD5, 0x00, {L'5'}},
    {VK_NUMPAD6, 0x00, {L'6'}},
    {VK_NUMPAD7, 0x00, {L'7'}},
    {VK_NUMPAD8, 0x00, {L'8'}},
    {VK_NUMPAD9, 0x00, {L'9'}},
    {0,          0,    0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations with shift states
//---------------------------------------------------------------------------

static VK_TO_WCHAR_TABLE vk_to_wchar[] = {
    {(PVK_TO_WCHARS1)vk_to_wchar3, 3, sizeof(vk_to_wchar3[0])},
    {(PVK_TO_WCHARS1)vk_to_wchar5, 5, sizeof(vk_to_wchar5[0])},
    {(PVK_TO_WCHARS1)vk_to_wchar2, 2, sizeof(vk_to_wchar2[0])},
    {(PVK_TO_WCHARS1)vk_to_wchar1, 1, sizeof(vk_to_wchar1[0])},
    {NULL,                         0, 0}
};

//---------------------------------------------------------------------------
// Dead keys sequences translations
//---------------------------------------------------------------------------

static DEADKEY dead_keys[] = {
    DEADTRANS(L'e', 0x00B4, 0x00E9, 0x0000),
    DEADTRANS(L'u', 0x00B4, 0x00FA, 0x0000),
    DEADTRANS(L'i', 0x00B4, 0x00ED, 0x0000),
    DEADTRANS(L'y', 0x00B4, 0x00FD, 0x0000),
    DEADTRANS(L'o', 0x00B4, 0x00F3, 0x0000),
    DEADTRANS(L'a', 0x00B4, 0x00E1, 0x0000),
    DEADTRANS(L'E', 0x00B4, 0x00C9, 0x0000),
    DEADTRANS(L'U', 0x00B4, 0x00DA, 0x0000),
    DEADTRANS(L'I', 0x00B4, 0x00CD, 0x0000),
    DEADTRANS(L'Y', 0x00B4, 0x00DD, 0x0000),
    DEADTRANS(L'O', 0x00B4, 0x00D3, 0x0000),
    DEADTRANS(L'A', 0x00B4, 0x00C1, 0x0000),
    DEADTRANS(L'n', 0x00B4, 0x0144, 0x0000),
    DEADTRANS(L'c', 0x00B4, 0x0107, 0x0000),
    DEADTRANS(L's', 0x00B4, 0x015B, 0x0000),
    DEADTRANS(L'l', 0x00B4, 0x013A, 0x0000),
    DEADTRANS(L'r', 0x00B4, 0x0155, 0x0000),
    DEADTRANS(L'z', 0x00B4, 0x017A, 0x0000),
    DEADTRANS(L'N', 0x00B4, 0x0143, 0x0000),
    DEADTRANS(L'C', 0x00B4, 0x0106, 0x0000),
    DEADTRANS(L'S', 0x00B4, 0x015A, 0x0000),
    DEADTRANS(L'L', 0x00B4, 0x0139, 0x0000),
    DEADTRANS(L'R', 0x00B4, 0x0154, 0x0000),
    DEADTRANS(L'Z', 0x00B4, 0x0179, 0x0000),
    DEADTRANS(L' ', 0x00B4, 0x00B4, 0x0000),
    DEADTRANS(L'e', 0x00A8, 0x00EB, 0x0000),
    DEADTRANS(L'u', 0x00A8, 0x00FC, 0x0000),
    DEADTRANS(L'i', 0x00A8, 0x00EF, 0x0000),
    DEADTRANS(L'y', 0x00A8, 0x00FF, 0x0000),
    DEADTRANS(L'o', 0x00A8, 0x00F6, 0x0000),
    DEADTRANS(L'a', 0x00A8, 0x00E4, 0x0000),
    DEADTRANS(L'E', 0x00A8, 0x00CB, 0x0000),
    DEADTRANS(L'U', 0x00A8, 0x00DC, 0x0000),
    DEADTRANS(L'I', 0x00A8, 0x00CF, 0x0000),
    DEADTRANS(L'Y', 0x00A8, 0x0178, 0x0000),
    DEADTRANS(L'O', 0x00A8, 0x00D6, 0x0000),
    DEADTRANS(L'A', 0x00A8, 0x00C4, 0x0000),
    DEADTRANS(L' ', 0x00A8, 0x00A8, 0x0000),
    DEADTRANS(L'e', L'^',   0x00EA, 0x0000),
    DEADTRANS(L'u', L'^',   0x00FB, 0x0000),
    DEADTRANS(L'i', L'^',   0x00EE, 0x0000),
    DEADTRANS(L'o', L'^',   0x00F4, 0x0000),
    DEADTRANS(L'a', L'^',   0x00E2, 0x0000),
    DEADTRANS(L'E', L'^',   0x00CA, 0x0000),
    DEADTRANS(L'U', L'^',   0x00DB, 0x0000),
    DEADTRANS(L'I', L'^',   0x00CE, 0x0000),
    DEADTRANS(L'O', L'^',   0x00D4, 0x0000),
    DEADTRANS(L'A', L'^',   0x00C2, 0x0000),
    DEADTRANS(L'c', L'^',   0x0109, 0x0000),
    DEADTRANS(L'h', L'^',   0x0125, 0x0000),
    DEADTRANS(L'j', L'^',   0x0135, 0x0000),
    DEADTRANS(L'g', L'^',   0x011D, 0x0000),
    DEADTRANS(L's', L'^',   0x015D, 0x0000),
    DEADTRANS(L'w', L'^',   0x0175, 0x0000),
    DEADTRANS(L'y', L'^',   0x0177, 0x0000),
    DEADTRANS(L'C', L'^',   0x0108, 0x0000),
    DEADTRANS(L'H', L'^',   0x0124, 0x0000),
    DEADTRANS(L'J', L'^',   0x0134, 0x0000),
    DEADTRANS(L'G', L'^',   0x011C, 0x0000),
    DEADTRANS(L'S', L'^',   0x015C, 0x0000),
    DEADTRANS(L'W', L'^',   0x0174, 0x0000),
    DEADTRANS(L'Y', L'^',   0x0176, 0x0000),
    DEADTRANS(L' ', L'^',   L'^',   0x0000),
    DEADTRANS(L'n', L'~',   0x00F1, 0x0000),
    DEADTRANS(L'o', L'~',   0x00F5, 0x0000),
    DEADTRANS(L'a', L'~',   0x00E3, 0x0000),
    DEADTRANS(L'N', L'~',   0x00D1, 0x0000),
    DEADTRANS(L'O', L'~',   0x00D5, 0x0000),
    DEADTRANS(L'A', L'~',   0x00C3, 0x0000),
    DEADTRANS(L'u', L'~',   0x0169, 0x0000),
    DEADTRANS(L'i', L'~',   0x0129, 0x0000),
    DEADTRANS(L'U', L'~',   0x0168, 0x0000),
    DEADTRANS(L'I', L'~',   0x0128, 0x0000),
    DEADTRANS(L' ', L'~',   L'~',   0x0000),
    DEADTRANS(L'e', L'`',   0x00E8, 0x0000),
    DEADTRANS(L'u', L'`',   0x00F9, 0x0000),
    DEADTRANS(L'i', L'`',   0x00EC, 0x0000),
    DEADTRANS(L'o', L'`',   0x00F2, 0x0000),
    DEADTRANS(L'a', L'`',   0x00E0, 0x0000),
    DEADTRANS(L'E', L'`',   0x00C8, 0x0000),
    DEADTRANS(L'U', L'`',   0x00D9, 0x0000),
    DEADTRANS(L'I', L'`',   0x00CC, 0x0000),
    DEADTRANS(L'O', L'`',   0x00D2, 0x0000),
    DEADTRANS(L'A', L'`',   0x00C0, 0x0000),
    DEADTRANS(L' ', L'`',   L'`',   0x0000),
    {0, 0, 0}
};

//---------------------------------------------------------------------------
// Scan codes to key names
//---------------------------------------------------------------------------

static VSC_LPWSTR key_names[] = {
    {0x01, L"Esc"},
    {0x0E, L"Backspace"},
    {0x0F, L"Tab"},
    {0x1C, L"Enter"},
    {0x1D, L"Ctrl"},
    {0x2A, L"Shift"},
    {0x36, L"Right Shift"},
    {0x37, L"Num *"},
    {0x38, L"Alt"},
    {0x39, L"Space"},
    {0x3A, L"Caps Lock"},
    {0x3B, L"F1"},
    {0x3C, L"F2"},
    {0x3D, L"F3"},
    {0x3E, L"F4"},
    {0x3F, L"F5"},
    {0x40, L"F6"},
    {0x41, L"F7"},
    {0x42, L"F8"},
    {0x43, L"F9"},
    {0x44, L"F10"},
    {0x45, L"Pause"},
    {0x46, L"Scroll Lock"},
    {0x47, L"Num 7"},
    {0x48, L"Num 8"},
    {0x49, L"Num 9"},
    {0x4A, L"Num -"},
    {0x4B, L"Num 4"},
    {0x4C, L"Num 5"},
    {0x4D, L"Num 6"},
    {0x4E, L"Num +"},
    {0x4F, L"Num 1"},
    {0x50, L"Num 2"},
    {0x51, L"Num 3"},
    {0x52, L"Num 0"},
    {0x53, L"Num Del"},
    {0x54, L"Sys Req"},
    {0x57, L"F11"},
    {0x58, L"F12"},
    {0x7C, L"F13"},
    {0x7D, L"F14"},
    {0x7E, L"F15"},
    {0x7F, L"F16"},
    {0x80, L"F17"},
    {0x81, L"F18"},
    {0x82, L"F19"},
    {0x83, L"F20"},
    {0x84, L"F21"},
    {0x85, L"F22"},
    {0x86, L"F23"},
    {0x87, L"F24"},
    {0x00, NULL}
};

//---------------------------------------------------------------------------
// Scan codes to key names (extended keypad)
//---------------------------------------------------------------------------

static VSC_LPWSTR key_names_ext[] = {
    {0x1C, L"Num Enter"},
    {0x1D, L"Right Ctrl"},
    {0x35, L"Num /"},
    {0x37, L"Prnt Scrn"},
    {0x38, L"Right Alt"},
    {0x45, L"Num Lock"},
    {0x46, L"Break"},
    {0x47, L"Home"},
    {0x48, L"Up"},
    {0x49, L"Page Up"},
    {0x4B, L"Left"},
    {0x4D, L"Right"},
    {0x4F, L"End"},
    {0x50, L"Down"},
    {0x51, L"Page Down"},
    {0x52, L"Insert"},
    {0x53, L"Delete"},
    {0x54, L"<00>"},
    {0x56, L"Help"},
    {0x5B, L"Left Windows"},
    {0x5C, L"Right Windows"},
    {0x5D, L"Application"},
    {0x00, NULL}
};

//---------------------------------------------------------------------------
// Names of dead keys
//---------------------------------------------------------------------------

static DEADKEY_LPWSTR key_names_dead[] = {
    L"\x00b4" L"ACUTE ACCENT",
    L"\x00a8" L"DIAERESIS",
    L"^"      L"CIRCUMFLEX ACCENT",
    L"~"      L"TILDE",
    L"`"      L"GRAVE ACCENT",
    NULL
};

//---------------------------------------------------------------------------
// Scan code to virtual key conversion table
//---------------------------------------------------------------------------

static USHORT scancode_to_vk[] = {
    /* 00 */ VK__none_,
    /* 01 */ VK_ESCAPE,
    /* 02 */ '1',
    /* 03 */ '2',
    /* 04 */ '3',
    /* 05 */ '4',
    /* 06 */ '5',
    /* 07 */ '6',
    /* 08 */ '7',
    /* 09 */ '8',
    /* 0A */ '9',
    /* 0B */ '0',
    /* 0C */ VK_OEM_MINUS,
    /* 0D */ VK_OEM_PLUS,
    /* 0E */ VK_BACK,
    /* 0F */ VK_TAB,
    /* 10 */ 'Q',
    /* 11 */ 'W',
    /* 12 */ 'E',
    /* 13 */ 'R',
    /* 14 */ 'T',
    /* 15 */ 'Y',
    /* 16 */ 'U',
    /* 17 */ 'I',
    /* 18 */ 'O',
    /* 19 */ 'P',
    /* 1A */ VK_OEM_4,
    /* 1B */ VK_OEM_6,
    /* 1C */ VK_RETURN,
    /* 1D */ VK_LCONTROL,
    /* 1E */ 'A',
    /* 1F */ 'S',
    /* 20 */ 'D',
    /* 21 */ 'F',
    /* 22 */ 'G',
    /* 23 */ 'H',
    /* 24 */ 'J',
    /* 25 */ 'K',
    /* 26 */ 'L',
    /* 27 */ VK_OEM_1,
    /* 28 */ VK_OEM_3,
    /* 29 */ VK_OEM_8,
    /* 2A */ VK_LSHIFT,
    /* 2B */ VK_OEM_7,
    /* 2C */ 'Z',
    /* 2D */ 'X',
    /* 2E */ 'C',
    /* 2F */ 'V',
    /* 30 */ 'B',
    /* 31 */ 'N',
    /* 32 */ 'M',
    /* 33 */ VK_OEM_COMMA,
    /* 34 */ VK_OEM_PERIOD,
    /* 35 */ VK_OEM_2,
    /* 36 */ VK_RSHIFT | KBDEXT,
    /* 37 */ VK_MULTIPLY | KBDMULTIVK,
    /* 38 */ VK_LMENU,
    /* 39 */ VK_SPACE,
    /* 3A */ VK_CAPITAL,
    /* 3B */ VK_F1,
    /* 3C */ VK_F2,
    /* 3D */ VK_F3,
    /* 3E */ VK_F4,
    /* 3F */ VK_F5,
    /* 40 */ VK_F6,
    /* 41 */ VK_F7,
    /* 42 */ VK_F8,
    /* 43 */ VK_F9,
    /* 44 */ VK_F10,
    /* 45 */ VK_NUMLOCK | KBDEXT | KBDMULTIVK,
    /* 46 */ VK_SCROLL | KBDMULTIVK,
    /* 47 */ VK_HOME | KBDSPECIAL | KBDNUMPAD,
    /* 48 */ VK_UP | KBDSPECIAL | KBDNUMPAD,
    /* 49 */ VK_PRIOR | KBDSPECIAL | KBDNUMPAD,
    /* 4A */ VK_SUBTRACT,
    /* 4B */ VK_LEFT | KBDSPECIAL | KBDNUMPAD,
    /* 4C */ VK_CLEAR | KBDSPECIAL | KBDNUMPAD,
    /* 4D */ VK_RIGHT | KBDSPECIAL | KBDNUMPAD,
    /* 4E */ VK_ADD,
    /* 4F */ VK_END | KBDSPECIAL | KBDNUMPAD,
    /* 50 */ VK_DOWN | KBDSPECIAL | KBDNUMPAD,
    /* 51 */ VK_NEXT | KBDSPECIAL | KBDNUMPAD,
    /* 52 */ VK_INSERT | KBDSPECIAL | KBDNUMPAD,
    /* 53 */ VK_DELETE | KBDSPECIAL | KBDNUMPAD,
    /* 54 */ VK_SNAPSHOT,
    /* 55 */ VK__none_,
    /* 56 */ VK_OEM_5,
    /* 57 */ VK_F11,
    /* 58 */ VK_F12,
    /* 59 */ VK_CLEAR,
    /* 5A */ VK_OEM_WSCTRL,
    /* 5B */ VK_OEM_FINISH,
    /* 5C */ VK_OEM_JUMP,
    /* 5D */ VK_EREOF,
    /* 5E */ VK_OEM_BACKTAB,
    /* 5F */ VK_OEM_AUTO,
    /* 60 */ VK__none_,
    /* 61 */ VK__none_,
    /* 62 */ VK_ZOOM,
    /* 63 */ VK_HELP,
    /* 64 */ VK_F13,
    /* 65 */ VK_F14,
    /* 66 */ VK_F15,
    /* 67 */ VK_F16,
    /* 68 */ VK_F17,
    /* 69 */ VK_F18,
    /* 6A */ VK_F19,
    /* 6B */ VK_F20,
    /* 6C */ VK_F21,
    /* 6D */ VK_F22,
    /* 6E */ VK_F23,
    /* 6F */ VK_OEM_PA3,
    /* 70 */ VK__none_,
    /* 71 */ VK_OEM_RESET,
    /* 72 */ VK__none_,
    /* 73 */ 0x00C1,
    /* 74 */ VK__none_,
    /* 75 */ VK__none_,
    /* 76 */ VK_F24,
    /* 77 */ VK__none_,
    /* 78 */ VK__none_,
    /* 79 */ VK__none_,
    /* 7A */ VK__none_,
    /* 7B */ VK_OEM_PA1,
    /* 7C */ VK_TAB,
    /* 7D */ VK__none_,
    /* 7E */ 0x00C2,
};

//---------------------------------------------------------------------------
// Scan code to virtual key conversion table (scancodes with E0 prefix)
//---------------------------------------------------------------------------

static VSC_VK scancode_to_vk_e0[] = {
    {0x10, VK_MEDIA_PREV_TRACK | KBDEXT},
    {0x19, VK_MEDIA_NEXT_TRACK | KBDEXT},
    {0x1D, VK_RCONTROL | KBDEXT},
    {0x20, VK_VOLUME_MUTE | KBDEXT},
    {0x21, VK_LAUNCH_APP2 | KBDEXT},
    {0x22, VK_MEDIA_PLAY_PAUSE | KBDEXT},
    {0x24, VK_MEDIA_STOP | KBDEXT},
    {0x2E, VK_VOLUME_DOWN | KBDEXT},
    {0x30, VK_VOLUME_UP | KBDEXT},
    {0x32, VK_BROWSER_HOME | KBDEXT},
    {0x35, VK_DIVIDE | KBDEXT},
    {0x37, VK_SNAPSHOT | KBDEXT},
    {0x38, VK_RMENU | KBDEXT},
    {0x47, VK_HOME | KBDEXT},
    {0x48, VK_UP | KBDEXT},
    {0x49, VK_PRIOR | KBDEXT},
    {0x4B, VK_LEFT | KBDEXT},
    {0x4D, VK_RIGHT | KBDEXT},
    {0x4F, VK_END | KBDEXT},
    {0x50, VK_DOWN | KBDEXT},
    {0x51, VK_NEXT | KBDEXT},
    {0x52, VK_INSERT | KBDEXT},
    {0x53, VK_DELETE | KBDEXT},
    {0x5B, VK_LWIN | KBDEXT},
    {0x5C, VK_RWIN | KBDEXT},
    {0x5D, VK_APPS | KBDEXT},
    {0x5F, VK_SLEEP | KBDEXT},
    {0x65, VK_BROWSER_SEARCH | KBDEXT},
    {0x66, VK_BROWSER_FAVORITES | KBDEXT},
    {0x67, VK_BROWSER_REFRESH | KBDEXT},
    {0x68, VK_BROWSER_STOP | KBDEXT},
    {0x69, VK_BROWSER_FORWARD | KBDEXT},
    {0x6A, VK_BROWSER_BACK | KBDEXT},
    {0x6B, VK_LAUNCH_APP1 | KBDEXT},
    {0x6C, VK_LAUNCH_MAIL | KBDEXT},
    {0x6D, VK_LAUNCH_MEDIA_SELECT | KBDEXT},
    {0x1C, VK_RETURN | KBDEXT},
    {0x46, VK_CANCEL | KBDEXT},
    {0x00, 0x0000}
};

//---------------------------------------------------------------------------
// Scan code to virtual key conversion table (scancodes with E1 prefix)
//---------------------------------------------------------------------------

static VSC_VK scancode_to_vk_e1[] = {
    {0x1D, VK_PAUSE},
    {0x00, 0x0000}
};

//---------------------------------------------------------------------------
// Main keyboard layout structure, point to all tables
//---------------------------------------------------------------------------

static KBDTABLES kbd_tables = {
    .pCharModifiers  = &char_modifiers,
    .pVkToWcharTable = vk_to_wchar,
    .pDeadKey        = dead_keys,
    .pKeyNames       = key_names,
    .pKeyNamesExt    = key_names_ext,
    .pKeyNamesDead   = key_names_dead,
    .pusVSCtoVK      = scancode_to_vk,
    .bMaxVSCtoVK     = ARRAYSIZE(scancode_to_vk),
    .pVSCtoVK_E0     = scancode_to_vk_e0,
    .pVSCtoVK_E1     = scancode_to_vk_e1,
    .fLocaleFlags    = MAKELONG(KLLF_ALTGR, KBD_VERSION),
    .nLgMax          = 0,
    .cbLgEntry       = 0,
    .pLigature       = NULL,
    .dwType          = 4,
    .dwSubType       = 0,
};

//---------------------------------------------------------------------------
// Keyboard layout entry point
//---------------------------------------------------------------------------

__declspec(dllexport) PKBDTABLES KbdLayerDescriptor(void)
{
    return &kbd_tables;
}
