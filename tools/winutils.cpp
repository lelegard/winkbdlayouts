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
// File name (without directory), file base name (without directory and prefix).
//----------------------------------------------------------------------------

std::string FileName(const std::string& name)
{
    const size_t pos = name.find_last_of(":/\\");
    return pos == std::string::npos ? name : name.substr(pos + 1);
}

std::string FileBaseName(const std::string& name)
{
    const std::string filename(FileName(name));
    const size_t pos = filename.find_last_of(".");
    return pos == std::string::npos ? filename : filename.substr(0, pos);
}


//----------------------------------------------------------------------------
// Transform an error code into an error message string.
//----------------------------------------------------------------------------

std::string Error(::DWORD code)
{
    std::string message(1024, ' ');
    size_t length = ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, 0, &message[0], ::DWORD(message.size()), nullptr);
    message.resize(std::min(length, message.size()));

    while (!message.empty() && std::isspace(message.back())) {
        message.pop_back();
    }

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

std::string GetEnv(const std::string& name, const std::string& def)
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
// Get the file name of a module in a process.
//---------------------------------------------------------------------------

std::string ModuleFileName(::HANDLE process, ::HMODULE module)
{
    std::string name(2048, ' ');
    ::DWORD size = ::GetModuleFileNameExA(process, module, &name[0], ::DWORD(name.size()));
    name.resize(std::min<size_t>(size, name.size()));
    return name;
}


//---------------------------------------------------------------------------
// Check if current process is admin.
//---------------------------------------------------------------------------

bool IsAdmin()
{
    // Allocate and initialize an SID of the administrators group.
    ::SID_IDENTIFIER_AUTHORITY nt_auth = SECURITY_NT_AUTHORITY;
    ::PSID admin_group = nullptr;
    if (!::AllocateAndInitializeSid(&nt_auth, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admin_group)) {
        const ::DWORD err = ::GetLastError();
        std::cerr << "AllocateAndInitializeSid error: " << Error(err) << std::endl;
        return false;
    }

    // Check if the SID of administrators group is enabled in the primary access token of the process.
    ::BOOL is_admin = false;
    if (!::CheckTokenMembership(nullptr, admin_group, &is_admin)) {
        const ::DWORD err = ::GetLastError();
        std::cerr << "CheckTokenMembership error: " << Error(err) << std::endl;
        return false;
    }

    // Free allocated SID.
    ::FreeSid(admin_group);
    return is_admin;
}


//---------------------------------------------------------------------------
// Restart current program as admin.
//---------------------------------------------------------------------------

bool RestartAsAdmin(const StringVector& args, bool wait_process)
{
    const std::string program(GetCurrentProgram());
    const std::string all_args(Join(args, " "));

    ::SHELLEXECUTEINFOA sei;
    Zero(&sei, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.fMask = wait_process ? (SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NOASYNC) : 0;
    sei.lpVerb = "runas";
    sei.lpFile = program.c_str();
    sei.lpParameters = args.empty() ? nullptr : all_args.c_str();
    sei.nShow = SW_NORMAL;

    // Initialize COM, if not already done. Required for ShellExecute.
    ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (!::ShellExecuteExA(&sei)) {
        const ::DWORD err = ::GetLastError();
        if (err != ERROR_CANCELLED) {
            // Real error, not user cancelled UAC.
            std::cerr << "ShellExecuteEx error: " << Error(err) << std::endl;
        }
        return false;
    }

    // Wait for process completion.
    if (wait_process) {
        ::WaitForSingleObject(sei.hProcess, INFINITE);
    }
    return true;
}
