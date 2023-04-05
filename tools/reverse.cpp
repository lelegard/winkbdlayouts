//---------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Utility to analyze an installed keyboard layout DLL.
// Generate a C source file for this keyboard.
//
//---------------------------------------------------------------------------

#include "options.h"
#include "strutils.h"

// Entry point of all keyboard layout DLL's.
#define KBD_DLL_ENTRY_NAME "KbdLayerDescriptor"

// Tables of values => symbols
typedef __int64 Value;
typedef std::map<Value, std::string> SymbolTable;
#define SYM(e) {e, #e}


//----------------------------------------------------------------------------
// Command line options.
//----------------------------------------------------------------------------

class ReverseOptions : public Options
{
public:
    // Constructor.
    ReverseOptions(int argc, char* argv[]);

    // Command line options.
    std::string dashed;
    std::string input;
    std::string output;
    std::string comment;
    int         kbd_type;
    bool        num_only;
};

ReverseOptions::ReverseOptions(int argc, char* argv[]) :
    Options(argc, argv,
        "[options] kbd-name-or-file\n"
        "\n"
        "  kbd-name-or-file : Either the file name of a keyboard layout DLL or the\n"
        "  name of a keyboard layout, for instance \"fr\" for C:\\Windows\\System32\\kbdfr.dll\n"
        "\n"
        "Options:\n"
        "\n"
        "  -c \"string\" : comment string in the header\n"
        "  -h : display this help text\n"
        "  -n : numerical output only, do not attempt to translate to source macros\n"
        "  -o file : output file name, default is standard output\n"
        "  -t value : keyboard type, defaults to dwType in kbd table or 4 if unspecified"),
    dashed(75, '-'),
    input(),
    output(),
    comment("Windows Keyboards Layouts (WKL)"),
    kbd_type(0),
    num_only(false)
{
    // Parse arguments.
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--help" || args[i] == "-h") {
            usage();
        }
        else if (args[i] == "-n") {
            num_only = true;
        }
        else if (args[i] == "-o" && i + 1 < args.size()) {
            output = args[++i];
        }
        else if (args[i] == "-c" && i + 1 < args.size()) {
            comment = args[++i];
        }
        else if (args[i] == "-t" && i + 1 < args.size()) {
            kbd_type = std::atoi(argv[++i]);
        }
        else if (!args[i].empty() && args[i].front() != '-' && input.empty()) {
            input = args[i];
        }
        else {
            fatal("invalid option '" + args[i] + "', try --help");
        }
    }
    if (input.empty()) {
        fatal("no keyboard layout specified, try --help");
    }
}


//---------------------------------------------------------------------------
// Format an integer as a decimal or hexadecimal string.
// If hex_digits is zero, format in decimal.
//---------------------------------------------------------------------------

std::string Integer(Value value, int hex_digits = 0)
{
    return hex_digits <= 0 ? Format("%lld", value) : Format("0x%0*llX", hex_digits, value);
}


//---------------------------------------------------------------------------
// Format an integer as a string, using a table of symbols.
// If no symbol found or option -n, return a number.
// If hex_digits is zero, format in decimal.
//---------------------------------------------------------------------------

std::string Symbol(const ReverseOptions& opt, const SymbolTable& symbols, Value value, int hex_digits = 0)
{
    if (!opt.num_only) {
        const auto it = symbols.find(value);
        if (it != symbols.end()) {
            return it->second;
        }
    }
    return Integer(value, hex_digits);
}


//---------------------------------------------------------------------------
// Format a bit mask of symbols, same principle as Symbol().
//---------------------------------------------------------------------------

std::string BitMask(const ReverseOptions& opt, const SymbolTable& symbols, Value value, int hex_digits = 0)
{
    if (!opt.num_only) {
        std::string str;
        Value bits = 0;
        for (const auto& sym : symbols) {
            if (sym.first == 0 && value == 0) {
                // Specific symbol for zero (no flag)
                return sym.second;
            }
            if (sym.first != 0 && (value & sym.first) == sym.first) {
                // Found one flag.
                if (!str.empty()) {
                    str += " | ";
                }
                str += sym.second;
                bits |= sym.first;
            }
        }
        if (bits != 0) {
            // Found at least some bits, add remaining bits.
            if ((value & ~bits) != 0) {
                if (!str.empty()) {
                    str += " | ";
                }
                str += Format("0x%0*lld", hex_digits, value & ~bits);
            }
            return str;
        }
    }
    return Integer(value, hex_digits);
}


//---------------------------------------------------------------------------
// Format a symbol and a bit mask of attributes, same principle as Symbol().
//---------------------------------------------------------------------------

std::string Attributes(const ReverseOptions& opt, const SymbolTable& symbols, const SymbolTable& attributes, Value value, int hex_digits = 0)
{
    if (!opt.num_only) {
        // Compute mask of all possible attributes.
        Value all_attributes = 0;
        for (const auto& sym : attributes) {
            all_attributes |= sym.first;
        }
        // Base value.
        std::string str(Symbol(opt, symbols, value & ~all_attributes, hex_digits));
        // Add attributes.
        if ((value & all_attributes) != 0) {
            str += " | " + BitMask(opt, attributes, value & all_attributes, hex_digits);
        }
        return str;
    }
    return Integer(value, hex_digits);
}


//---------------------------------------------------------------------------
// Common symbol tables.
//---------------------------------------------------------------------------

const SymbolTable shift_state_symbols{
    SYM(KBDBASE),
    SYM(KBDSHIFT),
    SYM(KBDCTRL),
    SYM(KBDALT),
    SYM(KBDKANA),
    SYM(KBDROYA),
    SYM(KBDLOYA),
    SYM(KBDGRPSELTAP)
};

const SymbolTable vk_symbols {
    SYM(VK_LBUTTON),
    SYM(VK_RBUTTON),
    SYM(VK_CANCEL),
    SYM(VK_MBUTTON),
    SYM(VK_XBUTTON1),
    SYM(VK_XBUTTON2),
    SYM(VK_BACK),
    SYM(VK_TAB),
    SYM(VK_CLEAR),
    SYM(VK_RETURN),
    SYM(VK_SHIFT),
    SYM(VK_CONTROL),
    SYM(VK_MENU),
    SYM(VK_PAUSE),
    SYM(VK_CAPITAL),
    SYM(VK_KANA),
    SYM(VK_IME_ON),
    SYM(VK_JUNJA),
    SYM(VK_FINAL),
    SYM(VK_HANJA),
    SYM(VK_KANJI),
    SYM(VK_IME_OFF),
    SYM(VK_ESCAPE),
    SYM(VK_CONVERT),
    SYM(VK_NONCONVERT),
    SYM(VK_ACCEPT),
    SYM(VK_MODECHANGE),
    SYM(VK_SPACE),
    SYM(VK_PRIOR),
    SYM(VK_NEXT),
    SYM(VK_END),
    SYM(VK_HOME),
    SYM(VK_LEFT),
    SYM(VK_UP),
    SYM(VK_RIGHT),
    SYM(VK_DOWN),
    SYM(VK_SELECT),
    SYM(VK_PRINT),
    SYM(VK_EXECUTE),
    SYM(VK_SNAPSHOT),
    SYM(VK_INSERT),
    SYM(VK_DELETE),
    SYM(VK_HELP),
    SYM('0'),
    SYM('1'),
    SYM('2'),
    SYM('3'),
    SYM('4'),
    SYM('5'),
    SYM('6'),
    SYM('7'),
    SYM('8'),
    SYM('9'),
    SYM('A'),
    SYM('B'),
    SYM('C'),
    SYM('D'),
    SYM('E'),
    SYM('F'),
    SYM('G'),
    SYM('H'),
    SYM('I'),
    SYM('J'),
    SYM('K'),
    SYM('L'),
    SYM('M'),
    SYM('N'),
    SYM('O'),
    SYM('P'),
    SYM('Q'),
    SYM('R'),
    SYM('S'),
    SYM('T'),
    SYM('U'),
    SYM('V'),
    SYM('W'),
    SYM('X'),
    SYM('Y'),
    SYM('Z'),
    SYM(VK_LWIN),
    SYM(VK_RWIN),
    SYM(VK_APPS),
    SYM(VK_SLEEP),
    SYM(VK_NUMPAD0),
    SYM(VK_NUMPAD1),
    SYM(VK_NUMPAD2),
    SYM(VK_NUMPAD3),
    SYM(VK_NUMPAD4),
    SYM(VK_NUMPAD5),
    SYM(VK_NUMPAD6),
    SYM(VK_NUMPAD7),
    SYM(VK_NUMPAD8),
    SYM(VK_NUMPAD9),
    SYM(VK_MULTIPLY),
    SYM(VK_ADD),
    SYM(VK_SEPARATOR),
    SYM(VK_SUBTRACT),
    SYM(VK_DECIMAL),
    SYM(VK_DIVIDE),
    SYM(VK_F1),
    SYM(VK_F2),
    SYM(VK_F3),
    SYM(VK_F4),
    SYM(VK_F5),
    SYM(VK_F6),
    SYM(VK_F7),
    SYM(VK_F8),
    SYM(VK_F9),
    SYM(VK_F10),
    SYM(VK_F11),
    SYM(VK_F12),
    SYM(VK_F13),
    SYM(VK_F14),
    SYM(VK_F15),
    SYM(VK_F16),
    SYM(VK_F17),
    SYM(VK_F18),
    SYM(VK_F19),
    SYM(VK_F20),
    SYM(VK_F21),
    SYM(VK_F22),
    SYM(VK_F23),
    SYM(VK_F24),
    SYM(VK_NAVIGATION_VIEW),
    SYM(VK_NAVIGATION_MENU),
    SYM(VK_NAVIGATION_UP),
    SYM(VK_NAVIGATION_DOWN),
    SYM(VK_NAVIGATION_LEFT),
    SYM(VK_NAVIGATION_RIGHT),
    SYM(VK_NAVIGATION_ACCEPT),
    SYM(VK_NAVIGATION_CANCEL),
    SYM(VK_NUMLOCK),
    SYM(VK_SCROLL),
    SYM(VK_OEM_NEC_EQUAL),
    SYM(VK_OEM_FJ_JISHO),
    SYM(VK_OEM_FJ_MASSHOU),
    SYM(VK_OEM_FJ_TOUROKU),
    SYM(VK_OEM_FJ_LOYA),
    SYM(VK_OEM_FJ_ROYA),
    SYM(VK_LSHIFT),
    SYM(VK_RSHIFT),
    SYM(VK_LCONTROL),
    SYM(VK_RCONTROL),
    SYM(VK_LMENU),
    SYM(VK_RMENU),
    SYM(VK_BROWSER_BACK),
    SYM(VK_BROWSER_FORWARD),
    SYM(VK_BROWSER_REFRESH),
    SYM(VK_BROWSER_STOP),
    SYM(VK_BROWSER_SEARCH),
    SYM(VK_BROWSER_FAVORITES),
    SYM(VK_BROWSER_HOME),
    SYM(VK_VOLUME_MUTE),
    SYM(VK_VOLUME_DOWN),
    SYM(VK_VOLUME_UP),
    SYM(VK_MEDIA_NEXT_TRACK),
    SYM(VK_MEDIA_PREV_TRACK),
    SYM(VK_MEDIA_STOP),
    SYM(VK_MEDIA_PLAY_PAUSE),
    SYM(VK_LAUNCH_MAIL),
    SYM(VK_LAUNCH_MEDIA_SELECT),
    SYM(VK_LAUNCH_APP1),
    SYM(VK_LAUNCH_APP2),
    SYM(VK_OEM_1),
    SYM(VK_OEM_PLUS),
    SYM(VK_OEM_COMMA),
    SYM(VK_OEM_MINUS),
    SYM(VK_OEM_PERIOD),
    SYM(VK_OEM_2),
    SYM(VK_OEM_3),
    SYM(VK_GAMEPAD_A),
    SYM(VK_GAMEPAD_B),
    SYM(VK_GAMEPAD_X),
    SYM(VK_GAMEPAD_Y),
    SYM(VK_GAMEPAD_RIGHT_SHOULDER),
    SYM(VK_GAMEPAD_LEFT_SHOULDER),
    SYM(VK_GAMEPAD_LEFT_TRIGGER),
    SYM(VK_GAMEPAD_RIGHT_TRIGGER),
    SYM(VK_GAMEPAD_DPAD_UP),
    SYM(VK_GAMEPAD_DPAD_DOWN),
    SYM(VK_GAMEPAD_DPAD_LEFT),
    SYM(VK_GAMEPAD_DPAD_RIGHT),
    SYM(VK_GAMEPAD_MENU),
    SYM(VK_GAMEPAD_VIEW),
    SYM(VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON),
    SYM(VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON),
    SYM(VK_GAMEPAD_LEFT_THUMBSTICK_UP),
    SYM(VK_GAMEPAD_LEFT_THUMBSTICK_DOWN),
    SYM(VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT),
    SYM(VK_GAMEPAD_LEFT_THUMBSTICK_LEFT),
    SYM(VK_GAMEPAD_RIGHT_THUMBSTICK_UP),
    SYM(VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN),
    SYM(VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT),
    SYM(VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT),
    SYM(VK_OEM_4),
    SYM(VK_OEM_5),
    SYM(VK_OEM_6),
    SYM(VK_OEM_7),
    SYM(VK_OEM_8),
    SYM(VK_OEM_AX),
    SYM(VK_OEM_102),
    SYM(VK_ICO_HELP),
    SYM(VK_ICO_00),
    SYM(VK_PROCESSKEY),
    SYM(VK_ICO_CLEAR),
    SYM(VK_PACKET),
    SYM(VK_OEM_RESET),
    SYM(VK_OEM_JUMP),
    SYM(VK_OEM_PA1),
    SYM(VK_OEM_PA2),
    SYM(VK_OEM_PA3),
    SYM(VK_OEM_WSCTRL),
    SYM(VK_OEM_CUSEL),
    SYM(VK_OEM_ATTN),
    SYM(VK_OEM_FINISH),
    SYM(VK_OEM_COPY),
    SYM(VK_OEM_AUTO),
    SYM(VK_OEM_ENLW),
    SYM(VK_OEM_BACKTAB),
    SYM(VK_ATTN),
    SYM(VK_CRSEL),
    SYM(VK_EXSEL),
    SYM(VK_EREOF),
    SYM(VK_PLAY),
    SYM(VK_ZOOM),
    SYM(VK_NONAME),
    SYM(VK_PA1),
    SYM(VK_OEM_CLEAR),
    SYM(VK__none_)
};

const SymbolTable vk_flags_symbols{
    SYM(KBDEXT),
    SYM(KBDMULTIVK),
    SYM(KBDSPECIAL),
    SYM(KBDNUMPAD),
    SYM(KBDUNICODE),
    SYM(KBDINJECTEDVK),
    SYM(KBDMAPPEDVK),
    SYM(KBDBREAK)
};

const SymbolTable vk_attr_symbols{
    SYM(CAPLOK),
    SYM(SGCAPS),
    SYM(CAPLOKALTGR),
    SYM(KANALOK),
    SYM(GRPSELTAP)
};

const SymbolTable wchar_symbols{
    {'\t', "L'\\t'"},
    {'\n', "L'\\n'"},
    {'\r', "L'\\r'"},
    {'\'', "L'\\\''"},
    {'\\', "L'\\\\'"},
    SYM(WCH_NONE),
    SYM(WCH_DEAD),
    SYM(WCH_LGTR)
};

const SymbolTable wchar_literals{
    {'\t', "\\t"},
    {'\n', "\\n"},
    {'\r', "\\r"},
    {'"',  "\\\""},
    {'\\', "\\\\"},
};


//---------------------------------------------------------------------------
// Format a Pointer
//---------------------------------------------------------------------------

std::string Pointer(const void* value, const std::string& name)
{
    return value == nullptr ? "NULL" : name;
}


//---------------------------------------------------------------------------
// Format a WCHAR
//---------------------------------------------------------------------------

std::string Wchar(const ReverseOptions& opt, ::WCHAR value)
{
    if (!opt.num_only) {
        const auto it = wchar_symbols.find(value);
        if (it != wchar_symbols.end()) {
            return it->second;
        }
    }
    if (value == '\'' || value == '\\') {
        return Format("L'\\%c'", value);
    }
    else if (value >= L' ' && value < 0x007F) {
        return Format("L'%c'", value);
    }
    else {
        return Format("0x%04X", value);
    }
}


//---------------------------------------------------------------------------
// Format a string of WCHAR
//---------------------------------------------------------------------------

std::string Wstring(const ::WCHAR* value)
{
    if (value == nullptr) {
        return "NULL";
    }
    std::string str("L\"");
    for (; *value != 0; ++value) {
        const auto it = wchar_literals.find(*value);
        if (it != wchar_literals.end()) {
            str.append(it->second);
        }
        else if (*value >= L' ' && *value < 0x007F) {
            str.push_back(char(*value));
        }
        else {
            str.append(Format("\\x%04x", *value));
        }
    }
    str.push_back('"');
    return str;
}


//---------------------------------------------------------------------------
// Generate various parts of the source file.
//---------------------------------------------------------------------------

void GenerateVkToBits(const ReverseOptions& opt, std::ostream& out, const ::VK_TO_BIT* vtb, const std::string& name)
{
    Grid grid;
    for (; vtb->Vk != 0; vtb++) {
        grid.push_back({
            "{" + Symbol(opt, vk_symbols, vtb->Vk, 2) + ",",
            BitMask(opt, shift_state_symbols, vtb->ModBits, 4) + "},"
        });
    }
    grid.push_back({"{0,", "0}"});

    out << "//" << opt.dashed << std::endl
        << "// Associate a virtual key with a modifier bitmask" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static VK_TO_BIT " << name << "[] = {" << std::endl;
    PrintGrid(out, grid, "    ");
    out << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void GenerateCharModifiers(const ReverseOptions& opt, std::ostream& out, const ::MODIFIERS& mods, const std::string& name)
{
    const char* vk_to_bits_name = "vk_to_bits";

    if (mods.pVkToBit != nullptr) {
        GenerateVkToBits(opt, out, mods.pVkToBit, vk_to_bits_name);
    }

    // At most 8 entriess in ModNumber.
    static const std::string mod_names[] = {
        "000 = <none>",
        "001 = SHIFT",
        "010 = ALT",
        "011 = SHIFT ALT",
        "100 = CTRL",
        "101 = SHIFT CTRL",
        "110 = CTRL ALT",
        "111 = SHIFT CTRL ALT"
    };

    Grid grid;
    for (::WORD i = 0; i < mods.wMaxModBits; ++i) {
        GridLine line{ Symbol(opt, { SYM(SHFT_INVALID) }, mods.ModNumber[i]) + "," };
        if (i < ARRAYSIZE(mod_names)) {
            line.push_back("// " + mod_names[i]);
        }
        grid.push_back(line);
    }

    out << "//" << opt.dashed << std::endl
        << "// Map character modifier bits to modification number" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static MODIFIERS " << name << " = {" << std::endl
        << "    .pVkToBit    = " << (mods.pVkToBit != nullptr ? vk_to_bits_name : "nullptr") << "," << std::endl
        << "    .wMaxModBits = " << mods.wMaxModBits << "," << std::endl
        << "    .ModNumber   = {" << std::endl;
    PrintGrid(out, grid, "        ");
    out << "    }" << std::endl
        << "};" << std::endl
        << std::endl;
}

//---------------------------------------------------------------------------

void GenerateSubVkToWchar(const ReverseOptions& opt, std::ostream& out, const ::VK_TO_WCHARS10* vtwc, size_t count, size_t size, const std::string& name)
{
    // TODO: clarity role of each column to add proper comment in generated source.
    //  |         |  Shift  |  Ctrl   |  Ctl+Alt|S+Ctrl   |
    //  |=========|=========|=========|=========|=========|

    Grid grid;
    while (vtwc->VirtualKey != 0) {
        GridLine line({
            "{" + Symbol(opt, vk_symbols, vtwc->VirtualKey, 2) + ",",
            BitMask(opt, vk_attr_symbols, vtwc->Attributes, 2) + ","
        });
        for (size_t i = 0; i < count; ++i) {
            std::string str(Wchar(opt, vtwc->wch[i]));
            if (i == 0) {
                str.insert(0, 1, '{');
            }
            str.append(i == count - 1 ? "}}," : ",");
            line.push_back(str);
        }
        grid.push_back(line);

        // Move to next structure (variable size).
        vtwc = reinterpret_cast<const ::VK_TO_WCHARS10*>(reinterpret_cast<const char*>(vtwc) + size);
    }
    GridLine line({"{0,"});
    line.resize(count + 1, "0,");
    line.push_back("0}");
    grid.push_back(line);

    out << "//" << opt.dashed << std::endl
        << "// Virtual Key to WCHAR translations for " << count << " shift states" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static VK_TO_WCHARS" << count << " " << name << "[] = {" << std::endl;
    PrintGrid(out, grid, "    ");
    out << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void GenerateVkToWchar(const ReverseOptions& opt, std::ostream& out, const ::VK_TO_WCHAR_TABLE* vtwc, const std::string& name)
{
    Grid grid;
    for (; vtwc->pVkToWchars != nullptr; vtwc++) {
        const std::string sub_name(Format("vk_to_wchar%d", vtwc->nModifications));
        GenerateSubVkToWchar(opt, out, reinterpret_cast<PVK_TO_WCHARS10>(vtwc->pVkToWchars), vtwc->nModifications, vtwc->cbSize, sub_name);
        grid.push_back({
            "{(PVK_TO_WCHARS1)" + sub_name + ",",
            Format("%d,", vtwc->nModifications),
            "sizeof(" + sub_name + "[0])},"
        });
    }
    grid.push_back({ "{NULL,", "0,", "0}" });
 
    out << "//" << opt.dashed << std::endl
        << "// Virtual Key to WCHAR translations with shift states" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static VK_TO_WCHAR_TABLE " << name << "[] = {" << std::endl;
    PrintGrid(out, grid, "    ");
    out << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void GenerateLgToWchar(const ReverseOptions& opt, std::ostream& out, const ::LIGATURE1* ligatures, size_t count, size_t size, const std::string& name)
{
    const ::LIGATURE5* lg = reinterpret_cast<const ::LIGATURE5*>(ligatures);

    Grid grid;
    while (lg->VirtualKey != 0) {
        GridLine line({
            "{" + Symbol(opt, vk_symbols, lg->VirtualKey, 2) + ",",
            Format("%d,", lg->ModificationNumber)
        });
        for (size_t i = 0; i < count; ++i) {
            std::string str(Wchar(opt, lg->wch[i]));
            if (i == 0) {
                str.insert(0, 1, '{');
            }
            str.append(i == count - 1 ? "}}," : ",");
            line.push_back(str);
        }
        grid.push_back(line);

        // Move to next structure (variable size).
        lg = reinterpret_cast<const ::LIGATURE5*>(reinterpret_cast<const char*>(lg) + size);
    }
    GridLine line({ "{0," });
    line.resize(count, "0,");
    line.push_back("0}");
    grid.push_back(line);

    out << "//" << opt.dashed << std::endl
        << "// Ligatures to WCHAR translations" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static LIGATURE" << count << " " << name << "[] = {" << std::endl;
    PrintGrid(out, grid, "    ");
    out << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void GenerateDeadKeys(const ReverseOptions& opt, std::ostream& out, const ::DEADKEY* dk, const std::string& name)
{
    Grid grid;
    for (; dk->dwBoth != 0; dk++) {
        grid.push_back({
            "DEADTRANS(" + Wchar(opt, LOWORD(dk->dwBoth)) + ",",
            Wchar(opt, HIWORD(dk->dwBoth)) + ",",
            Wchar(opt, dk->wchComposed) + ",",
            BitMask(opt, { SYM(DKF_DEAD) }, dk->uFlags, 4) + "),"
        });
    }

    out << "//" << opt.dashed << std::endl
        << "// Dead keys sequences translations" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static DEADKEY " << name << "[] = {" << std::endl;
    PrintGrid(out, grid, "    ");
    out << "    {0, 0, 0}" << std::endl
        << "};" << std::endl
        << std::endl;
}

//---------------------------------------------------------------------------

void GenerateVscToString(const ReverseOptions& opt, std::ostream& out, const ::VSC_LPWSTR* vts, const std::string& name, const std::string& comment = "")
{
    Grid grid;
    for (; vts->vsc != 0; vts++) {
        grid.push_back({
            "{" + Format("0x%02X", vts->vsc) + ",",
            Wstring(vts->pwsz) + "},"
        });
    }
    grid.push_back({ "{0x00,", "NULL}"});

    out << "//" << opt.dashed << std::endl
        << "// Scan codes to key names" << comment << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static VSC_LPWSTR " << name << "[] = {" << std::endl;
    PrintGrid(out, grid, "    ");
    out << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void GenerateKeyNames(const ReverseOptions& opt, std::ostream& out, const ::DEADKEY_LPWSTR* names, const std::string& name)
{
    Grid grid;
    for (; *names != nullptr; ++names) {
        if (**names != 0) {
            ::WCHAR prefix[2]{ **names, L'\0' };
            grid.push_back({
                Wstring(prefix),
                Wstring(*names + 1) + ","
            });
        }
    }

    out << "//" << opt.dashed << std::endl
        << "// Names of dead keys" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static DEADKEY_LPWSTR " << name << "[] = {" << std::endl;
    PrintGrid(out, grid, "    ");
    out << "    NULL" << std::endl << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void GenerateScanToVk(const ReverseOptions& opt, std::ostream& out, const ::USHORT* vk, size_t vk_count, const std::string& name)
{
    out << "//" << opt.dashed << std::endl
        << "// Scan code to virtual key conversion table" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static USHORT " << name << "[] = {" << std::endl;
 
    for (size_t i = 0; i < vk_count; ++i) {
        out << Format("    /* %02X */ ", i) << Attributes(opt, vk_symbols, vk_flags_symbols, vk[i], 4) << "," << std::endl;
    }

    out << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void GenerateVscToVk(const ReverseOptions& opt, std::ostream& out, const ::VSC_VK* vtvk, const std::string& name, const std::string& comment = "")
{
    Grid grid;
    for (; vtvk->Vsc != 0; vtvk++) {
        grid.push_back({
            "{" + Format("0x%02X", vtvk->Vsc) + ",",
            Attributes(opt, vk_symbols, vk_flags_symbols, vtvk->Vk, 4) + "},"
        });
    }
    grid.push_back({ "{0x00,", "0x0000}" });

    out << "//" << opt.dashed << std::endl
        << "// Scan code to virtual key conversion table" << comment << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static VSC_VK " << name << "[] = {" << std::endl;
    PrintGrid(out, grid, "    ");
    out << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

std::string LocaleFlags(const ReverseOptions& opt, ::DWORD flags)
{
    if (opt.num_only) {
        return Format("0x%08X", flags);
    }
    else {
        std::string lostr(BitMask(opt, { SYM(KLLF_ALTGR), SYM(KLLF_SHIFTLOCK), SYM(KLLF_LRM_RLM)}, LOWORD(flags), 4));
        std::string histr(Symbol(opt, { SYM(KBD_VERSION) }, HIWORD(flags), 4));
        return "MAKELONG(" + lostr + ", " + histr + ")";
    }
}


//---------------------------------------------------------------------------
// Starting point of the source file creation.
//---------------------------------------------------------------------------

void GenerateSourceFile(const ReverseOptions& opt, std::ostream& out, const ::KBDTABLES& tables)
{
    out << "//" << opt.dashed << std::endl
        << "// " << opt.comment << std::endl
        << "// Automatically generated from " << FileName(opt.input) << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "#define KBD_TYPE " << (opt.kbd_type > 0 ? opt.kbd_type : (tables.dwType > 0 ? tables.dwType : 4)) << std::endl
        << std::endl
        << "#include <windows.h>" << std::endl
        << "#include <kbd.h>" << std::endl
        << "#include <dontuse.h>" << std::endl
        << std::endl;

    const std::string char_modifiers_name = "char_modifiers";
    if (tables.pCharModifiers != nullptr) {
        GenerateCharModifiers(opt, out, *tables.pCharModifiers, char_modifiers_name);
    }

    const std::string vk_to_wchar_name = "vk_to_wchar";
    if (tables.pVkToWcharTable != nullptr) {
        GenerateVkToWchar(opt, out, tables.pVkToWcharTable, vk_to_wchar_name);
    }

    const std::string dead_keys_name = "dead_keys";
    if (tables.pDeadKey != nullptr) {
        GenerateDeadKeys(opt, out, tables.pDeadKey, dead_keys_name);
    }

    const std::string key_names_name = "key_names";
    if (tables.pKeyNames != nullptr) {
        GenerateVscToString(opt, out, tables.pKeyNames, key_names_name);
    }

    const std::string key_names_ext_name = "key_names_ext";
    if (tables.pKeyNamesExt != nullptr) {
        GenerateVscToString(opt, out, tables.pKeyNamesExt, key_names_ext_name, " (extended keypad)");
    }

    const std::string key_names_dead_name = "key_names_dead";
    if (tables.pKeyNamesDead != nullptr) {
        GenerateKeyNames(opt, out, tables.pKeyNamesDead, key_names_dead_name);
    }

    const std::string scancode_to_vk_name = "scancode_to_vk";
    if (tables.pusVSCtoVK != nullptr) {
        GenerateScanToVk(opt, out, tables.pusVSCtoVK, tables.bMaxVSCtoVK, scancode_to_vk_name);
    }

    const std::string scancode_to_vk_e0_name = "scancode_to_vk_e0";
    if (tables.pVSCtoVK_E0 != nullptr) {
        GenerateVscToVk(opt, out, tables.pVSCtoVK_E0, scancode_to_vk_e0_name, " (scancodes with E0 prefix)");
    }

    const std::string scancode_to_vk_e1_name = "scancode_to_vk_e1";
    if (tables.pVSCtoVK_E1 != nullptr) {
        GenerateVscToVk(opt, out, tables.pVSCtoVK_E1, scancode_to_vk_e1_name, " (scancodes with E1 prefix)");
    }

    const std::string ligatures_name = "ligatures";
    if (tables.pLigature != nullptr) {
        GenerateLgToWchar(opt, out, tables.pLigature, tables.nLgMax, tables.cbLgEntry, ligatures_name);
    }

    const std::string kbd_table_name = "kbd_tables";
    out << "//" << opt.dashed << std::endl
        << "// Main keyboard layout structure, point to all tables" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "static KBDTABLES " << kbd_table_name << " = {" << std::endl
        << "    .pCharModifiers  = " << Pointer(tables.pCharModifiers, "&" + char_modifiers_name) << "," << std::endl
        << "    .pVkToWcharTable = " << Pointer(tables.pVkToWcharTable, vk_to_wchar_name) << "," << std::endl
        << "    .pDeadKey        = " << Pointer(tables.pDeadKey, dead_keys_name) << "," << std::endl
        << "    .pKeyNames       = " << Pointer(tables.pKeyNames, key_names_name) << "," << std::endl
        << "    .pKeyNamesExt    = " << Pointer(tables.pKeyNamesExt, key_names_ext_name) << "," << std::endl
        << "    .pKeyNamesDead   = " << Pointer(tables.pKeyNamesDead, key_names_dead_name) << "," << std::endl
        << "    .pusVSCtoVK      = " << Pointer(tables.pusVSCtoVK, scancode_to_vk_name) << "," << std::endl
        << "    .bMaxVSCtoVK     = " << (tables.pusVSCtoVK == nullptr ? "0," : "ARRAYSIZE(" + scancode_to_vk_name + "),") << std::endl
        << "    .pVSCtoVK_E0     = " << Pointer(tables.pVSCtoVK_E0, scancode_to_vk_e0_name) << "," << std::endl
        << "    .pVSCtoVK_E1     = " << Pointer(tables.pVSCtoVK_E1, scancode_to_vk_e1_name) << "," << std::endl
        << "    .fLocaleFlags    = " << LocaleFlags(opt, tables.fLocaleFlags) << "," << std::endl
        << "    .nLgMax          = " << int(tables.nLgMax) << "," << std::endl
        << "    .cbLgEntry       = " << (tables.pLigature == nullptr ? "0," : "sizeof(" + ligatures_name + "[0]),") << std::endl
        << "    .pLigature       = " << Pointer(tables.pLigature, "(PLIGATURE1)" + ligatures_name) << "," << std::endl
        << "    .dwType          = " << tables.dwType << "," << std::endl
        << "    .dwSubType       = " << tables.dwSubType << "," << std::endl
        << "};" << std::endl
        << std::endl
        << "//" << opt.dashed << std::endl
        << "// Keyboard layout entry point" << std::endl
        << "//" << opt.dashed << std::endl
        << std::endl
        << "__declspec(dllexport) PKBDTABLES " KBD_DLL_ENTRY_NAME "(void)" << std::endl
        << "{" << std::endl
        << "    return &" << kbd_table_name << ";" << std::endl
        << "}" << std::endl;
}

//---------------------------------------------------------------------------
// Application entry point.
//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Parse command line options.
    ReverseOptions opt(argc, argv);

    // If input does not look like a file name, must be a keyboard name.
    if (opt.input.find_first_of(":\\/.") == std::string::npos) {
        opt.input = GetEnv("SYSTEMROOT", "C:\\Windows") + "\\System32\\kbd" + opt.input + ".dll";
    }

    // Load the DLL in our virtual memory space.
    ::HMODULE dll = ::LoadLibraryA(opt.input.c_str());
    if (dll == nullptr) {
        const ::DWORD err = ::GetLastError();
        opt.fatal("error opening " + opt.input + ": " + Error(err));
    }

    // Get the DLL entry point.
    ::FARPROC proc_addr = ::GetProcAddress(dll, KBD_DLL_ENTRY_NAME);
    if (proc_addr == nullptr) {
        const ::DWORD err = ::GetLastError();
        opt.fatal("cannot find " KBD_DLL_ENTRY_NAME " in " + opt.input + ": " + Error(err));
    }

    // Call the entry point to get the keyboard tables.
    // The entry point profile is: PKBDTABLES KbdLayerDescriptor()
    ::PKBDTABLES tables = reinterpret_cast<::PKBDTABLES(*)()>(proc_addr)();
    if (tables == nullptr) {
        opt.fatal(KBD_DLL_ENTRY_NAME "() returned null in " + opt.input);
    }

    // Keyboard tables are now identified, generate the source file.
    if (opt.output.empty()) {
        // No output specified, using standard output.
        GenerateSourceFile(opt, std::cout, *tables);
    }
    else {
        // Create the specified output file.
        std::ofstream out(opt.output);
        if (out) {
            GenerateSourceFile(opt, out, *tables);
        }
        else {
            opt.fatal("cannot create " + opt.output);
        }
    }
    return EXIT_SUCCESS;
}
