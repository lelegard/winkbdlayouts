//---------------------------------------------------------------------------
// Czech Apple VM Keyboard Layout (WKL)
// Automatically generated from kbdprlcz.dll
//---------------------------------------------------------------------------

#define KBD_TYPE 4

#include <windows.h>
#include <kbd.h>
#include <dontuse.h>
#include "unicode.h"

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
    L"\x00b0" L"DEGREE SIGN",
    L","      L"COMMA",
    L"\x00b4" L"ACUTE ACCENT",
    L"\x02c7" L"CARON (Mandarin Chinese third tone)",
    L"^"      L"CIRCUMFLEX ACCENT",
    L"-"      L"HYPHEN-MINUS",
    L"~"      L"TILDE",
    L"\x00a8" L"DIAERESIS",
    L"\x00a8" L"DIAERESIS",
    L"\""     L"QUOTATION MARK",
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
    /* 0C */ VK_OEM_PLUS,
    /* 0D */ VK_OEM_2,
    /* 0E */ VK_BACK,
    /* 0F */ VK_TAB,
    /* 10 */ 'Q',
    /* 11 */ 'W',
    /* 12 */ 'E',
    /* 13 */ 'R',
    /* 14 */ 'T',
    /* 15 */ 'Z',
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
    /* 28 */ VK_OEM_7,
    /* 29 */ VK_OEM_3,
    /* 2A */ VK_LSHIFT,
    /* 2B */ VK_OEM_5,
    /* 2C */ 'Y',
    /* 2D */ 'X',
    /* 2E */ 'C',
    /* 2F */ 'V',
    /* 30 */ 'B',
    /* 31 */ 'N',
    /* 32 */ 'M',
    /* 33 */ VK_OEM_COMMA,
    /* 34 */ VK_OEM_PERIOD,
    /* 35 */ VK_OEM_MINUS,
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
    /* 56 */ VK_OEM_7,
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
        1,            // 001 = Shift
        2,            // 010 = Control
        SHFT_INVALID, // 011 = Shift Control
        SHFT_INVALID, // 100 = Alt
        SHFT_INVALID, // 101 = Shift Alt
        3,            // 110 = Control Alt (AltGr)
        4,            // 111 = Shift Control Alt (Shift AltGr)
    }
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 3 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS3 vk_to_wchar3[] = {
    //                         Shift   Ctrl
    //                         -----   ----
    {VK_BACK,   0x00, {UC_BS,  UC_BS,  UC_DEL}},
    {VK_ESCAPE, 0x00, {UC_ESC, UC_ESC, UC_ESC}},
    {VK_RETURN, 0x00, {L'\r',  L'\r',  L'\n'}},
    {VK_CANCEL, 0x00, {UC_ETX, UC_ETX, UC_ETX}},
    {0,         0,    0,       0,      0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 5 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS5 vk_to_wchar5[] = {
    //                                                            Shift     Ctrl      AltGr                 Shift/AltGr
    //                                                            -----     ----      -----                 -----------
    {'1',           CAPLOK,               {L'+',                  L'1',     WCH_NONE, L'`',                 UC_NOT}},
    {'2',           CAPLOK,               {UC_LOWER_E_CARON,      L'2',     WCH_NONE, L'@',                 UC_BULLET}},
    {'3',           CAPLOK,               {UC_LOWER_S_CARON,      L'3',     WCH_NONE, L'#',                 UC_NOT_EQUAL}},
    {'4',           CAPLOK,               {UC_LOWER_C_CARON,      L'4',     WCH_NONE, L'$',                 UC_POUND}},
    {'5',           CAPLOK,               {UC_LOWER_R_CARON,      L'5',     WCH_NONE, L'~',                 UC_LOZENGE}},
    {'6',           CAPLOK,               {UC_LOWER_Z_CARON,      L'6',     WCH_NONE, L'^',                 UC_DAGGER}},
    {'7',           CAPLOK,               {UC_LOWER_Y_ACUTE,      L'7',     WCH_NONE, L'&',                 UC_PILCROW}},
    {'8',           CAPLOK,               {UC_LOWER_A_ACUTE,      L'8',     WCH_NONE, L'*',                 UC_DIVISION}},
    {'9',           CAPLOK,               {UC_LOWER_I_ACUTE,      L'9',     WCH_NONE, L'{',                 UC_DA_LEFT_QUOT}},
    {'0',           CAPLOK,               {UC_LOWER_E_ACUTE,      L'0',     WCH_NONE, L'}',                 UC_DA_RIGHT_QUOT}},
    {VK_OEM_PLUS,   0x00,                 {L'=',                  L'%',     WCH_NONE, WCH_DEAD,             WCH_DEAD}},
    {VK__none_,     0x00,                 {WCH_NONE,              WCH_NONE, WCH_NONE, UC_DEGREE,            L','}},
    {VK_OEM_2,      0x00,                 {WCH_DEAD,              WCH_DEAD, WCH_NONE, WCH_DEAD,             WCH_DEAD}},
    {VK__none_,     0x00,                 {L'\'',                 UC_CARON, WCH_NONE, L'^',                 L'-'}},
    {'Q',           CAPLOK,               {L'q',                  L'Q',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'W',           CAPLOK | CAPLOKALTGR, {L'w',                  L'W',     WCH_NONE, UC_LOWER_E_DOT_ABOVE, UC_UPPER_E_DOT_ABOVE}},
    {'E',           CAPLOK | CAPLOKALTGR, {L'e',                  L'E',     WCH_NONE, UC_LOWER_E_OGONEK,    UC_UPPER_E_OGONEK}},
    {'R',           CAPLOK,               {L'r',                  L'R',     WCH_NONE, UC_EURO,              UC_REGISTERED}},
    {'T',           CAPLOK,               {L't',                  L'T',     WCH_NONE, WCH_NONE,             UC_TRADE}},
    {'Z',           CAPLOK | CAPLOKALTGR, {L'z',                  L'Z',     WCH_NONE, UC_LOWER_Z_DOT_ABOVE, UC_UPPER_Z_DOT_ABOVE}},
    {'U',           CAPLOK,               {L'u',                  L'U',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'I',           CAPLOK,               {L'i',                  L'I',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'O',           CAPLOK,               {L'o',                  L'O',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'P',           CAPLOK,               {L'p',                  L'P',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {VK_OEM_4,      0x00,                 {UC_LOWER_U_ACUTE,      L'/',     WCH_NONE, L'[',                 UC_SA_LEFT_QUOT}},
    {VK_OEM_6,      0x00,                 {L')',                  L'(',     WCH_NONE, L']',                 UC_SA_RIGHT_QUOT}},
    {'A',           CAPLOK | CAPLOKALTGR, {L'a',                  L'A',     WCH_NONE, UC_LOWER_A_OGONEK,    UC_UPPER_A_OGONEK}},
    {'S',           CAPLOK,               {L's',                  L'S',     WCH_NONE, UC_LOWER_SHARP_S,     UC_N_ARY_SUM}},
    {'D',           CAPLOK,               {L'd',                  L'D',     WCH_NONE, UC_PARTIAL_DIFF,      0x2206}},
    {'F',           CAPLOK,               {L'f',                  L'F',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'G',           CAPLOK,               {L'g',                  L'G',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'H',           CAPLOK,               {L'h',                  L'H',     WCH_NONE, UC_S_LEFT_QUOT,       UC_D_LEFT_QUOT}},
    {'J',           CAPLOK,               {L'j',                  L'J',     WCH_NONE, UC_S_RIGHT_QUOT,      UC_D_RIGHT_QUOT}},
    {'K',           CAPLOK,               {L'k',                  L'K',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'L',           CAPLOK | CAPLOKALTGR, {L'l',                  L'L',     WCH_NONE, UC_LOWER_L_STROKE,    UC_UPPER_L_STROKE}},
    {VK_OEM_1,      0x00,                 {UC_LOWER_U_RING_ABOVE, L'"',     WCH_NONE, L';',                 UC_ELLIPSIS}},
    {VK_OEM_7,      0x00,                 {UC_SECTION,            L'!',     WCH_NONE, L'\'',                WCH_DEAD}},
    {VK__none_,     0x00,                 {WCH_NONE,              WCH_NONE, WCH_NONE, WCH_NONE,             L'~'}},
    {VK_OEM_3,      0x00,                 {L'<',                  L'>',     WCH_NONE, UC_LESS_EQUAL,        UC_GREATER_EQUAL}},
    {VK_OEM_5,      0x00,                 {WCH_DEAD,              L'`',     WCH_NONE, WCH_DEAD,             WCH_DEAD}},
    {VK__none_,     0x00,                 {UC_DIAERESIS,          WCH_NONE, WCH_NONE, UC_DIAERESIS,         L'"'}},
    {'Y',           CAPLOK,               {L'y',                  L'Y',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'X',           CAPLOK,               {L'x',                  L'X',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'C',           CAPLOK,               {L'c',                  L'C',     WCH_NONE, WCH_NONE,             UC_COPYRIGHT}},
    {'V',           CAPLOK,               {L'v',                  L'V',     WCH_NONE, WCH_NONE,             UC_SQUARE_ROOT}},
    {'B',           CAPLOK,               {L'b',                  L'B',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {'N',           CAPLOK,               {L'n',                  L'N',     WCH_NONE, L',',                 UC_D9_QUOT}},
    {'M',           CAPLOK,               {L'm',                  L'M',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {VK_OEM_COMMA,  0x00,                 {L',',                  L'?',     WCH_NONE, L'<',                 UC_LESS_EQUAL}},
    {VK_OEM_PERIOD, 0x00,                 {L'.',                  L':',     WCH_NONE, L'>',                 UC_GREATER_EQUAL}},
    {VK_OEM_MINUS,  0x00,                 {L'-',                  L'_',     WCH_NONE, 0xFE63,               UC_EM_DASH}},
    {VK_SPACE,      0x00,                 {L' ',                  L' ',     L' ',     WCH_NONE,             WCH_NONE}},
    {VK_OEM_102,    0x00,                 {L'\\',                 L'|',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {VK_DECIMAL,    0x00,                 {L',',                  L',',     WCH_NONE, WCH_NONE,             WCH_NONE}},
    {0,             0,                    0,                      0,        0,        0,                    0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 2 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS2 vk_to_wchar2[] = {
    //                          Shift
    //                          -----
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
    //              Accent        Composed               Flags
    //              ------        --------               -----
    DEADTRANS(L'a', UC_DEGREE,    UC_LOWER_A_RING_ABOVE, 0x0000),
    DEADTRANS(L'u', UC_DEGREE,    UC_LOWER_U_RING_ABOVE, 0x0000),
    DEADTRANS(L'A', UC_DEGREE,    UC_UPPER_A_RING_ABOVE, 0x0000),
    DEADTRANS(L'U', UC_DEGREE,    UC_UPPER_U_RING_ABOVE, 0x0000),
    DEADTRANS(L' ', UC_DEGREE,    UC_DEGREE,             0x0000),
    DEADTRANS(L'r', L',',         UC_LOWER_R_CEDILLA,    0x0000),
    DEADTRANS(L'u', L',',         UC_LOWER_U_OGONEK,     0x0000),
    DEADTRANS(L'i', L',',         UC_LOWER_I_OGONEK,     0x0000),
    DEADTRANS(L'g', L',',         UC_LOWER_G_CEDILLA,    0x0000),
    DEADTRANS(L'k', L',',         UC_LOWER_K_CEDILLA,    0x0000),
    DEADTRANS(L'l', L',',         UC_LOWER_L_CEDILLA,    0x0000),
    DEADTRANS(L'n', L',',         UC_LOWER_N_CEDILLA,    0x0000),
    DEADTRANS(L'R', L',',         UC_UPPER_R_CEDILLA,    0x0000),
    DEADTRANS(L'U', L',',         UC_UPPER_U_OGONEK,     0x0000),
    DEADTRANS(L'I', L',',         UC_UPPER_I_OGONEK,     0x0000),
    DEADTRANS(L'G', L',',         UC_UPPER_G_CEDILLA,    0x0000),
    DEADTRANS(L'K', L',',         UC_UPPER_K_CEDILLA,    0x0000),
    DEADTRANS(L'L', L',',         UC_UPPER_L_CEDILLA,    0x0000),
    DEADTRANS(L'N', L',',         UC_UPPER_N_CEDILLA,    0x0000),
    DEADTRANS(L' ', L',',         L',',                  0x0000),
    DEADTRANS(L'n', L'\'',        UC_LOWER_N_ACUTE,      0x0000),
    DEADTRANS(L'c', L'\'',        UC_LOWER_C_ACUTE,      0x0000),
    DEADTRANS(L'y', L'\'',        UC_LOWER_Y_ACUTE,      0x0000),
    DEADTRANS(L'a', L'\'',        UC_LOWER_A_ACUTE,      0x0000),
    DEADTRANS(L's', L'\'',        UC_LOWER_S_ACUTE,      0x0000),
    DEADTRANS(L'l', L'\'',        UC_LOWER_L_ACUTE,      0x0000),
    DEADTRANS(L'e', L'\'',        UC_LOWER_E_ACUTE,      0x0000),
    DEADTRANS(L'r', L'\'',        UC_LOWER_R_ACUTE,      0x0000),
    DEADTRANS(L'u', L'\'',        UC_LOWER_U_ACUTE,      0x0000),
    DEADTRANS(L'i', L'\'',        UC_LOWER_I_ACUTE,      0x0000),
    DEADTRANS(L'z', L'\'',        UC_LOWER_Z_ACUTE,      0x0000),
    DEADTRANS(L'o', L'\'',        UC_LOWER_O_ACUTE,      0x0000),
    DEADTRANS(L'g', L'\'',        UC_LOWER_G_CEDILLA,    0x0000),
    DEADTRANS(L'N', L'\'',        UC_UPPER_N_ACUTE,      0x0000),
    DEADTRANS(L'C', L'\'',        UC_UPPER_C_ACUTE,      0x0000),
    DEADTRANS(L'Y', L'\'',        UC_UPPER_Y_ACUTE,      0x0000),
    DEADTRANS(L'A', L'\'',        UC_UPPER_A_ACUTE,      0x0000),
    DEADTRANS(L'S', L'\'',        UC_UPPER_S_ACUTE,      0x0000),
    DEADTRANS(L'L', L'\'',        UC_UPPER_L_ACUTE,      0x0000),
    DEADTRANS(L'E', L'\'',        UC_UPPER_E_ACUTE,      0x0000),
    DEADTRANS(L'R', L'\'',        UC_UPPER_R_ACUTE,      0x0000),
    DEADTRANS(L'U', L'\'',        UC_UPPER_U_ACUTE,      0x0000),
    DEADTRANS(L'I', L'\'',        UC_UPPER_I_ACUTE,      0x0000),
    DEADTRANS(L'Z', L'\'',        UC_UPPER_Z_ACUTE,      0x0000),
    DEADTRANS(L'O', L'\'',        UC_UPPER_O_ACUTE,      0x0000),
    DEADTRANS(L'G', L'\'',        UC_UPPER_G_CEDILLA,    0x0000),
    DEADTRANS(L' ', L'\'',        L'\'',                 0x0000),
    DEADTRANS(L'u', UC_CARON,     UC_LOWER_U_RING_ABOVE, 0x0000),
    DEADTRANS(L'o', UC_CARON,     UC_LOWER_O_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'n', UC_CARON,     UC_LOWER_N_CARON,      0x0000),
    DEADTRANS(L'c', UC_CARON,     UC_LOWER_C_CARON,      0x0000),
    DEADTRANS(L'd', UC_CARON,     UC_LOWER_D_CARON,      0x0000),
    DEADTRANS(L's', UC_CARON,     UC_LOWER_S_CARON,      0x0000),
    DEADTRANS(L'l', UC_CARON,     UC_LOWER_L_CARON,      0x0000),
    DEADTRANS(L'e', UC_CARON,     UC_LOWER_E_CARON,      0x0000),
    DEADTRANS(L'r', UC_CARON,     UC_LOWER_R_CARON,      0x0000),
    DEADTRANS(L't', UC_CARON,     UC_LOWER_T_CARON,      0x0000),
    DEADTRANS(L'z', UC_CARON,     UC_LOWER_Z_CARON,      0x0000),
    DEADTRANS(L'N', UC_CARON,     UC_UPPER_N_CARON,      0x0000),
    DEADTRANS(L'C', UC_CARON,     UC_UPPER_C_CARON,      0x0000),
    DEADTRANS(L'D', UC_CARON,     UC_UPPER_D_CARON,      0x0000),
    DEADTRANS(L'S', UC_CARON,     UC_UPPER_S_CARON,      0x0000),
    DEADTRANS(L'L', UC_CARON,     UC_UPPER_L_CARON,      0x0000),
    DEADTRANS(L'E', UC_CARON,     UC_UPPER_E_CARON,      0x0000),
    DEADTRANS(L'R', UC_CARON,     UC_UPPER_R_CARON,      0x0000),
    DEADTRANS(L'T', UC_CARON,     UC_UPPER_T_CARON,      0x0000),
    DEADTRANS(L'Z', UC_CARON,     UC_UPPER_Z_CARON,      0x0000),
    DEADTRANS(L' ', UC_CARON,     UC_CARON,              0x0000),
    DEADTRANS(L'a', L'^',         UC_LOWER_A_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'e', L'^',         UC_LOWER_E_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'u', L'^',         UC_LOWER_U_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'i', L'^',         UC_LOWER_I_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'o', L'^',         UC_LOWER_O_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'A', L'^',         UC_UPPER_A_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'E', L'^',         UC_UPPER_E_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'U', L'^',         UC_UPPER_U_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'I', L'^',         UC_UPPER_I_CIRCUMFLEX, 0x0000),
    DEADTRANS(L'O', L'^',         UC_UPPER_O_CIRCUMFLEX, 0x0000),
    DEADTRANS(L' ', L'^',         L'^',                  0x0000),
    DEADTRANS(L'e', L'-',         UC_LOWER_E_MACRON,     0x0000),
    DEADTRANS(L'u', L'-',         UC_LOWER_U_MACRON,     0x0000),
    DEADTRANS(L'i', L'-',         UC_LOWER_I_MACRON,     0x0000),
    DEADTRANS(L'o', L'-',         UC_LOWER_O_MACRON,     0x0000),
    DEADTRANS(L'a', L'-',         UC_LOWER_A_MACRON,     0x0000),
    DEADTRANS(L'E', L'-',         UC_UPPER_E_MACRON,     0x0000),
    DEADTRANS(L'U', L'-',         UC_UPPER_U_MACRON,     0x0000),
    DEADTRANS(L'I', L'-',         UC_UPPER_I_MACRON,     0x0000),
    DEADTRANS(L'O', L'-',         UC_UPPER_O_MACRON,     0x0000),
    DEADTRANS(L'A', L'-',         UC_UPPER_A_MACRON,     0x0000),
    DEADTRANS(L' ', L'-',         L'-',                  0x0000),
    DEADTRANS(L'o', L'~',         UC_LOWER_O_TILDE,      0x0000),
    DEADTRANS(L'O', L'~',         UC_UPPER_O_TILDE,      0x0000),
    DEADTRANS(L' ', L'~',         L'~',                  0x0000),
    DEADTRANS(L'y', UC_DIAERESIS, UC_LOWER_Y_DIAERESIS,  0x0000),
    DEADTRANS(L'a', UC_DIAERESIS, UC_LOWER_A_DIAERESIS,  0x0000),
    DEADTRANS(L'e', UC_DIAERESIS, UC_LOWER_E_DIAERESIS,  0x0000),
    DEADTRANS(L'u', UC_DIAERESIS, UC_LOWER_U_DIAERESIS,  0x0000),
    DEADTRANS(L'i', UC_DIAERESIS, UC_LOWER_I_DIAERESIS,  0x0000),
    DEADTRANS(L'o', UC_DIAERESIS, UC_LOWER_O_DIAERESIS,  0x0000),
    DEADTRANS(L'Y', UC_DIAERESIS, UC_UPPER_Y_DIAERESIS,  0x0000),
    DEADTRANS(L'A', UC_DIAERESIS, UC_UPPER_A_DIAERESIS,  0x0000),
    DEADTRANS(L'E', UC_DIAERESIS, UC_UPPER_E_DIAERESIS,  0x0000),
    DEADTRANS(L'U', UC_DIAERESIS, UC_UPPER_U_DIAERESIS,  0x0000),
    DEADTRANS(L'I', UC_DIAERESIS, UC_UPPER_I_DIAERESIS,  0x0000),
    DEADTRANS(L'O', UC_DIAERESIS, UC_UPPER_O_DIAERESIS,  0x0000),
    DEADTRANS(L' ', UC_DIAERESIS, UC_DIAERESIS,          0x0000),
    DEADTRANS(L'u', L'"',         UC_LOWER_U_DACUTE,     0x0000),
    DEADTRANS(L'o', L'"',         UC_LOWER_O_DACUTE,     0x0000),
    DEADTRANS(L'U', L'"',         UC_UPPER_O_DACUTE,     0x0000),
    DEADTRANS(L'O', L'"',         UC_UPPER_U_DACUTE,     0x0000),
    DEADTRANS(L' ', L'"',         L'"',                  0x0000),
    {0, 0, 0}
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
