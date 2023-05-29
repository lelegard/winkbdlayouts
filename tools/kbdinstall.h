//----------------------------------------------------------------------------
//
// Windows Keyboards Layouts (WKL)
// Copyright (c) 2023, Thierry Lelegard
// BSD-2-Clause license, see the LICENSE file.
//
// Keyboard layout DLL installation and uninstallation.
//
//----------------------------------------------------------------------------

#pragma once
#include "error.h"

// Install a keyboard layout DLL.
// The specified DLL is copied into %SystemRoot%\System32.
// The base_language is a 16-bit value, for instance 0x0409 for English.
// The description string will identify the keyboard in the system settings.
// Return the allocated complete language id or zero on error.
uint32_t InstallKeyboardLayout(Error& err, const WString& dll, uint16_t base_language, const WString& description);

// Uninstall a keyboard layout DLL.
// Only the file name part of the DLL is used.
// The DLL is deleted from %SystemRoot%\System32 and all registrations are removed.
// Return true on success, false on error.
bool UninstallKeyboardLayout(Error& err, const WString& dll);

// Install a keyboard layout DLL from the WKL project.
// These keyboard DLL are supposed to contain resource strings to indicate the language and description.
// The specified DLL is copied into %SystemRoot%\System32.
// Return the allocated complete language id or zero on error.
uint32_t WKLInstallKeyboardLayout(Error& err, const WString& dll);

// Install one or more keyboard layout DLL's from the WKL project.
// When a path is a directory, install all kbd*.dll from that directory.
// Return true on success, false on error.
bool WKLInstallAllKeyboardLayouts(Error& err, const WStringVector& paths);

// Uninstall all WKL keyboard layout DLL's.
// Return true on success, false on error.
bool WKLUninstallAllKeyboardLayouts(Error& err);
