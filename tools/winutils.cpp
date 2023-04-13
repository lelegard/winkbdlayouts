//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Some Windows utilities.
//
//----------------------------------------------------------------------------

#include "winutils.h"
#include "strutils.h"


//----------------------------------------------------------------------------
// Transform an error code into an error message string.
//----------------------------------------------------------------------------

WString ErrorText(DWORD code)
{
    WString message(1024, ' ');
    size_t length = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, 0, &message[0], DWORD(message.size()), nullptr);
    message.resize(std::min(length, message.size()));

    while (!message.empty() && std::isspace(message.back())) {
        message.pop_back();
    }

    if (!message.empty()) {
        return message;
    }
    else {
        // Message is not found.
        return Format(L"System error %d (0x%X)", code, code);
    }
}


//----------------------------------------------------------------------------
// File name (without directory), file base name (without directory and prefix).
//----------------------------------------------------------------------------

WString FullName(const WString& name, bool include_dir, bool include_file)
{
    if (!include_dir && !include_file) {
        return WString();
    }

    WString path(1024, L'\0');
    wchar_t* file_part = nullptr;
    DWORD size = GetFullPathNameW(name.c_str(), DWORD(path.size()), &path[0], &file_part);
    if (size >= DWORD(path.size())) {
        path.resize(size_t(size + 1));
        size = GetFullPathNameW(name.c_str(), DWORD(path.size()), &path[0], &file_part);
    }

    if (!include_dir) {
        return file_part == nullptr ? WString() : file_part;
    }
    if (!include_file && file_part != nullptr) {
        path.resize(std::min<size_t>(path.size(), file_part - path.data()));
    }
    else {
        path.resize(std::min<size_t>(path.size(), size));
    }
    return path;
}

WString DirName(const WString& name)
{
    return FullName(name, true, false);
}

WString FileName(const WString& name)
{
    return FullName(name, false, true);
}

WString FileBaseName(const WString& name)
{
    const WString filename(FileName(name));
    const size_t pos = filename.rfind(L'.');
    return pos == std::string::npos ? filename : filename.substr(0, pos);
}


//----------------------------------------------------------------------------
// Check if a file or directory exists
//----------------------------------------------------------------------------

bool FileExists(const WString& path)
{
    return GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES;
}

bool IsDirectory(const WString& path)
{
    const DWORD attr = GetFileAttributesW(path.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}


//---------------------------------------------------------------------------
// Search files matching a wildcard.
//---------------------------------------------------------------------------

bool SearchFiles(WStringList& files, const WString& directory, const WString& pattern)
{
    files.clear();

    // Initiate the search
    const WString full_pattern(directory + L"\\" + pattern);
    WIN32_FIND_DATAW fdata;
    HANDLE handle = FindFirstFileW(full_pattern.c_str(), &fdata);
    if (handle == INVALID_HANDLE_VALUE) {
        // No file matching the pattern is not an error
        const DWORD err = GetLastError();
        return err == ERROR_SUCCESS || err == ERROR_FILE_NOT_FOUND;
    }

    // Loop on all file matching the pattern
    do {
        // Get next file name.
        fdata.cFileName[sizeof(fdata.cFileName) / sizeof(fdata.cFileName[0]) - 1] = 0;
        const WString file(fdata.cFileName);

        // Filter out . and ..
        if (file != L"." && file != L"..") {
            files.push_back(file);
        }
    } while (FindNextFileW(handle, &fdata) != 0);
    const DWORD err = GetLastError(); // FindNextFile status

    // Cleanup the search context
    FindClose(handle);
    return err == ERROR_SUCCESS || err == ERROR_NO_MORE_FILES; // normal end of search
}


//---------------------------------------------------------------------------
// Get the value of an environment variable.
//---------------------------------------------------------------------------

WString GetEnv(const WString& name, const WString& def)
{
    WString value(2048, ' ');
    DWORD size = GetEnvironmentVariableW(name.c_str(), &value[0], DWORD(value.size()));
    if (size >= DWORD(value.size())) {
        value.resize(size_t(size + 1));
        size = GetEnvironmentVariableW(name.c_str(), &value[0], DWORD(value.size()));
    }
    value.resize(std::min<size_t>(value.size(), size));
    return value.empty() ? def : value;
}


//---------------------------------------------------------------------------
// Get the file name of a module in a process.
//---------------------------------------------------------------------------

WString ModuleFileName(HANDLE process, HMODULE module)
{
    WString name(2048, ' ');
    DWORD size = GetModuleFileNameExW(process, module, &name[0], DWORD(name.size()));
    name.resize(std::min<size_t>(size, name.size()));
    return name;
}


//---------------------------------------------------------------------------
// Get a resource string in a module.
//---------------------------------------------------------------------------

WString GetResourceString(const WString& filename, int resource_index)
{
    WString str;
    HMODULE hmod = LoadLibraryExW(filename.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE);
    if (hmod != nullptr) {
        str = GetResourceString(hmod, resource_index);
        FreeLibrary(hmod);
    }
    return str;
}

WString GetResourceString(HMODULE module, int resource_index)
{
    // Start with a 1kB buffer and iterate with double size as long as the
    // value seems to be longer. Stop at 1MB (fool-proof check).
    WString value(1024, L'\0');
    int length = 0;
    while ((length = LoadStringW(module, resource_index, &value[0], int(value.size()))) >= int(value.size()) - 1 && value.size() < 1000000) {
        value.resize(2 * value.size());
    }
    value.resize(std::min<size_t>(value.size(), std::max(0, length)));
    return value;
}


//---------------------------------------------------------------------------
// Check if current process is admin.
//---------------------------------------------------------------------------

bool IsAdmin()
{
    // Allocate and initialize an SID of the administrators group.
    SID_IDENTIFIER_AUTHORITY nt_auth = SECURITY_NT_AUTHORITY;
    PSID admin_group = nullptr;
    if (!AllocateAndInitializeSid(&nt_auth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admin_group)) {
        const DWORD err = GetLastError();
        std::cerr << "AllocateAndInitializeSid error: " << ErrorText(err) << std::endl;
        return false;
    }

    // Check if the SID of administrators group is enabled in the primary access token of the process.
    BOOL is_admin = false;
    if (!CheckTokenMembership(nullptr, admin_group, &is_admin)) {
        const DWORD err = GetLastError();
        std::cerr << "CheckTokenMembership error: " << ErrorText(err) << std::endl;
        return false;
    }

    // Free allocated SID.
    FreeSid(admin_group);
    return is_admin;
}


//---------------------------------------------------------------------------
// Restart current program as admin.
//---------------------------------------------------------------------------

bool RestartAsAdmin(const WStringVector& args, bool wait_process)
{
    const WString program(GetCurrentProgram());
    const WString all_args(Join(args, L" "));

    SHELLEXECUTEINFOW sei;
    Zero(&sei, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.fMask = wait_process ? (SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NOASYNC) : 0;
    sei.lpVerb = L"runas";
    sei.lpFile = program.c_str();
    sei.lpParameters = args.empty() ? nullptr : all_args.c_str();
    sei.nShow = SW_NORMAL;

    // Initialize COM, if not already done. Required for ShellExecute.
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (!ShellExecuteExW(&sei)) {
        const DWORD err = GetLastError();
        if (err != ERROR_CANCELLED) {
            // Real error, not user cancelled UAC.
            std::cerr << "ShellExecuteEx error: " << ErrorText(err) << std::endl;
        }
        return false;
    }

    // Wait for process completion.
    if (wait_process) {
        WaitForSingleObject(sei.hProcess, INFINITE);
    }
    return true;
}


//---------------------------------------------------------------------------
// Setting the Windwos console (DOS or PowerShell) to UTF-8 mode.
//---------------------------------------------------------------------------

ConsoleState::ConsoleState() :
    _input_cp(GetConsoleCP()),
    _output_cp(GetConsoleOutputCP())
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
}

ConsoleState::~ConsoleState()
{
    SetConsoleCP(_input_cp);
    SetConsoleOutputCP(_output_cp);
}
