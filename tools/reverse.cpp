//---------------------------------------------------------------------------
// Utility to analyze an installed keyboard layout DLL.
// Syntax:
//   reverse full-path-of-dll-file
//   reverse name   (DLL is C:\Windows\System32|kbdNAME.dll)
//---------------------------------------------------------------------------

#include <windows.h>
#include <kbd.h>
#if defined(min)
    #undef min
#endif
#if defined(max)
    #undef max
#endif
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <map>

#define KBD_DLL_ENTRY_NAME "KbdLayerDescriptor"

typedef __int64 Value;
typedef std::map<Value, std::string> SymbolTable;
#define SYM(e) {e, #e}


//----------------------------------------------------------------------------
// Command line options.
//----------------------------------------------------------------------------

class Options
{
public:
    // Constructor.
    Options(int argc, char* argv[]);

    // Command line options.
    std::string command;
    std::string input;
    std::string output;
    bool num_only;

    // Print help and exits.
    [[noreturn]] void usage() const;

    // Print a fatal error and exit.
    [[noreturn]] void fatal(const std::string& message) const;
};

[[noreturn]] void Options::usage() const
{
    std::cerr << std::endl
        << "Syntax: " << command << " [options] kbd-name-or-file" << std::endl
        << std::endl
        << "  kbd-name-or-file : Either the file name of a keyboard layout DLL or the" << std::endl
        << "  name of a keyboard layout, for instance \"fr\" for C:\\Windows\\System32\\kbdfr.dll." << std::endl
        << std::endl
        << "Options:" << std::endl
        << std::endl
        << "  -h : display this help text" << std::endl
        << "  -n : numerical output only, do not attempt to translate to source macros." << std::endl
        << "  -o file : output file name, default is standard output" << std::endl
        << std::endl;
    ::exit(EXIT_FAILURE);
}

[[noreturn]] void Options::fatal(const std::string& message) const
{
    std::cerr << command << ": " << message << std::endl;
    ::exit(EXIT_FAILURE);
}

Options::Options(int argc, char* argv[]) :
    command(argc < 1 ? "" : argv[0]),
    input(),
    output(),
    num_only(false)
{
    // Cleanup command name.
    size_t pos = command.find_last_of(":/\\");
    if (pos != std::string::npos) {
        command.erase(0, pos + 1);
    }
    pos = command.find_last_of(".");
    if (pos != std::string::npos) {
        command.resize(pos);
    }

    // Parse arguments.
    for (int i = 1; i < argc; ++i) {
        const std::string arg(argv[i]);
        if (arg == "--help" || arg == "-h") {
            usage();
        }
        else if (arg == "-o" && i + 1 < argc) {
            output = argv[++i];
        }
        else if (arg == "-n") {
            num_only = true;
        }
        else if (!arg.empty() && arg.front() != '-' && input.empty()) {
            input = arg;
        }
        else {
            fatal("invalid option '" + arg + "', try --help");
        }
    }
    if (input.empty()) {
        fatal("no keyboard layout specified, try --help");
    }
}


//---------------------------------------------------------------------------
// Format a C++ string in a printf-way.
//---------------------------------------------------------------------------

std::string Format(const char* fmt, ...)
{
    va_list ap;

    // Get required output size.
    va_start(ap, fmt);
    int len = ::vsnprintf(nullptr, 0, fmt, ap);
    va_end(ap);

    if (len < 0) {
        return std::string(); // error
    }

    // Actual formatting.
    std::string buf(len + 1, '\0');
    va_start(ap, fmt);
    len = ::vsnprintf(&buf[0], buf.size(), fmt, ap);
    va_end(ap);

    buf.resize(std::max(0, len));
    return buf;
}


//----------------------------------------------------------------------------
// Transform an error code into an error message string.
//----------------------------------------------------------------------------

std::string Error(::DWORD code = ::GetLastError())
{
    std::string message(1024, ' ');
    size_t length = ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, 0, &message[0], ::DWORD(message.size()), nullptr);
    message.resize(std::min(length, message.size()));

    if (!message.empty()) {
        return message;
    }
    else {
        // Message is not found.
        return Format("System error %d (0x%X)", code, code);
    }
}


//---------------------------------------------------------------------------
// Get the value of an environment variable.
//---------------------------------------------------------------------------

std::string GetEnv(const std::string& name, const std::string& def = "")
{
    std::string value(1024, ' ');
    ::DWORD size = ::GetEnvironmentVariableA(name.c_str(), &value[0], ::DWORD(value.size()));
    if (size >= ::DWORD(value.size())) {
        value.resize(size_t(size + 1));
        size = ::GetEnvironmentVariableA(name.c_str(), &value[0], ::DWORD(value.size()));
    }
    value.resize(std::max<size_t>(0, std::min<size_t>(value.size(), size)));
    return value.empty() ? def : value;
}


//---------------------------------------------------------------------------
// Format an integer as a decimal or hexadecimal string.
// If hex_digits is zero, format in decimal.
//---------------------------------------------------------------------------

std::string Integer(Value value, int hex_digits = 0)
{
    return hex_digits <= 0 ? Format("%lld", value) : Format("0x%0*lld", hex_digits, value);
}


//---------------------------------------------------------------------------
// Format an integer as a string, using a table of symbols.
// If no symbol found or option -n, return a number.
// If hex_digits is zero, format in decimal.
//---------------------------------------------------------------------------

std::string Symbol(const Options& opt, const SymbolTable& symbols, Value value, int hex_digits = 0)
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

std::string BitMask(const Options& opt, const SymbolTable& symbols, Value value, int hex_digits = 0)
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

std::string Attributes(const Options& opt, const SymbolTable& symbols, const SymbolTable& attributes, Value value, int hex_digits = 0)
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
    SYM(VK_OEM_CLEAR)
};


//---------------------------------------------------------------------------
// Generate various part of the source file.
//---------------------------------------------------------------------------

void GenerateVkToBits(const Options& opt, std::ostream& out, const ::VK_TO_BIT* vtb, const std::string& name)
{
    out << "static VK_TO_BIT " << name << "[] = {" << std::endl;
    for (; vtb->Vk != 0; vtb++) {
        out << "    {" << Symbol(opt, vk_symbols, vtb->Vk, 2) << ", " << BitMask(opt, shift_state_symbols, vtb->ModBits, 4) << "}," << std::endl;
    }
    out << "    {0, 0}" << std::endl 
        << "};" << std::endl 
        << std::endl;
}

//---------------------------------------------------------------------------

void GenerateCharModifiers(const Options& opt, std::ostream& out, const ::MODIFIERS& mods, const std::string& name)
{
    const char* vk_to_bits_name = "vk_to_bits";

    if (mods.pVkToBit != nullptr) {
        GenerateVkToBits(opt, out, mods.pVkToBit, vk_to_bits_name);
    }

    out << "static MODIFIERS " << name << " = {" << std::endl
        << "    .pVkToBit = " << (mods.pVkToBit != nullptr ? vk_to_bits_name : "nullptr") << "," << std::endl
        << "    .wMaxModBits = " << mods.wMaxModBits << "," << std::endl
        << "    .ModNumber = {" << std::endl;

    // At most 8 entriess in ModNumber.
    static const char* const mod_names[] = {
        "000 = <none>",
        "001 = SHIFT",
        "010 = ALT",
        "011 = SHIFT ALT",
        "100 = CTRL",
        "101 = SHIFT CTRL",
        "110 = CTRL ALT",
        "111 = SHIFT CTRL ALT"
    };

    for (::WORD i = 0; i < mods.wMaxModBits; ++i) {
        out << "        " << Symbol(opt, { SYM(SHFT_INVALID) }, mods.ModNumber[i]) << ",";
        if (i < ARRAYSIZE(mod_names)) {
            out << "  // " << mod_names[i];
        }
        out << std::endl;
    }

    out << "    }" << std::endl
        << "};" << std::endl
        << std::endl;
}

//---------------------------------------------------------------------------

void GenerateVkToWchar(const Options& opt, std::ostream& out, const ::VK_TO_WCHAR_TABLE* vtwc, const std::string& name)
{
    out << "static VK_TO_WCHAR_TABLE " << name << "[] = {" << std::endl;
    for (; vtwc->pVkToWchars != nullptr; vtwc++) {
        /* @@@@@@@@@@@@@@@@
        {(PVK_TO_WCHARS1)aVkToWch3, 3, sizeof(aVkToWch3[0])},
        { (PVK_TO_WCHARS1)aVkToWch4, 4, sizeof(aVkToWch4[0]) },
        { (PVK_TO_WCHARS1)aVkToWch5, 5, sizeof(aVkToWch5[0]) },
        { (PVK_TO_WCHARS1)aVkToWch2, 2, sizeof(aVkToWch2[0]) },
        { (PVK_TO_WCHARS1)aVkToWch1, 1, sizeof(aVkToWch1[0]) },
        */
    }
    out << "    {NULL, 0, 0}" << std::endl
        << "};" << std::endl
        << std::endl;
}

//---------------------------------------------------------------------------

std::string LocaleFlags(const Options& opt, ::DWORD flags)
{
    if (opt.num_only) {
        return Format("0x%08X", flags);
    }
    else {
        std::string lostr(BitMask(opt, { SYM(KLLF_ALTGR), SYM(KLLF_SHIFTLOCK), SYM(KLLF_LRM_RLM)}, flags & 0xFFFF, 4));
        std::string histr(Symbol(opt, { SYM(KBD_VERSION) }, (flags >> 16) & 0xFFFF, 4));
        return "MAKELONG(" + lostr + ", " + histr + ")";
    }
}


//---------------------------------------------------------------------------
// Starting point of the source file creation.
//---------------------------------------------------------------------------

void GenerateSourceFile(const Options& opt, std::ostream& out, const ::KBDTABLES& tables)
{
    out << "// Automatically generated from " << opt.input << std::endl
        << std::endl
        << "#define KBD_TYPE " << (tables.dwType > 0 ? tables.dwType : 4) << std::endl
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

    const std::string kbd_table_name = "kbd_tables";
    out << "static KBDTABLES " << kbd_table_name << " = {" << std::endl
        << "    .pCharModifiers = " << (tables.pCharModifiers != nullptr ? "&" + char_modifiers_name : "nullptr") << "," << std::endl
        << "    .pVkToWcharTable = " << (tables.pVkToWcharTable != nullptr ? vk_to_wchar_name : "nullptr") << "," << std::endl
        << "    .pDeadKey = NULL," << std::endl
        << "    .pKeyNames = NULL," << std::endl
        << "    .pKeyNamesExt = NULL," << std::endl
        << "    .pKeyNamesDead = NULL," << std::endl
        << "    .pusVSCtoVK = NULL," << std::endl
        << "    .bMaxVSCtoVK = 0, // ARRAYSIZE(ausVK)," << std::endl
        << "    .pVSCtoVK_E0 = 0," << std::endl
        << "    .pVSCtoVK_E1 = 0," << std::endl
        << "    .fLocaleFlags = " << LocaleFlags(opt, tables.fLocaleFlags) << "," << std::endl
        << "    .nLgMax = 0," << std::endl
        << "    .cbLgEntry = 0," << std::endl
        << "    .pLigature = NULL," << std::endl
        << "    .dwType = " << tables.dwType << "," << std::endl
        << "    .dwSubType = " << tables.dwSubType << "," << std::endl
        << "};" << std::endl
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
    Options opt(argc, argv);

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
