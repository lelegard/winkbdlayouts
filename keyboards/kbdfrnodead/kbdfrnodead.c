//---------------------------------------------------------------------------
// French Keyboard Layout without dead keys (WKL)
// Automatically generated from kbdfr.dll and then manually modified.
// Using dead keys for '~', '^', '`' is a pain for developers.
// We remove all dead keys and we remap some accentuated characters
// with AltGr or Shift-AltGr on some letters (see vk_to_wchar6).
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
    {0x01, L"ECHAP"},
    {0x0E, L"RET.ARR"},
    {0x0F, L"TAB"},
    {0x1C, L"ENTREE"},
    {0x1D, L"CTRL"},
    {0x2A, L"MAJ"},
    {0x36, L"MAJ DROITE"},
    {0x37, L"* (PAVE NUM.)"},
    {0x38, L"ALT"},
    {0x39, L"ESPACE"},
    {0x3A, L"VERR.MAJ"},
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
    {0x46, L"DEFIL"},
    {0x47, L"7 (PAVE NUM.)"},
    {0x48, L"8 (PAVE NUM.)"},
    {0x49, L"9 (PAVE NUM.)"},
    {0x4A, L"- (PAVE NUM.)"},
    {0x4B, L"4 (PAVE NUM.)"},
    {0x4C, L"5 (PAVE NUM.)"},
    {0x4D, L"6 (PAVE NUM.)"},
    {0x4E, L"+ (PAVE NUM.)"},
    {0x4F, L"1 (PAVE NUM.)"},
    {0x50, L"2 (PAVE NUM.)"},
    {0x51, L"3 (PAVE NUM.)"},
    {0x52, L"0 (PAVE NUM.)"},
    {0x53, L". (PAVE NUM.)"},
    {0x57, L"F11"},
    {0x58, L"F12"},
    {0x00, NULL}
};

//---------------------------------------------------------------------------
// Scan codes to key names (extended keypad)
//---------------------------------------------------------------------------

static VSC_LPWSTR key_names_ext[] = {
    {0x1C, L"ENTREE (PAVE NUM.)"},
    {0x1D, L"CTRL DROITE"},
    {0x35, L"/ (PAVE NUM.)"},
    {0x37, L"Impr.Ecran"},
    {0x38, L"ALT DROITE"},
    {0x45, L"Ver.Num"},
    {0x46, L"ATTN"},
    {0x47, L"ORIGINE"},
    {0x48, L"HAUT"},
    {0x49, L"PG.PREC"},
    {0x4B, L"GAUCHE"},
    {0x4D, L"DROITE"},
    {0x4F, L"FIN"},
    {0x50, L"BAS"},
    {0x51, L"PG.SUIV"},
    {0x52, L"INS"},
    {0x53, L"SUPPR"},
    {0x54, L"<00>"},
    {0x56, L"AIDE"},
    {0x5B, L"WINDOWS GAUCHE"},
    {0x5C, L"WINDOWS DROITE"},
    {0x5D, L"APPLICATION"},
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
    /* 0C */ VK_OEM_4,
    /* 0D */ VK_OEM_PLUS,
    /* 0E */ VK_BACK,
    /* 0F */ VK_TAB,
    /* 10 */ 'A',
    /* 11 */ 'Z',
    /* 12 */ 'E',
    /* 13 */ 'R',
    /* 14 */ 'T',
    /* 15 */ 'Y',
    /* 16 */ 'U',
    /* 17 */ 'I',
    /* 18 */ 'O',
    /* 19 */ 'P',
    /* 1A */ VK_OEM_6,
    /* 1B */ VK_OEM_1,
    /* 1C */ VK_RETURN,
    /* 1D */ VK_LCONTROL,
    /* 1E */ 'Q',
    /* 1F */ 'S',
    /* 20 */ 'D',
    /* 21 */ 'F',
    /* 22 */ 'G',
    /* 23 */ 'H',
    /* 24 */ 'J',
    /* 25 */ 'K',
    /* 26 */ 'L',
    /* 27 */ 'M',
    /* 28 */ VK_OEM_3,
    /* 29 */ VK_OEM_7,
    /* 2A */ VK_LSHIFT,
    /* 2B */ VK_OEM_5,
    /* 2C */ 'W',
    /* 2D */ 'X',
    /* 2E */ 'C',
    /* 2F */ 'V',
    /* 30 */ 'B',
    /* 31 */ 'N',
    /* 32 */ VK_OEM_COMMA,
    /* 33 */ VK_OEM_PERIOD,
    /* 34 */ VK_OEM_2,
    /* 35 */ VK_OEM_8,
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
    /* 56 */ VK_OEM_102,
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
        4,            // 011 = Shift Control
        SHFT_INVALID, // 100 = Alt
        SHFT_INVALID, // 101 = Shift Alt
        3,            // 110 = Control Alt (AltGr)
        5,            // 111 = Shift Control Alt (Shift AltGr)
    }
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 3 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS3 vk_to_wchar3[] = {
    //                            Shift         Ctrl
    //                            -----         ----
    {VK_OEM_6,   CAPLOK, {L'^',   UC_DIAERESIS, UC_ESC}},
    {VK_BACK,    0x00,   {UC_BS,  UC_BS,        UC_DEL}},
    {VK_ESCAPE,  0x00,   {UC_ESC, UC_ESC,       UC_ESC}},
    {VK_RETURN,  0x00,   {L'\r',  L'\r',        L'\n'}},
    {VK_SPACE,   0x00,   {L' ',   L' ',         L' '}},
    {VK_CANCEL,  0x00,   {UC_ETX, UC_ETX,       UC_ETX}},
    {0,          0,      0,       0,            0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 4 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS4 vk_to_wchar4[] = {
    //                                       Shift      Ctrl      AltGr
    //                                       -----      ----      -----
    {'T',         CAPLOK, {L't',             L'T',      WCH_NONE, WCH_LGTR}},
    {VK_OEM_1,    CAPLOK, {L'$',             UC_POUND,  UC_GS,    UC_CURRENCY}},
    {0,           0,      0,                 0,         0,        0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 5 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS5 vk_to_wchar5[] = {
    //                   Shift Ctrl      AltGr  Shift/Ctrl
    //                   ----- ----      -----  ----------
    {'5', CAPLOK, {L'(', L'5', WCH_NONE, L'[',  UC_ESC}},
    {'6', CAPLOK, {L'-', L'6', WCH_NONE, L'|',  UC_US}},
    {'8', CAPLOK, {L'_', L'8', WCH_NONE, L'\\', UC_FS}},
    {0,   0,      0,     0,    0,        0,     0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 6 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS6 vk_to_wchar6[] = {
    //                                          Shift       Ctrl      AltGr                  Shift/Ctrl Shift/AltGr
    //                                           -----       ----      -----                  ---------- -----------
    {'1',           CAPLOK, {L'&',               L'1',       WCH_NONE, UC_LOWER_ALPHA,        WCH_NONE,  UC_LOWER_BETA}},
    {'2',           CAPLOK, {UC_LOWER_E_ACUTE,   L'2',       WCH_NONE, L'~',                  WCH_NONE,  UC_UPPER_E_ACUTE}},
    {'3',           CAPLOK, {L'"',               L'3',       WCH_NONE, L'#',                  WCH_NONE,  UC_DA_LEFT_QUOT}},
    {'4',           CAPLOK, {L'\'',              L'4',       WCH_NONE, L'{',                  WCH_NONE,  UC_DA_RIGHT_QUOT}},
    {'7',           CAPLOK, {UC_LOWER_E_GRAVE,   L'7',       WCH_NONE, L'`',                  WCH_NONE,  UC_UPPER_E_GRAVE}},
    {'9',           CAPLOK, {UC_LOWER_C_CEDILLA, L'9',       WCH_NONE, L'^',                  UC_RS,     UC_UPPER_C_CEDILLA}},
    {'0',           CAPLOK, {UC_LOWER_A_GRAVE,   L'0',       0x0000,   L'@',                  WCH_NONE,  UC_UPPER_A_GRAVE}},
    {VK_OEM_4,      CAPLOK, {L')',               UC_DEGREE,  WCH_NONE, L']',                  WCH_NONE,  UC_LOWER_PI}},
    {VK_OEM_PLUS,   CAPLOK, {L'=',               L'+',       WCH_NONE, L'}',                  WCH_NONE,  UC_ALMOST_EQUAL}},
    {'A',           CAPLOK, {L'a',               L'A',       WCH_NONE, UC_LOWER_A_CIRCUMFLEX, WCH_NONE,  UC_UPPER_A_CIRCUMFLEX}},
    {'Z',           CAPLOK, {L'z',               L'Z',       WCH_NONE, UC_LOWER_AE,           WCH_NONE,  UC_UPPER_AE}},
    {'E',           CAPLOK, {L'e',               L'E',       WCH_NONE, UC_EURO,               WCH_NONE,  UC_YEN}},
    {'R',           CAPLOK, {L'r',               L'R',       WCH_NONE, UC_LOWER_E_CIRCUMFLEX, WCH_NONE,  UC_UPPER_E_CIRCUMFLEX}},
    {'U',           CAPLOK, {L'u',               L'U',       WCH_NONE, UC_LOWER_U_CIRCUMFLEX, WCH_NONE,  UC_UPPER_U_CIRCUMFLEX}},
    {'I',           CAPLOK, {L'i',               L'I',       WCH_NONE, UC_LOWER_I_CIRCUMFLEX, WCH_NONE,  UC_UPPER_I_CIRCUMFLEX}},
    {'O',           CAPLOK, {L'o',               L'O',       WCH_NONE, UC_LOWER_O_CIRCUMFLEX, WCH_NONE,  UC_UPPER_O_CIRCUMFLEX}},
    {'P',           CAPLOK, {L'p',               L'P',       WCH_NONE, UC_LOWER_O_DIAERESIS,  WCH_NONE,  UC_UPPER_O_DIAERESIS}},
    {'D',           CAPLOK, {L'd',               L'D',       WCH_NONE, UC_LOWER_E_DIAERESIS,  WCH_NONE,  UC_UPPER_E_DIAERESIS}},
    {'J',           CAPLOK, {L'j',               L'J',       WCH_NONE, UC_LOWER_U_DIAERESIS,  WCH_NONE,  UC_UPPER_U_DIAERESIS}},
    {'L',           CAPLOK, {L'l',               L'L',       WCH_NONE, UC_LOWER_OE,           WCH_NONE,  UC_UPPER_OE}},
    {VK_OEM_3,      CAPLOK, {UC_LOWER_U_GRAVE,   L'%',       WCH_NONE, UC_PER_MILLE,          WCH_NONE,  UC_UPPER_U_GRAVE}},
    {VK_OEM_5,      CAPLOK, {L'*',               UC_MICRO,   UC_FS,    UC_EM_DASH,            WCH_NONE,  UC_EN_DASH}},
    {VK_OEM_102,    0x00,   {L'<',               L'>',       UC_FS,    UC_LESS_EQUAL,         WCH_NONE,  UC_GREATER_EQUAL}},
    {'W',           CAPLOK, {L'w',               L'W',       WCH_NONE, UC_LOWER_PHI,          WCH_NONE,  UC_UPPER_PHI}},
    {'X',           CAPLOK, {L'x',               L'X',       WCH_NONE, UC_LOWER_PSI,          WCH_NONE,  UC_UPPER_PSI}},
    {'B',           CAPLOK, {L'b',               L'B',       WCH_NONE, UC_LOWER_SHARP_S,      WCH_NONE,  WCH_NONE}},
    {'N',           CAPLOK, {L'n',               L'N',       WCH_NONE, UC_LOWER_N_TILDE,      WCH_NONE,  UC_UPPER_N_TILDE}},
    {VK_OEM_PERIOD, CAPLOK, {L';',               L'.',       WCH_NONE, UC_ELLIPSIS,           WCH_NONE,  UC_BULLET}},
    {VK_OEM_8,      CAPLOK, {L'!',               UC_SECTION, WCH_NONE, UC_NOT_EQUAL,          WCH_NONE,  UC_PLUS_MINUS}},
    {0,             0,      0,                   0,          0,        0,                     0,         0}
};

//---------------------------------------------------------------------------
// Virtual Key to WCHAR translations for 2 shift states
//---------------------------------------------------------------------------

static VK_TO_WCHARS2 vk_to_wchar2[] = {
    //                                         Shift
    //                                         -----
    {VK_OEM_7,      0x00,   {UC_SUP2,          WCH_NONE}},
    {'Y',           CAPLOK, {L'y',             L'Y'}},
    {'Q',           CAPLOK, {L'q',             L'Q'}},
    {'S',           CAPLOK, {L's',             L'S'}},
    {'F',           CAPLOK, {L'f',             L'F'}},
    {'G',           CAPLOK, {L'g',             L'G'}},
    {'H',           CAPLOK, {L'h',             L'H'}},
    {'K',           CAPLOK, {L'k',             L'K'}},
    {'M',           CAPLOK, {L'm',             L'M'}},
    {'C',           CAPLOK, {L'c',             L'C'}},
    {'V',           CAPLOK, {L'v',             L'V'}},
    {VK_OEM_COMMA,  CAPLOK, {L',',             L'?'}},
    {VK_OEM_2,      CAPLOK, {L':',             L'/'}},
    {VK_DECIMAL,    0x00,   {L'.',             L'.'}},
    {VK_TAB,        0x00,   {L'\t',            L'\t'}},
    {VK_ADD,        0x00,   {L'+',             L'+'}},
    {VK_DIVIDE,     0x00,   {L'/',             L'/'}},
    {VK_MULTIPLY,   0x00,   {L'*',             L'*'}},
    {VK_SUBTRACT,   0x00,   {L'-',             L'-'}},
    {0,             0,      0,                 0}
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
    {(PVK_TO_WCHARS1)vk_to_wchar4, 4, sizeof(vk_to_wchar4[0])},
    {(PVK_TO_WCHARS1)vk_to_wchar5, 5, sizeof(vk_to_wchar5[0])},
    {(PVK_TO_WCHARS1)vk_to_wchar6, 6, sizeof(vk_to_wchar6[0])},
    {(PVK_TO_WCHARS1)vk_to_wchar2, 2, sizeof(vk_to_wchar2[0])},
    {(PVK_TO_WCHARS1)vk_to_wchar1, 1, sizeof(vk_to_wchar1[0])},
    {NULL,                         0, 0}
};

//---------------------------------------------------------------------------
// Ligatures to WCHAR translations
//---------------------------------------------------------------------------

static LIGATURE4 ligatures[] = {
    {'T', 3, {L't', L'e', L's', L't'}}, // AltGr
    {0,   0, {0,    0,    0,    0}}
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
    .fLocaleFlags    = MAKELONG(KLLF_ALTGR, KBD_VERSION),
    .nLgMax          = 4,
    .cbLgEntry       = sizeof(ligatures[0]),
    .pLigature       = (PLIGATURE1)ligatures,
    .dwType          = 0,
    .dwSubType       = 0,
};

//---------------------------------------------------------------------------
// Keyboard layout entry point
//---------------------------------------------------------------------------

__declspec(dllexport) PKBDTABLES KbdLayerDescriptor(void)
{
    return &kbd_tables;
}
