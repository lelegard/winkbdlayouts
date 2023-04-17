//---------------------------------------------------------------------------
// Arabic Apple VM Keyboard Layout (WKL)
// "Apple VM" keyboards are derived from the base keyboard and swaps the
// keys @/# and </>, scan codes 0x29 and 0x56. This is required for Windows
// running on some virtual machines.
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
        1,            // 001 = Shift
        2,            // 010 = Control
        SHFT_INVALID, // 011 = Shift Control
        SHFT_INVALID, // 100 = Alt
        SHFT_INVALID, // 101 = Shift Alt
        3,            // 110 = Control Alt (AltGr)
        4,            // 111 = Shift Control Alt
    }
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 3 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS3 vk_to_wchar3[] = {
    //                         Shift   Ctrl
    //                         =====   ====
    {VK_BACK,   0x00, {0x0008, 0x0008, 0x007F}}, // BS, BS, DEL
    {VK_ESCAPE, 0x00, {0x001B, 0x001B, 0x001B}}, // ESC, ESC, ESC
    {VK_RETURN, 0x00, {L'\r',  L'\r',  L'\n'}},
    {VK_CANCEL, 0x00, {0x0003, 0x0003, 0x0003}},
    {0,         0,    0,       0,      0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 5 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS5 vk_to_wchar5[] = {
    //                               Shift     Ctrl      Ctrl/Alt  Shift/Ctrl/Alt
    //                               =====     ====      ========  ==============
    {'1',           SGCAPS, {0x0661, L'!',     0x0661,   0x0638,   0x0638}},
    {'1',           0x00,   {L'1',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'2',           SGCAPS, {0x0662, L'@',     0x0662,   0x0637,   0x274A}},
    {'2',           0x00,   {L'2',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'3',           SGCAPS, {0x0663, L'#',     0x0663,   0x0630,   0x00A3}},      // Pound
    {'3',           0x00,   {L'3',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'4',           SGCAPS, {0x0664, L'$',     0x0664,   0x062F,   0x20AC}},
    {'4',           0x00,   {L'4',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'5',           SGCAPS, {0x0665, 0x066A,   0x0665,   0x221E,   WCH_NONE}},
    {'5',           0x00,   {L'5',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'6',           SGCAPS, {0x0666, L'^',     0x0666,   0x0671,   WCH_NONE}},
    {'6',           0x00,   {L'6',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'7',           SGCAPS, {0x0667, L'&',     0x0667,   WCH_NONE, WCH_NONE}},
    {'7',           0x00,   {L'7',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'8',           SGCAPS, {0x0668, L'*',     0x0668,   WCH_NONE, WCH_NONE}},
    {'8',           0x00,   {L'8',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'9',           SGCAPS, {0x0669, L')',     0x0669,   WCH_NONE, WCH_NONE}},
    {'9',           0x00,   {L'9',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'0',           SGCAPS, {0x0660, L'(',     0x0660,   0x00B0,   WCH_NONE}},    // Degree
    {'0',           0x00,   {L'0',   0x0000,   0x0000,   0x0000,   0x0000}},
    {VK_OEM_MINUS,  SGCAPS, {L'-',   0x0640,   WCH_NONE, L'_',     L'_'}},
    {VK_OEM_MINUS,  0x00,   {L'-',   0x0000,   0x0000,   0x0000,   0x0000}},
    {VK_OEM_PLUS,   SGCAPS, {L'=',   L'+',     L'=',     WCH_NONE, WCH_NONE}},
    {VK_OEM_PLUS,   0x00,   {L'=',   0x0000,   0x0000,   0x0000,   0x0000}},
    {'Q',           0x00,   {0x0636, 0x064E,   WCH_NONE, 0x2018,   WCH_NONE}},
    {'W',           0x00,   {0x0635, 0x064B,   WCH_NONE, 0x2018,   WCH_NONE}},
    {'E',           0x00,   {0x062B, 0x0650,   WCH_NONE, 0x201C,   WCH_NONE}},
    {'R',           0x00,   {0x0642, 0x064D,   WCH_NONE, 0x201C,   0x0609}},
    {'T',           0x00,   {0x0641, 0x064F,   WCH_NONE, 0x06A4,   0x06A4}},
    {'Y',           0x00,   {0x063A, 0x064C,   WCH_NONE, WCH_NONE, WCH_NONE}},
    {'U',           0x00,   {0x0639, 0x0652,   WCH_NONE, 0x06D5,   0x06D5}},
    {'I',           0x00,   {0x0647, 0x0651,   WCH_NONE, WCH_NONE, WCH_NONE}},
    {'O',           0x00,   {0x062E, L']',     WCH_NONE, WCH_NONE, WCH_NONE}},
    {'P',           0x00,   {0x062D, L'[',     WCH_NONE, WCH_NONE, WCH_NONE}},
    {VK_OEM_4,      0x00,   {0x062C, L'}',     0x001B,   0x0686,   0x0686}},      // ESC
    {VK_OEM_6,      0x00,   {0x0629, L'{',     0x001D,   WCH_NONE, WCH_NONE}},
    {'A',           0x00,   {0x0634, 0x00AB,   WCH_NONE, WCH_NONE, WCH_NONE}},    // <<
    {'S',           0x00,   {0x0633, 0x00BB,   WCH_NONE, 0x06D2,   0x06D2}},      // >>
    {'D',           0x00,   {0x064A, 0x0649,   WCH_NONE, 0x06CC,   0x06CC}},
    {'F',           0x00,   {0x0628, WCH_NONE, WCH_NONE, 0x067E,   0x067E}},
    {'G',           0x00,   {0x0644, WCH_NONE, WCH_NONE, 0x0653,   WCH_NONE}},
    {'H',           0x00,   {0x0627, 0x0622,   WCH_NONE, 0x0670,   WCH_NONE}},
    {'J',           0x00,   {0x062A, WCH_NONE, WCH_NONE, 0x0679,   0x0679}},
    {'K',           0x00,   {0x0646, 0x066B,   WCH_NONE, 0x06BA,   0x06BA}},
    {'L',           0x00,   {0x0645, 0x066C,   WCH_NONE, WCH_NONE, WCH_NONE}},
    {VK_OEM_1,      0x00,   {0x0643, L':',     L';',     0x06AF,   0x06A9}},
    {VK_OEM_7,      0x00,   {0x061B, L'"',     L'\'',    0x2026,   0x2026}},
    {VK_OEM_3,      SGCAPS, {0x00A7, 0x00B1,   L'0',     0x0003,   0x0003}},      // Section, +/-
    {VK_OEM_3,      0x00,   {0x0003, 0x0000,   0x0000,   0x0000,   0x0000}},
    {VK_OEM_5,      0x00,   {L'\\',  L'|',     0x001C,   WCH_NONE, WCH_NONE}},
    {'Z',           0x00,   {0x0638, L'\'',    WCH_NONE, WCH_NONE, WCH_NONE}},
    {'X',           0x00,   {0x0637, WCH_NONE, WCH_NONE, WCH_NONE, WCH_NONE}},
    {'C',           0x00,   {0x0630, 0x0626,   WCH_NONE, 0x0688,   0x0688}},
    {'V',           0x00,   {0x062F, 0x0621,   WCH_NONE, 0x0691,   0x0691}},
    {'B',           0x00,   {0x0632, 0x0623,   WCH_NONE, 0x0698,   0x0698}},
    {'N',           0x00,   {0x0631, 0x0625,   WCH_NONE, WCH_NONE, WCH_NONE}},
    {'M',           0x00,   {0x0648, 0x0624,   WCH_NONE, WCH_NONE, WCH_NONE}},
    {VK_OEM_COMMA,  0x00,   {0x060C, L'>',     L',',     L',',     L','}},
    {VK_OEM_PERIOD, 0x00,   {L'.',   L'<',     L'.',     WCH_NONE, WCH_NONE}},
    {VK_OEM_2,      0x00,   {L'/',   0x061F,   L'/',     0x00F7,   0x00F7}},      // Division, Division
    {VK_SPACE,      0x00,   {L' ',   L' ',     WCH_NONE, 0x00A0,   0x00A0}},      // Nbrk space, Nbrk space
    {VK_OEM_102,    0x00,   {0x0640, WCH_NONE, L'`',     WCH_NONE, WCH_NONE}},
    {VK_DECIMAL,    0x00,   {L'.',   L'.',     WCH_NONE, WCH_NONE, WCH_NONE}},
    {0,             0,      0,       0,        0,        0,        0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 2 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS2 vk_to_wchar2[] = {
    //                          Shift
    //                          =====
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
    /* 28 */ VK_OEM_7,
    /* 29 */ VK_OEM_3,
    /* 2A */ VK_LSHIFT,
    /* 2B */ VK_OEM_5,
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
    /* 56 */ VK_OEM_7, // "Apple VM" keyboard
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
    .pDeadKey        = NULL,
    .pKeyNames       = key_names,
    .pKeyNamesExt    = key_names_ext,
    .pKeyNamesDead   = NULL,
    .pusVSCtoVK      = scancode_to_vk,
    .bMaxVSCtoVK     = ARRAYSIZE(scancode_to_vk),
    .pVSCtoVK_E0     = scancode_to_vk_e0,
    .pVSCtoVK_E1     = scancode_to_vk_e1,
    .fLocaleFlags    = MAKELONG(KLLF_ALTGR | KLLF_LRM_RLM, KBD_VERSION),
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
