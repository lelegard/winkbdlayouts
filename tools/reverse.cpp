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
#include <algorithm>
#include <vector>
#include <string>

#define ENTRY_NAME           "KbdLayerDescriptor"
#define KBD_TABLE_NAME       "kbd_tables"
#define CHAR_MODIFIERS_NAME  "char_modifiers"
#define VK_TO_BITS_NAME      "vk_to_bits"
#define VK_TO_WCHAR_NAME     "vk_to_wchar"


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
// Generate the source file.
//---------------------------------------------------------------------------

void GenerateVkToBits(std::ostream& out, const ::VK_TO_BIT* vtb)
{
    out << "static VK_TO_BIT " VK_TO_BITS_NAME "[] = {" << std::endl;
    for (; vtb->Vk != 0; vtb++) {
        out << "    {" << int(vtb->Vk) << ", " << int(vtb->ModBits) << "}," << std::endl;
    }
    out << "    {0, 0}" << std::endl 
        << "};" << std::endl 
        << std::endl;
}

//---------------------------------------------------------------------------

void GenerateCharModifiers(std::ostream& out, const ::MODIFIERS& mods)
{
    if (mods.pVkToBit != nullptr) {
        GenerateVkToBits(out, mods.pVkToBit);
    }

    out << "static MODIFIERS " CHAR_MODIFIERS_NAME " = {" << std::endl
        << "    .pVkToBit = " << (mods.pVkToBit != nullptr ? VK_TO_BITS_NAME : "nullptr") << "," << std::endl
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
        out << "        " << int(mods.ModNumber[i]) << ",";
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

void GenerateVkToWchar(std::ostream& out, const ::VK_TO_WCHAR_TABLE* vtwc)
{
    out << "static VK_TO_WCHAR_TABLE " VK_TO_WCHAR_NAME "[] = {" << std::endl;
    for (; vtwc->pVkToWchars != nullptr; vtwc++) {
        /*
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

void GenerateSourceFile(std::ostream& out, const ::KBDTABLES& tables, const std::string& dll_file)
{
    out << "// Automatically generated from " << dll_file << std::endl
        << std::endl
        << "#define KBD_TYPE " << (tables.dwType > 0 ? tables.dwType : 4) << std::endl
        << std::endl
        << "#include <windows.h>" << std::endl
        << "#include <kbd.h>" << std::endl
        << "#include <dontuse.h>" << std::endl
        << std::endl;

    if (tables.pCharModifiers != nullptr) {
        GenerateCharModifiers(out, *tables.pCharModifiers);
    }
    if (tables.pVkToWcharTable != nullptr) {
        GenerateVkToWchar(out, tables.pVkToWcharTable);
    }

    out << "static KBDTABLES " KBD_TABLE_NAME " = {" << std::endl
        << "    .pCharModifiers = " << (tables.pCharModifiers != nullptr ? "&" CHAR_MODIFIERS_NAME : "nullptr") << "," << std::endl
        << "    .pVkToWcharTable = " << (tables.pVkToWcharTable != nullptr ? VK_TO_WCHAR_NAME : "nullptr") << "," << std::endl
        << "    .pDeadKey = NULL," << std::endl
        << "    .pKeyNames = NULL," << std::endl
        << "    .pKeyNamesExt = NULL," << std::endl
        << "    .pKeyNamesDead = NULL," << std::endl
        << "    .pusVSCtoVK = NULL," << std::endl
        << "    .bMaxVSCtoVK = 0, // ARRAYSIZE(ausVK)," << std::endl
        << "    .pVSCtoVK_E0 = 0," << std::endl
        << "    .pVSCtoVK_E1 = 0," << std::endl
        << "    .fLocaleFlags = " << Format("0x%08X", tables.fLocaleFlags) << "," << std::endl
        << "    .nLgMax = 0," << std::endl
        << "    .cbLgEntry = 0," << std::endl
        << "    .pLigature = NULL," << std::endl
        << "    .dwType = " << tables.dwType << "," << std::endl
        << "    .dwSubType = " << Format("0x%04X", tables.dwSubType) << "," << std::endl
        << "};" << std::endl
        << std::endl
        << "__declspec(dllexport) PKBDTABLES " ENTRY_NAME "(void)" << std::endl
        << "{" << std::endl
        << "    return &" KBD_TABLE_NAME ";" << std::endl
        << "}" << std::endl;
}


//---------------------------------------------------------------------------
// Application entry point.
//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Get DLL name.
    if (argc != 2) {
        std::cerr << "Syntax: " << argv[0] << " kbdfile.dll" << std::endl;
        return EXIT_FAILURE;
    }
    std::string dll_name(argv[1]);
    if (dll_name.find('.') == std::string::npos) {
        // No dot in file name, must be a keyboard name.
        dll_name = GetEnv("SYSTEMROOT", "C:\\Windows") + "\\System32\\kbd" + dll_name + ".dll";
    }

    // Load the DLL and get its entry point.
    ::HMODULE dll = ::LoadLibraryA(dll_name.c_str());
    if (dll == nullptr) {
        const ::DWORD err = ::GetLastError();
        std::cerr << "error opening " << dll_name << ": " << Error(err) << std::endl;
        return EXIT_FAILURE;
    }
    ::FARPROC proc_addr = ::GetProcAddress(dll, ENTRY_NAME);
    if (proc_addr == nullptr) {
        const ::DWORD err = ::GetLastError();
        std::cerr << "cannot find " ENTRY_NAME " in " << dll_name << ": " << Error(err) << std::endl;
        return EXIT_FAILURE;
    }

    // Call the entry point to get the keyboard tables.
    typedef ::PKBDTABLES (*KbdEntryPoint)();
    ::PKBDTABLES tables = reinterpret_cast<KbdEntryPoint>(proc_addr)();
    if (tables == nullptr) {
        std::cerr << ENTRY_NAME "() returned null in " << dll_name << std::endl;
        return EXIT_FAILURE;
    }

    // Keyboard tables are now identified, start generating the source file.
    GenerateSourceFile(std::cout, *tables, dll_name);
    return EXIT_SUCCESS;
}
