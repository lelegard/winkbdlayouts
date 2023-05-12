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
#include "winutils.h"
#include "registry.h"
#include "grid.h"
#include "fileversion.h"
#include "winkeymap.h"
#include "unicode.h"

// Tables of values => symbols
typedef __int64 Value;
typedef std::map<Value, WString> SymbolTable;
#define SYM(e) {e, L#e}

// Configure the terminal console on init, restore on exit.
ConsoleState state;

// So-called "ligatures" can generate up to 16 characters.
// In "kbd.h", typedef's are predefined up to 5 characters.
// Longer typedef's must be explicitly defined.
#define LIG_MAX_PREDEFINED 5
#define LIG_MAX            16
TYPEDEF_LIGATURE(16)
typedef LIGATURE16 LIGATURE_MAX;


//----------------------------------------------------------------------------
// Command line options.
//----------------------------------------------------------------------------

class ReverseOptions : public Options
{
public:
    // Constructor.
    ReverseOptions(int argc, wchar_t* argv[]);

    // Command line options.
    WString     dashed;
    WString     input;
    WString     output;
    WString     comment;
    WString     map_template;
    WStringList headers;
    int         kbd_type;
    bool        num_only;
    bool        hexa_dump;
    bool        gen_resources;
    bool        gen_list;
};

ReverseOptions::ReverseOptions(int argc, wchar_t* argv[]) :
    Options(argc, argv,
        L"[options] kbd-name-or-file\n"
        L"\n"
        L"  kbd-name-or-file : Either the file name of a keyboard layout DLL or the\n"
        L"  name of a keyboard layout, for instance \"fr\" for C:\\Windows\\System32\\kbdfr.dll\n"
        L"\n"
        L"Options:\n"
        L"\n"
        L"  -c \"string\" : comment string in the header\n"
        L"  -d : add hexa dump in final comments\n"
        L"  -h : display this help text\n"
        L"  -l : generate a list of characters instead of a C source file\n"
        L"  -m infile : generate a keybard map based on the specified template\n"
        L"  -n : numerical output only, do not attempt to translate to source macros\n"
        L"  -o outfile : output file name, default is standard output\n"
        L"  -r : generate a resource file instead of a C source file\n"
        L"  -t value : keyboard type, defaults to dwType in kbd table or 4 if unspecified\n"
        L"  -u outfile : same as -o but update output, keeping leading comments"),
    dashed(75, L'-'),
    input(),
    output(),
    comment(L"Windows Keyboards Layouts (WKL)"),
    map_template(),
    headers(),
    kbd_type(0),
    num_only(false),
    hexa_dump(false),
    gen_resources(false),
    gen_list(false)
{
    bool get_headers = false;

    // Parse arguments.
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == L"--help" || args[i] == L"-h") {
            usage();
        }
        else if (args[i] == L"-d") {
            hexa_dump = true;
        }
        else if (args[i] == L"-n") {
            num_only = true;
        }
        else if (args[i] == L"-r") {
            gen_resources = true;
        }
        else if (args[i] == L"-l") {
            gen_list = true;
        }
        else if (args[i] == L"-o" && i + 1 < args.size()) {
            output = args[++i];
        }
        else if (args[i] == L"-u" && i + 1 < args.size()) {
            output = args[++i];
            get_headers = true;
        }
        else if (args[i] == L"-m" && i + 1 < args.size()) {
            map_template = args[++i];
        }
        else if (args[i] == L"-c" && i + 1 < args.size()) {
            comment = args[++i];
        }
        else if (args[i] == L"-t" && i + 1 < args.size()) {
            kbd_type = ToInt(args[++i]);
        }
        else if (!args[i].empty() && args[i].front() != '-' && input.empty()) {
            input = args[i];
        }
        else {
            fatal("invalid option '" + args[i] + "', try --help");
        }
    }
    if (input.empty()) {
        fatal(L"no keyboard layout specified, try --help");
    }
    if (get_headers) {
        // -u is used, load existing headers from previous output file, if it exists.
        std::string line;
        std::ifstream prev(output);
        while (std::getline(prev, line)) {
            // Remove leading (optional BOM) and trailing control characters.
            while (!line.empty() && line.back() < 0x20) {
                line.pop_back();
            }
            while (!line.empty() && line[0] > 0xB0) {
                line.erase(0, 1);
            }
            if (line.length() >= 2 && line[0] == '/' && line[1] == '/') {
                headers.push_back(ToUTF16(line));
            }
            else {
                break;
            }
        }
    }
}


//---------------------------------------------------------------------------
// Common symbol tables.
//---------------------------------------------------------------------------

// Full description of a modifier state, for use in comments in MODIFIERS structure.
static const WStringVector modifiers_comments {
    L"000 = <none>",
    L"001 = Shift",
    L"010 = Control",
    L"011 = Shift Control",
    L"100 = Alt",
    L"101 = Shift Alt",
    L"110 = Control Alt (AltGr)",
    L"111 = Shift Control Alt (Shift AltGr)"
};

// Top of columns of VK_TO_WCHARSx structures.
static const WStringVector modifiers_headers {
    L"Base",
    L"Shift",
    L"Ctrl",
    L"Shift/Ctrl",
    L"Alt",
    L"Shift/Alt",
    L"AltGr",       // AltGr = Ctrl/Alt
    L"Shift/AltGr"  // Shift/Ctrl/Alt
};

const SymbolTable shift_state_symbols {
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

const SymbolTable vk_flags_symbols {
    SYM(KBDEXT),
    SYM(KBDMULTIVK),
    SYM(KBDSPECIAL),
    SYM(KBDNUMPAD),
    SYM(KBDUNICODE),
    SYM(KBDINJECTEDVK),
    SYM(KBDMAPPEDVK),
    SYM(KBDBREAK)
};

const SymbolTable vk_attr_symbols {
    SYM(CAPLOK),
    SYM(SGCAPS),
    SYM(CAPLOKALTGR),
    SYM(KANALOK),
    SYM(GRPSELTAP)
};

// Complete symbol for a WCHAR (a character literal).
const SymbolTable wchar_symbols {
    {'\t', L"L'\\t'"},
    {'\n', L"L'\\n'"},
    {'\r', L"L'\\r'"},
    {'\'', L"L'\\\''"},
    {'\\', L"L'\\\\'"},
    SYM(WCH_NONE),
    SYM(WCH_DEAD),
    SYM(WCH_LGTR),
    // Automatically generated file (using a Python script)
    #include "unicode_syms.h"
};


//---------------------------------------------------------------------------
// Description of one data structure.
//---------------------------------------------------------------------------

class DataStructure
{
public:
    WString name;
    const void*  address;
    size_t       size;

    // Constructors with address or integer.
    DataStructure(const WString& n = L"", const void* a = nullptr, size_t s = 0)
        : name(n), address(a), size(s) {}
    DataStructure(const WString& n, const void* a, const void* end)
        : name(n), address(a), size(uintptr_t(end) - uintptr_t(a)) {}
    DataStructure(const WString& n, uintptr_t a, size_t s = 0)
        : name(n), address(reinterpret_cast<const void*>(a)), size(s) {}

    // Get/set address after last byte.
    const void* end() const { return reinterpret_cast<const uint8_t*>(address) + size; }
    void setEnd(const void* e) { size = uintptr_t(e) - uintptr_t(address); }

    // Sort operator.
    bool operator<(const DataStructure& s) const { return address < s.address; }

    // Hexa dump of the structure.
    void dump(std::ostream&) const;
};

void DataStructure::dump(std::ostream& out) const
{
    const WString header(name + Format(L" (%d bytes)", int(size)));
    out << "//" << std::endl
        << "// " << header << std::endl
        << "// " << std::string(header.length(), '-') << std::endl;
    PrintHexa(out, address, size, L"// ", true);
}


//---------------------------------------------------------------------------
// Generate various parts of the source file.
//---------------------------------------------------------------------------

class SourceGenerator
{
public:
    // Constructor.
    SourceGenerator(ReverseOptions& opt) : _ou(opt.out()), _opt(opt), _alldata() {}

    // Generate the source 
    void generate(const KBDTABLES&);

private:
    std::ostream&            _ou;
    ReverseOptions&          _opt;
    std::list<DataStructure> _alldata;

    // Format an integer as a decimal or hexadecimal string.
    // If hex_digits is zero, format in decimal.
    WString integer(Value value, int hex_digits = 0);

    // Format an integer as a string, using a table of symbols.
    // If no symbol found or option -n, return a number.
    // If hex_digits is zero, format in decimal.
    WString symbol(const SymbolTable& symbols, Value value, int hex_digits = 0);

    // Format a bit mask of symbols, same principle as symbol().
    WString bitMask(const SymbolTable& symbols, Value value, int hex_digits = 0);

    // Format a symbol and a bit mask of attributes, same principle as Symbol().
    WString attributes(const SymbolTable& symbols, const SymbolTable& attributes, Value value, int hex_digits = 0);

    // Format locale flags according to symbols.
    WString localeFlags(DWORD flags);

    // Format a Pointer
    WString pointer(const void* value, const WString& name);

    // Format a WCHAR. Add description in descs if one exists.
    WString wchar(wchar_t value);

    // Sort and merge adjacent data structures with same names (typically "Strings in ...").
    void sortDataStructures();

    // Generate the various data structures.
    void genVkToBits(const VK_TO_BIT*, const WString& name);
    void genCharModifiers(const MODIFIERS&, const WString& name);
    void genSubVkToWchar(const VK_TO_WCHARS10*, size_t count, size_t size, const WString& name, const MODIFIERS*);
    void genVkToWchar(const VK_TO_WCHAR_TABLE*, const WString& name, const::MODIFIERS*);
    void genLgToWchar(const LIGATURE1*, size_t count, size_t size, const WString& name, const MODIFIERS*);
    void genDeadKeys(const DEADKEY*, const WString& name);
    void genVscToString(const VSC_LPWSTR*, const WString& name, const WString& comment = L"");
    void genKeyNames(const DEADKEY_LPWSTR*, const WString& name);
    void genScanToVk(const USHORT* vk, size_t vk_count, const WString& name);
    void genVscToVk(const VSC_VK*, const WString& name, const WString& comment = L"");
    void genHexaDump();
};

//---------------------------------------------------------------------------

WString SourceGenerator::integer(Value value, int hex_digits)
{
    return hex_digits <= 0 ? Format(L"%lld", value) : Format(L"0x%0*llX", hex_digits, value);
}

//---------------------------------------------------------------------------

WString SourceGenerator::symbol(const SymbolTable& symbols, Value value, int hex_digits)
{
    if (!_opt.num_only) {
        const auto it = symbols.find(value);
        if (it != symbols.end()) {
            return it->second;
        }
    }
    return integer(value, hex_digits);
}

//---------------------------------------------------------------------------

WString SourceGenerator::bitMask(const SymbolTable& symbols, Value value, int hex_digits)
{
    if (!_opt.num_only) {
        WString str;
        Value bits = 0;
        for (const auto& sym : symbols) {
            if (sym.first == 0 && value == 0) {
                // Specific symbol for zero (no flag)
                return sym.second;
            }
            if (sym.first != 0 && (value & sym.first) == sym.first) {
                // Found one flag.
                if (!str.empty()) {
                    str += L" | ";
                }
                str += sym.second;
                bits |= sym.first;
            }
        }
        if (bits != 0) {
            // Found at least some bits, add remaining bits.
            if ((value & ~bits) != 0) {
                if (!str.empty()) {
                    str += L" | ";
                }
                str += Format(L"0x%0*lld", hex_digits, value & ~bits);
            }
            return str;
        }
    }
    return integer(value, hex_digits);
}

//---------------------------------------------------------------------------

WString SourceGenerator::attributes(const SymbolTable& symbols, const SymbolTable& attributes, Value value, int hex_digits)
{
    if (!_opt.num_only) {
        // Compute mask of all possible attributes.
        Value all_attributes = 0;
        for (const auto& sym : attributes) {
            all_attributes |= sym.first;
        }
        // Base value.
        WString str(symbol(symbols, value & ~all_attributes, hex_digits));
        // Add attributes.
        if ((value & all_attributes) != 0) {
            str += L" | " + bitMask(attributes, value & all_attributes, hex_digits);
        }
        return str;
    }
    return integer(value, hex_digits);
}

//---------------------------------------------------------------------------

WString SourceGenerator::localeFlags(const DWORD flags)
{
    if (_opt.num_only) {
        return Format(L"0x%08X", flags);
    }
    else {
        WString lostr(bitMask({ SYM(KLLF_ALTGR), SYM(KLLF_SHIFTLOCK), SYM(KLLF_LRM_RLM) }, LOWORD(flags), 4));
        WString histr(symbol({ SYM(KBD_VERSION) }, HIWORD(flags), 4));
        return L"MAKELONG(" + lostr + L", " + histr + L")";
    }
}

//---------------------------------------------------------------------------

WString SourceGenerator::pointer(const void* value, const WString& name)
{
    return value == nullptr ? L"NULL" : name;
}

//---------------------------------------------------------------------------

WString SourceGenerator::wchar(wchar_t value)
{
    // Format a WCHAR. Add description in descs if one exists.
    if (!_opt.num_only) {
        const auto sym = wchar_symbols.find(value);
        if (sym != wchar_symbols.end()) {
            return sym->second;
        }
    }
    if (value == L'\'' || value == L'\\') {
        WString res(L"L'\\ '");
        res[3] = value;
        return res;
    }
    else if (value >= L' ' && value < 0x007F) {
        WString res(L"L' '");
        res[2] = value;
        return res;
    }
    else {
        return Format(L"0x%04X", value);
    }
}

//---------------------------------------------------------------------------

void SourceGenerator::sortDataStructures()
{
    // Sort all data structures by address.
    _alldata.sort();

    // Merge adjacent data structures with same names (typically "Strings in ...").
    auto current = _alldata.begin();
    auto previous = current++;
    while (current != _alldata.end()) {
        const bool inter_zero = IsZero(previous->end(), current->address);
        // Merge if the two data structures have the same name and are adjacent or
        // only separated by zeroes (typpically padding).
        if (previous->name == current->name && (previous->end() == current->address || inter_zero)) {
            // Merge previous and current structure.
            previous->size = uintptr_t(current->end()) - uintptr_t(previous->address);
            current = _alldata.erase(current);
        }
        else {
            // If there is empty space between the two structures, create a structure for it.
            if (previous->end() < current->address) {
                DataStructure inter(inter_zero ? L"Padding" : L"Unreferenced", previous->end(), current->address);
                current = _alldata.insert(current, inter);
            }
            // Move to next pair of structures.
            previous = current;
            ++current;
        }
    }
}

//---------------------------------------------------------------------------

void SourceGenerator::genVkToBits(const VK_TO_BIT* vtb, const WString& name)
{
    DataStructure ds(name, vtb);

    Grid grid;
    for (; vtb->Vk != 0; vtb++) {
        grid.addLine({
            L"{" + symbol(vk_symbols, vtb->Vk, 2) + L",",
            bitMask(shift_state_symbols, vtb->ModBits, 4) + L"},"
        });
    }
    grid.addLine({L"{0,", L"0}"});
    vtb++;

    ds.setEnd(vtb);
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Associate a virtual key with a modifier bitmask" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static VK_TO_BIT " << name << "[] = {" << std::endl;
    grid.setMargin(4);
    grid.print(_ou);
    _ou << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::genCharModifiers(const MODIFIERS& mods, const WString& name)
{
    const wchar_t* vk_to_bits_name = L"vk_to_bits";
    if (mods.pVkToBit != nullptr) {
        genVkToBits(mods.pVkToBit, vk_to_bits_name);
    }

    Grid grid;
    // Note: wMaxModBits is the "max value", ie. size = wMaxModBits + 1
    for (WORD i = 0; i <= mods.wMaxModBits; ++i) {
        grid.addLine({symbol({SYM(SHFT_INVALID)}, mods.ModNumber[i]) + ","});
        if (!_opt.num_only && i < modifiers_comments.size()) {
            grid.addColumn(L"// " + modifiers_comments[i]);
        }
    }

    DataStructure ds(name, &mods);
    ds.setEnd(&mods.ModNumber[0] + mods.wMaxModBits + 1);
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Map character modifier bits to modification number" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static MODIFIERS " << name << " = {" << std::endl
        << "    .pVkToBit    = " << (mods.pVkToBit != nullptr ? vk_to_bits_name : L"NULL") << "," << std::endl
        << "    .wMaxModBits = " << mods.wMaxModBits << "," << std::endl
        << "    .ModNumber   = {" << std::endl;
    grid.setMargin(8);
    grid.print(_ou);
    _ou << "    }" << std::endl
        << "};" << std::endl
        << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::genSubVkToWchar(const VK_TO_WCHARS10* vtwc, size_t count, size_t size, const WString& name, const MODIFIERS* mods)
{
    DataStructure ds(name, vtwc);
    Grid grid;

    // Add header lines of comments to indicate the type of modifier on top of each column.
    if (mods != nullptr && !_opt.num_only) {
        Grid::Line headers(2 + count);
        headers[0] = L"//";
        bool not_empty = false;
        for (size_t i = 0; i <= mods->wMaxModBits && i < modifiers_headers.size(); ++i) {
            const size_t index = mods->ModNumber[i];
            if (2 + index < headers.size()) {
                headers[2 + index] = modifiers_headers[i];
                not_empty = not_empty || !modifiers_headers[i].empty();
            }
        }
        if (not_empty) {
            grid.addLine(headers);
            grid.addUnderlines({ L"//" });
        }
    }

    while (vtwc->VirtualKey != 0) {
        grid.addLine({
            "{" + symbol(vk_symbols, vtwc->VirtualKey, 2) + ",",
            bitMask(vk_attr_symbols, vtwc->Attributes, 2) + ","
        });
        for (size_t i = 0; i < count; ++i) {
            WString str(wchar(vtwc->wch[i]));
            if (i == 0) {
                str.insert(0, 1, L'{');
            }
            str.append(i == count - 1 ? L"}}," : L",");
            grid.addColumn(str);
        }

        // Move to next structure (variable size).
        vtwc = reinterpret_cast<const VK_TO_WCHARS10*>(reinterpret_cast<const char*>(vtwc) + size);
    }

    // Last null element.
    Grid::Line line({L"{0,"});
    line.resize(count + 1, L"0,");
    line.push_back(L"0}");
    grid.addLine(line);
    vtwc = reinterpret_cast<const VK_TO_WCHARS10*>(reinterpret_cast<const char*>(vtwc) + size);

    ds.setEnd(vtwc);
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Virtual Key to WCHAR translations for " << count << " shift states" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static VK_TO_WCHARS" << count << " " << name << "[] = {" << std::endl;
    grid.setMargin(4);
    grid.print(_ou);
    _ou << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::genVkToWchar(const VK_TO_WCHAR_TABLE* vtwc, const WString& name, const::MODIFIERS* mods)
{
    DataStructure ds(name, vtwc);

    Grid grid;
    for (; vtwc->pVkToWchars != nullptr; vtwc++) {
        const WString sub_name(Format(L"vk_to_wchar%d", vtwc->nModifications));
        genSubVkToWchar(reinterpret_cast<PVK_TO_WCHARS10>(vtwc->pVkToWchars), vtwc->nModifications, vtwc->cbSize, sub_name, mods);
        grid.addLine({
            L"{(PVK_TO_WCHARS1)" + sub_name + L",",
            Format(L"%d,", vtwc->nModifications),
            L"sizeof(" + sub_name + L"[0])},"
        });
    }
    grid.addLine({L"{NULL,", L"0,", L"0}"});
    vtwc++;

    ds.setEnd(vtwc);
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Virtual Key to WCHAR translations with shift states" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static VK_TO_WCHAR_TABLE " << name << "[] = {" << std::endl;
    grid.setMargin(4);
    grid.print(_ou);
    _ou << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::genLgToWchar(const LIGATURE1* ligatures, size_t count, size_t size, const WString& name, const MODIFIERS* mods)
{
    DataStructure ds(name, ligatures);
    const LIGATURE_MAX* lg = reinterpret_cast<const LIGATURE_MAX*>(ligatures);

    Grid grid;
    while (lg->VirtualKey != 0) {
        WStringList comments;
        // Start of entry: virtual key and modification number.
        grid.addLine({
            L"{" + symbol(vk_symbols, lg->VirtualKey, 2) + L",",
            Format(L"%d,", lg->ModificationNumber)
        });
        // Search a description for the modification number.
        if (mods != nullptr && !_opt.num_only) {
            for (size_t i = 0; i <= mods->wMaxModBits && i < modifiers_headers.size(); ++i) {
                if (lg->ModificationNumber == mods->ModNumber[i] && !modifiers_headers[i].empty()) {
                    comments.push_back(modifiers_headers[i]);
                    break;
                }
            }
        }
        // List of generated characters for that ligature.
        for (size_t i = 0; i < count; ++i) {
            WString str(wchar(lg->wch[i]));
            if (i == 0) {
                str.insert(0, 1, '{');
            }
            str.append(i == count - 1 ? L"}}," : L",");
            grid.addColumn(str);
        }
        // Add any interesting comment.
        if (!comments.empty()) {
            grid.addColumn(L"// " + Join(comments, L", "));
        }
        // Move to next structure (variable size).
        lg = reinterpret_cast<const LIGATURE_MAX*>(reinterpret_cast<const char*>(lg) + size);
    }

    // Last null element.
    Grid::Line line({L"{0,", L"0,"});
    switch (count) {
        case 0:
            line.push_back(L"}");
            break;
        case 1:
            line.push_back(L"{0}}");
            break;
        default:
            line.push_back(L"{0, ");
            line.resize(count + 1, L"0,");
            line.push_back(L"0}}");
    }
    grid.addLine(line);
    lg = reinterpret_cast<const LIGATURE_MAX*>(reinterpret_cast<const char*>(lg) + size);

    ds.setEnd(lg);
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Ligatures to WCHAR translations" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl;
    if (count > LIG_MAX_PREDEFINED) {
        _ou << "TYPEDEF_LIGATURE(" << count << ")" << std::endl
            << std::endl;
    }
    _ou << "static LIGATURE" << count << " " << name << "[] = {" << std::endl;
    grid.setMargin(4);
    grid.print(_ou);
    _ou << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::genDeadKeys(const DEADKEY* dk, const WString& name)
{
    DataStructure ds(name, dk);

    Grid grid;
    grid.addLine({L"//", L"Accent", L"Composed", L"Flags"});
    grid.addUnderlines({L"//"});
    for (; dk->dwBoth != 0; dk++) {
        grid.addLine({
            L"DEADTRANS(" + wchar(LOWORD(dk->dwBoth)) + ",",
            wchar(HIWORD(dk->dwBoth)) + ",",
            wchar(dk->wchComposed) + ",",
            bitMask({SYM(DKF_DEAD)}, dk->uFlags, 4) + "),"
        });
    }
    dk++; // last null element

    ds.setEnd(dk);
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Dead keys sequences translations" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static DEADKEY " << name << "[] = {" << std::endl;
    grid.setMargin(4);
    grid.print(_ou);
    _ou << "    {0, 0, 0}" << std::endl
        << "};" << std::endl
        << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::genVscToString(const VSC_LPWSTR* vts, const WString& name, const WString& comment)
{
    DataStructure ds(name, vts);

    Grid grid;
    for (; vts->vsc != 0; vts++) {
        grid.addLine({
            L"{" + Format(L"0x%02X", vts->vsc) + L",",
            WStringLiteral(vts->pwsz) + L"},"
        });
        _alldata.push_back(DataStructure("Strings in " + name, vts->pwsz, StringSize(vts->pwsz)));
    }
    grid.addLine({L"{0x00,", L"NULL}"});
    vts++;

    ds.setEnd(vts);
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Scan codes to key names" << comment << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static VSC_LPWSTR " << name << "[] = {" << std::endl;
    grid.setMargin(4);
    grid.print(_ou);
    _ou << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::genKeyNames(const DEADKEY_LPWSTR* names, const WString& name)
{
    DataStructure ds(name, names);

    Grid grid;
    for (; *names != nullptr; ++names) {
        if (**names != 0) {
            WCHAR prefix[2]{ **names, L'\0' };
            grid.addLine({WStringLiteral(prefix), WStringLiteral(*names + 1) + ","});
            _alldata.push_back(DataStructure("Strings in " + name, *names, StringSize(*names)));
        }
    }
    ++names; // skip last null pointer

    ds.setEnd(names);
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Names of dead keys" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static DEADKEY_LPWSTR " << name << "[] = {" << std::endl;
    grid.setMargin(4);
    grid.print(_ou);
    _ou << "    NULL" << std::endl << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::genScanToVk(const USHORT* vk, size_t vk_count, const WString& name)
{
    DataStructure ds(name, vk, vk_count * sizeof(*vk));
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Scan code to virtual key conversion table" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static USHORT " << name << "[] = {" << std::endl;
 
    for (size_t i = 0; i < vk_count; ++i) {
        _ou << Format(L"    /* %02X */ ", i) << attributes(vk_symbols, vk_flags_symbols, vk[i], 4) << "," << std::endl;
    }

    _ou << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::genVscToVk(const VSC_VK* vtvk, const WString& name, const WString& comment)
{
    DataStructure ds(name, vtvk);

    Grid grid;
    for (; vtvk->Vsc != 0; vtvk++) {
        grid.addLine({
            L"{" + Format(L"0x%02X", vtvk->Vsc) + L",",
            attributes(vk_symbols, vk_flags_symbols, vtvk->Vk, 4) + "},"
        });
    }
    grid.addLine({L"{0x00,", L"0x0000}"});
    vtvk++;

    ds.setEnd(vtvk);
    _alldata.push_back(ds);

    _ou << "//" << _opt.dashed << std::endl
        << "// Scan code to virtual key conversion table" << comment << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static VSC_VK " << name << "[] = {" << std::endl;
    grid.setMargin(4);
    grid.print(_ou);
    _ou << "};" << std::endl << std::endl;
}

//---------------------------------------------------------------------------

void SourceGenerator::generate(const KBDTABLES& tables)
{
    // Keyboard type are typically lower than 42. The field dwType was not used in older
    // versions and may contain crap. Try to guess a realistic value for keyboard type.
    // The last default keyord type is 4 (classical 101/102-key keyboard).
    const int kbd_type = _opt.kbd_type > 0 ? _opt.kbd_type : (tables.dwType > 0 && tables.dwType < 48 ? tables.dwType : 4);

    // File header.
    if (_opt.headers.empty()) {
        _ou << "//" << _opt.dashed << std::endl
            << "// " << _opt.comment << std::endl
            << "// Automatically generated from " << FileName(_opt.input) << std::endl
            << "//" << _opt.dashed << std::endl;
    }
    else {
        for (const auto& line : _opt.headers) {
            _ou << line << std::endl;
        }
    }
    _ou << std::endl
        << "#define KBD_TYPE " << kbd_type << std::endl
        << std::endl
        << "#include <windows.h>" << std::endl
        << "#include <kbd.h>" << std::endl
        << "#include <dontuse.h>" << std::endl;
    if (!_opt.num_only) {
        _ou << "#include \"unicode.h\"" << std::endl;
    }
    _ou << std::endl;

    const WString key_names_name(L"key_names");
    if (tables.pKeyNames != nullptr) {
        genVscToString(tables.pKeyNames, key_names_name);
    }

    const WString key_names_ext_name(L"key_names_ext");
    if (tables.pKeyNamesExt != nullptr) {
        genVscToString(tables.pKeyNamesExt, key_names_ext_name, L" (extended keypad)");
    }

    const WString key_names_dead_name(L"key_names_dead");
    if (tables.pKeyNamesDead != nullptr) {
        genKeyNames(tables.pKeyNamesDead, key_names_dead_name);
    }

    const WString scancode_to_vk_name(L"scancode_to_vk");
    if (tables.pusVSCtoVK != nullptr) {
        genScanToVk(tables.pusVSCtoVK, tables.bMaxVSCtoVK, scancode_to_vk_name);
    }

    const WString scancode_to_vk_e0_name(L"scancode_to_vk_e0");
    if (tables.pVSCtoVK_E0 != nullptr) {
        genVscToVk(tables.pVSCtoVK_E0, scancode_to_vk_e0_name, L" (scancodes with E0 prefix)");
    }

    const WString scancode_to_vk_e1_name(L"scancode_to_vk_e1");
    if (tables.pVSCtoVK_E1 != nullptr) {
        genVscToVk(tables.pVSCtoVK_E1, scancode_to_vk_e1_name, L" (scancodes with E1 prefix)");
    }

    const WString char_modifiers_name(L"char_modifiers");
    if (tables.pCharModifiers != nullptr) {
        genCharModifiers(*tables.pCharModifiers, char_modifiers_name);
    }

    const WString vk_to_wchar_name(L"vk_to_wchar");
    if (tables.pVkToWcharTable != nullptr) {
        genVkToWchar(tables.pVkToWcharTable, vk_to_wchar_name, tables.pCharModifiers);
    }

    const WString dead_keys_name(L"dead_keys");
    if (tables.pDeadKey != nullptr) {
        genDeadKeys(tables.pDeadKey, dead_keys_name);
    }

    const WString ligatures_name(L"ligatures");
    if (tables.pLigature != nullptr) {
        genLgToWchar(tables.pLigature, tables.nLgMax, tables.cbLgEntry, ligatures_name, tables.pCharModifiers);
    }

    // Generate main table.
    const WString kbd_table_name(L"kbd_tables");
    _alldata.push_back(DataStructure(kbd_table_name, &tables, sizeof(tables)));
    _ou << "//" << _opt.dashed << std::endl
        << "// Main keyboard layout structure, point to all tables" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "static KBDTABLES " << kbd_table_name << " = {" << std::endl
        << "    .pCharModifiers  = " << pointer(tables.pCharModifiers, L"&" + char_modifiers_name) << "," << std::endl
        << "    .pVkToWcharTable = " << pointer(tables.pVkToWcharTable, vk_to_wchar_name) << "," << std::endl
        << "    .pDeadKey        = " << pointer(tables.pDeadKey, dead_keys_name) << "," << std::endl
        << "    .pKeyNames       = " << pointer(tables.pKeyNames, key_names_name) << "," << std::endl
        << "    .pKeyNamesExt    = " << pointer(tables.pKeyNamesExt, key_names_ext_name) << "," << std::endl
        << "    .pKeyNamesDead   = " << pointer(tables.pKeyNamesDead, key_names_dead_name) << "," << std::endl
        << "    .pusVSCtoVK      = " << pointer(tables.pusVSCtoVK, scancode_to_vk_name) << "," << std::endl
        << "    .bMaxVSCtoVK     = " << (tables.pusVSCtoVK == nullptr ? L"0," : "ARRAYSIZE(" + scancode_to_vk_name + "),") << std::endl
        << "    .pVSCtoVK_E0     = " << pointer(tables.pVSCtoVK_E0, scancode_to_vk_e0_name) << "," << std::endl
        << "    .pVSCtoVK_E1     = " << pointer(tables.pVSCtoVK_E1, scancode_to_vk_e1_name) << "," << std::endl
        << "    .fLocaleFlags    = " << localeFlags(tables.fLocaleFlags) << "," << std::endl
        << "    .nLgMax          = " << int(tables.nLgMax) << "," << std::endl
        << "    .cbLgEntry       = " << (tables.pLigature == nullptr ? L"0," : "sizeof(" + ligatures_name + "[0]),") << std::endl
        << "    .pLigature       = " << pointer(tables.pLigature, L"(PLIGATURE1)" + ligatures_name) << "," << std::endl
        << "    .dwType          = " << tables.dwType << "," << std::endl
        << "    .dwSubType       = " << tables.dwSubType << "," << std::endl
        << "};" << std::endl
        << std::endl
        << "//" << _opt.dashed << std::endl
        << "// Keyboard layout entry point" << std::endl
        << "//" << _opt.dashed << std::endl
        << std::endl
        << "__declspec(dllexport) PKBDTABLES " KBD_DLL_ENTRY_NAME "(void)" << std::endl
        << "{" << std::endl
        << "    return &" << kbd_table_name << ";" << std::endl
        << "}" << std::endl;

    // Dump file content.
    if (_opt.hexa_dump) {
        genHexaDump();
    }
}

//---------------------------------------------------------------------------

void SourceGenerator::genHexaDump()
{
    // Rearrange, merge, describe inter-structure spaces, etc.
    sortDataStructures();

    // Get system page size.
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    const size_t page_size = size_t(sysinfo.dwPageSize);

    const uintptr_t first_address = uintptr_t(_alldata.front().address);
    const uintptr_t last_address = uintptr_t(_alldata.back().end());
    const uintptr_t first_page = first_address - first_address % page_size;
    const uintptr_t last_page = last_address + (page_size - last_address % page_size) % page_size;

    _ou << std::endl
        << "//" << _opt.dashed << std::endl
        << "// Data structures dump" << std::endl
        << "//" << _opt.dashed << std::endl
        << "//" << std::endl
        << "// Total size: " << (last_page - first_page) << " bytes (" << ((last_page - first_page) / page_size) << " pages)" << std::endl
        << Format(L"// Base: 0x%08llX", size_t(first_page)) << std::endl
        << Format(L"// End:  0x%08llX", size_t(last_page)) << std::endl;

    // Dump start of memory page, before the first data structure.
    if (first_page < first_address) {
        const DataStructure ds(L"Start of memory page before first data structure", first_page, first_address - first_page);
        ds.dump(_ou);
    }

    // Dump all data structures.
    for (const auto& data : _alldata) {
        data.dump(_ou);
    }

    // Dump end of memory page after last structure.
    if (last_address < last_page) {
        const DataStructure ds(L"End of memory page after last data structure", last_address, last_page - last_address);
        ds.dump(_ou);
    }
}


//---------------------------------------------------------------------------
// Generate the partial resource file for WKL project.
//---------------------------------------------------------------------------

void GenerateResourceFile(ReverseOptions& opt, HMODULE hmod)
{
    // Extract file information from the file.
    FileVersionInfo info(opt);
    if (!info.load(hmod)) {
        opt.fatal("Error loading version information from " + opt.input);
    }

    // This is the information we need for the resource file.
    WString wkl_text(info.LayoutText);
    WString wkl_lang(info.BaseLanguage);

    // All possible matches from registry.
    WStringVector ids;
    WStringVector texts;

    // These strings are not empty if we reverse a WKL keyboard layout DLL.
    // Otherwise, look for the information somewhere else.
    if (wkl_text.empty() || wkl_lang.empty()) {

        // Get DLL name.
        const WString dllname(ToLower(FileName(opt.input)));

        // Enumerate keyboard layouts in registry to find an entry matching the DLL name.
        // Some DLL's are registered several times, scan all entries.
        Registry reg(opt);
        WStringList all_lang_ids;
        if (reg.getSubKeys(REGISTRY_LAYOUT_KEY, all_lang_ids)) {
            for (const auto& id : all_lang_ids) {
                // The base language is the last 4 hexa digits in layout id. need at least 4 chars.
                if (id.size() >= 4 && ToLower(reg.getValue(REGISTRY_LAYOUT_KEY "\\" + id, REGISTRY_LAYOUT_FILE, L"", true)) == dllname) {
                    // This entry matches the DLL we search.
                    // Search text in values "Layout Display Name" and "Layout Text".
                    WString text(reg.getValue(REGISTRY_LAYOUT_KEY "\\" + id, REGISTRY_LAYOUT_DISPLAY, L"", true));
                    if (text.empty() || text[0] == L'@') {
                        text = reg.getValue(REGISTRY_LAYOUT_KEY "\\" + id, REGISTRY_LAYOUT_TEXT, L"", true);
                    }
                    ids.push_back(id);
                    texts.push_back(text);
                }
            }
        }
        if (wkl_lang.empty() && ids.empty()) {
            opt.fatal("unable to identify the base language for " + opt.input);
        }

        if (wkl_lang.empty()) {
            // The language is not known, keep the entry with the shortest (non empty) description.
            // When there are multiple entries, the shortest description is usually the base one.
            // What would be the right strategy?
            size_t index = 0;
            size_t desc_length = WString::npos;
            for (size_t i = 0; i < ids.size(); i++) {
                const size_t len = texts[i].length();
                if (len > 0 && len < desc_length) {
                    index = i;
                    desc_length = len;
                }
            }
            wkl_lang = ids[index].substr(ids[index].size() - 4);
            if (wkl_text.empty()) {
                wkl_text = texts[index];
            }
        }
        else {
            // The base language is already known, find a matching description.
            for (size_t i = 0; wkl_text.empty() && i < ids.size(); i++) {
                if (EndsWith(ToLower(wkl_lang), ToLower(ids[i]))) {
                    wkl_text = texts[i];
                }
            }
        }

        // If still nothing for description, fallback to version info.
        if (wkl_text.empty()) {
            wkl_text = info.FileDescription;
        }
    }

    // Content of the resource file.
    opt.out() << "#define WKL_TEXT \"" << wkl_text << "\"" << std::endl;
    opt.out() << "#define WKL_LANG \"" << wkl_lang << "\"" << std::endl;
    if (ids.size() > 1) {
        opt.out() << std::endl << "// Other possible matching entries:" << std::endl;
        for (size_t i = 0; i < ids.size(); i++) {
            opt.out() << "// " << ids[i] << ": \"" << texts[i] << "\"" << std::endl;
        }
    }
}


//---------------------------------------------------------------------------
// Generate a character table for the keyboard DLL.
//---------------------------------------------------------------------------

void GenerateCharacterTableLine(Grid& grid, uint16_t sc, const VirtualKey& vk, bool extended)
{
    if (sc != 0 && vk.vk != 0) {
        const auto it = vk_symbols.find(vk.vk);
        grid.addLine({
            Format(L"%02X%s", sc, extended ? L" (ext)" : L""),
            it != vk_symbols.end() ? it->second : Format(L"%02X", vk.vk)
            });
        for (wchar_t c : vk.wc) {
            grid.addColumn(c < L' ' || c == UC_DEL ? L"" : WString(1, c));
        }
    }
}

void GenerateCharacterTable(ReverseOptions& opt, const KBDTABLES* tables)
{
    // Header lines.
    Grid grid;
    Grid::Line header{ L"Scan code", L"Virtual key" };
    header.insert(header.end(), modifiers_headers.begin(), modifiers_headers.end());
    grid.addLine(header);
    grid.addUnderlines();

    // List of characters.
    WinKeyMap kmap(tables);
    WinKeyVector keys;
    kmap.buildKeyMap(keys);

    for (const auto& wk : keys) {
        GenerateCharacterTableLine(grid, wk.sc, wk.vk, false);
        GenerateCharacterTableLine(grid, wk.sc, wk.evk, true);
    }

    // Remove unused spaces.
    grid.removeEmptyColumns(2, true);
    grid.removeEmptyLines(2, true);

    // Print the grid.
    grid.setSpacing(2);
    opt.out() << UTF8_BOM;
    grid.print(opt.out());
}


//---------------------------------------------------------------------------
// Generate a keyboard map for the keyboard DLL.
//---------------------------------------------------------------------------

void GenerateKeyboardMap(ReverseOptions& opt, const KBDTABLES* tables)
{
    // Open the map template file.
    std::ifstream inmap(opt.map_template);
    if (!inmap) {
        opt.fatal("error opening file " + opt.map_template);
    }

    // Get lists of characters.
    WinKeyMap kmap(tables);
    WinKeyVector keys;
    kmap.buildKeyMap(keys);

    // Read map template line by line and generate the map..
    opt.out() << UTF8_BOM;
    std::string mapline;
    for (size_t linenum = 1; std::getline(inmap, mapline); ++linenum) {
        if (mapline.find_first_of("0123456789abcdefABCEDF") == std::string::npos) {
            // No scancode hexa value, just copy the line
            opt.out() << mapline << std::endl;
        }
        else {
            // Format a double line replacing the scancodes with the generated characters.
            WString line1, line2;
            static const wchar_t* const cellchars = L" 0123456789abcdefABCEDF";
            static const wchar_t* const hexachars = cellchars + 1;
            const WString in(ToUTF16(mapline));
            size_t start = 0;
            size_t end = 0;

            // Loop on all cells containing an hexa scancode.
            while ((start = in.find_first_of(hexachars, end)) != WString::npos) {
                // Locate start of cell.
                while (start > end && in[start - 1] == L' ') {
                    --start;
                }

                // Copy raw characters before cell.
                line1.append(in.substr(end, start - end));
                line2.append(in.substr(end, start - end));

                // Locate end of cell.
                end = in.find_first_not_of(cellchars, start);
                if (end == WString::npos) {
                    end = in.size();
                }
                const size_t width = end - start;

                // Locate hexa char inside cell.
                size_t hex = in.find_first_of(hexachars, start);
                assert(hex < end);
                size_t scancode = 0;
                if (hex + 2 >= end || !FromHexa(scancode, in.substr(hex, 2))) {
                    opt.error(Format(L"invalid cell \"%s\" in %s, line %d, col %d", in.substr(start, width).c_str(), opt.map_template.c_str(), linenum, start + 1));
                    end = start;
                    break;
                }
                const bool extended = hex + 2 < end && in[hex + 2] == L'e';

                // Find corresponding character definition.
                bool known = scancode < keys.size() && keys[scancode].sc != 0;
                const VirtualKey& vk(extended ? keys[scancode].evk : keys[scancode].vk);
                known = known && vk.vk != 0;

                // Format chars.
                const WString left(width == 2 ? 0 : (width - 3) / 2, L' ');
                const WString right(width == 2 ? 0 : width - left.size() - 3, L' ');
                line1.append(left);
                line2.append(left);
                line1.push_back(known ? Printable(vk.wc[KBDSHIFT]) : L' ');
                line2.push_back(known ? Printable(vk.wc[KBDBASE]) : L' ');
                line1.append(width == 2 ? L"" : L" ");
                line2.append(width == 2 ? L"" : L" ");
                line1.push_back(known ? Printable(vk.wc[KBDSHIFT | KBDCTRL | KBDALT]) : L' ');
                line2.push_back(known ? Printable(vk.wc[KBDCTRL | KBDALT]) : L' ');
                line1.append(right);
                line2.append(right);
            }

            // Append end of template line.
            line1.append(in.substr(end));
            line2.append(in.substr(end));
            opt.out() << line1 << std::endl << line2 << std::endl;
        }
    }
}


//---------------------------------------------------------------------------
// Application entry point.
//---------------------------------------------------------------------------

int wmain(int argc, wchar_t* argv[])
{
    // Parse command line options.
    ReverseOptions opt(argc, argv);

    // Resolve keyboard DLL file name.
    if (opt.input.find_first_of(L":\\/.") == WString::npos) {
        // No separator, must be a keyboard name, not a DLL file name.
        opt.input = GetEnv(L"SYSTEMROOT", L"C:\\Windows") + L"\\System32\\kbd" + opt.input + L".dll";
    }
 
    // Load the DLL in our virtual memory space.
    HMODULE dll = LoadLibraryW(opt.input.c_str());
    if (dll == nullptr) {
        const DWORD err = GetLastError();
        opt.fatal(opt.input + ": " + ErrorText(err));
    }

    // Get the DLL entry point.
    FARPROC proc_addr = GetProcAddress(dll, KBD_DLL_ENTRY_NAME);
    if (proc_addr == nullptr) {
        const DWORD err = GetLastError();
        opt.fatal("cannot find " KBD_DLL_ENTRY_NAME " in " + opt.input + ": " + ErrorText(err));
    }

    // Call the entry point to get the keyboard tables.
    // The entry point profile is: PKBDTABLES KbdLayerDescriptor()
    PKBDTABLES tables = reinterpret_cast<PKBDTABLES(*)()>(proc_addr)();
    if (tables == nullptr) {
        opt.fatal(KBD_DLL_ENTRY_NAME "() returned null in " + opt.input);
    }

    // Open the output file when specified.
    opt.setOutput(opt.output);

    // Generate the source file.
    if (opt.gen_resources) {
        GenerateResourceFile(opt, dll);
    }
    else if (opt.gen_list) {
        GenerateCharacterTable(opt, tables);
    }
    else if (!opt.map_template.empty()) {
        GenerateKeyboardMap(opt, tables);
    }
    else {
        SourceGenerator gen(opt);
        gen.generate(*tables);
    }
    opt.exit(EXIT_SUCCESS);
}
