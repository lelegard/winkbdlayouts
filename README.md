# Windows Keyboards Layouts (WKL)

The WKL project collects the source code of a few keyboard layouts which are not,
or no longer, included by default in Windows distributions.

Specifically, at least on Windows 11, the Apple keyboards are not available
by default and need custom mappings. When Windows 11 is installed as a virtual
machine on a Mac, the hypervisor may include specific mappings for the Apple
keyboards. This is the case for Parallels but not UTM/Qemu. When the hypervisor
does not provide mapping for Mac keyboards, this project is useful.

**Contents:**

* [Installation](#installation)
* [Build instructions](#build-instructions)
* [Language support and contributions](#language-support-and-contributions)
  * [Initial steps: create the new directory](#initial-steps-create-the-new-directory)
  * [Option 1: duplicate a source file from this project](#option-1-duplicate-a-source-file-from-this-project)
  * [Option 2: reverse the content of an existing keyboard DLL](#option-2-reverse-the-content-of-an-existing-keyboard-dll)
  * [Final steps: add the project into the solution](#final-steps-add-the-project-into-the-solution)
* [Scan codes](#scan-codes)
  * [Apple keyboards](#apple-keyboards)
  * [Apple keyboards in Windows virtual machines](#apple-keyboards-in-windows-virtual-machines)
* [Declaring a keyboard layout on windows](#declaring-a-keyboard-layout-on-windows)

## Installation

If you simply want to install the Windows Keyboards Layouts, grab the latest
archive file in the "Release" area of this project, expand it and run the PowerShell
script named `install.ps1`.

Alternatively, you may run the `setup.exe` file in the appropriate subdirectory
`x86`, `x64` or `arm64` for your system.

## Build instructions

The project is built for x86, x64 and arm64 Windows systems. The released archive
contains binaries for the three architectures.

To build this project, make sure to install all build tools (compilers and libraries)
for all targets when installing or updating Visual Studio.

Available PowerShell scripts:
- `build.ps1` : Build the binaries for all architectures, build the distribution archive.
- `install.ps1` : Install the keyboards on the current system after rebuild.
- `clean.ps1` : Cleanup all generated files.

## Language support and contributions

New layouts are welcome as contributions. Please post a pull request with your
implementation of new layouts.

To add support for a new keyboard layout, start from an existing keyboard
which is similar to yours and manually update the source code. There are
two ways to clone an existing keyboard: from a source file in this project
or from the binary DLL of an installed keyboard.

### Initial steps: create the new directory

- In the directory `keyboards`, create a subdirectory with name `kbdXXYYY` where
  `XX` is the language code for your keyboard (not necessarily two letters, this is
  just a convention) and `YYY` is the type or brand of keyboard (for instance `apple`).

- Copy any project file from an existing keyboard, for instance `kbdfrapple\kbdfrapple.vcxproj`,
  as `kbdXXYYY\kbdXXYYY.vcxproj`. No need to update the content, Visual Studio
  will adjust the project GUID the first time it is used.

### Option 1: duplicate a source file from this project

Let's assume you start from `kbdfrapple`.

- Copy `kbdfrapple\kbdfrapple.c` as `kbdXXYYY\kbdXXYYY.c`.
- Copy `kbdfrapple\strings.h` as `kbdXXYYY\strings.h`.

### Option 2: reverse the content of an existing keyboard DLL

- Use the `kbdreverse` tool in this project to extract the layout definition
  of an installed keyboard and rebuild a source file from that keyboard.

Example: To rebuild source files for a French keyboard (id `fr`), use these commands:
~~~
kbdreverse fr -o kbdXXYYY\kbdXXYYY.c
kbdreverse fr -r -o kbdXXYYY\strings.h
~~~

Using the parameter `fr` means reversing the file `C:\Windows\System32\kbdfr.dll`.
To reverse a keyboard DLL from another location, specify the full path of the DLL file.

### Final steps: add the project into the solution

- Update the key tables in `kbdXXYYY\kbdXXYYY.c` according to your keyboard.

- In the file `strings.h`, the `WKL_LANG` string is the base language for which
  your keyboard is a variant. For instance, `040c` means French. If you do not
  know which id to choose, use the Registry Editor to browse through the existing
  keyboard layout ids at
  `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts`.
  The subkeys are 8-digit hexadecimal values for installed keyboards. The rightmost
  4 digits are the base language id.

- Before building the new keyboard for the first time, open `winkbdlayouts.sln` with
  Visual Studio, right-click on the solution => "Add" => "Existing Project" and
  select `kbdXXYYY.vcxproj`. Then, "File" => "Save All".

The new keyboard is now part of the solution and will be built with the rest of it.

## Scan codes

If you have difficulties to collect the scan codes for your keyboard, run the
`scancodes` tool from this project.

The image [tools\scancodes.jpg](tools/scancodes.jpg) shows the scan codes for
a standard 101/102-key PC American keyboard.

### Apple keyboards

There are some specificities on the bottom row of Apple keyboards.
~~~
+---------+--------+---------+-------------------------+---------+--------+---------+
| control | option | command |          space          | command | option | control |
|  0x1D   | 0x38 a | 0x5B e  |          0x39           | 0x5C e  | 0x38 e | 0x1D e  |
+---------+--------+---------+-------------------------+---------+--------+---------+
~~~
In the diagrams, the hexadecimal values are the scan codes of the keys,
`e` means "Extended" (Ctrl or Right-Alt) and `a` means "Alt".

And on the right-hand side of Apple keyboards:
~~~
+--------+--------+--------+    +--------+--------+--------+--------+
|  F13   |  F14   |  F15   |    |  F16   |  F17   |  F18   |  F19   |
| 0x37 e | (none) | (none) |    |  0x67  |  0x68  |  0x69  |  0x6A  |
+--------+--------+--------+    +--------+--------+--------+--------+
|   fn   |  Home  |Page Up |    |Num Lock|   =    |   /    |   *    |
| (none) | 0x47 e | 0x49 e |    | 0x45 e |  0x59  | 0x35 e |  0x37  |
+--------+--------+--------+    +--------+--------+--------+--------+
|  Del   |  End   |Page Dwn|    |   7    |   8    |   9    |   -    |
| 0x53 e | 0x4F e | 0x51 e |    |  0x47  |  0x48  |  0x49  |  0x4A  |
+--------+--------+--------+    +--------+--------+--------+--------+
                                |   4    |   5    |   6    |   +    |
                                |  0x4B  |  0x4C  |  0x4D  |  0x4E  |
         +--------+             +--------+--------+--------+--------+
         |   Up   |             |   1    |   2    |   3    |        |
         | 0x48 e |             |  0x4F  |  0x50  |  0x51  |        |
+--------+--------+--------+    +--------+--------+--------+ Enter  |
|  Left  |  Down  | Right  |    |        0        |   ,    | 0x1C e |
| 0x4B e | 0x50 e | 0x4D e |    |       0x52      |  0x53  |        |
+--------+--------+--------+    +--------+--------+--------+--------+
~~~

### Apple keyboards in Windows virtual machines

When you run a Windows virtual machine (VM) on a Mac, there is a potential
issue with two keys on the left side of the keyboard: the keys `@`/`#` and
`<`/`>` are sometimes inverted.

The core reason for this issue is unknown. We observe that the problem appears
on some hypervisors and not on others. Using Parallels Desktop, there is no
problem. The two keys send their expected scan codes. On the other hand, using
VMware or UTM/Qemu, the two keys swap their scan codes.

On the diagram below, the left part shows the expected key codes of a standard
keyboard. This is also what is received by a Windows VM running on Parallels Desktop.
The right part of the diagram shows what is received by a Windows VM running on
VMware or UTM/Qemu. This has been experienced using the `scancodes` utility in
this project.
~~~
   Standard scan codes                On some virtual machines
+------+------+-----------           +------+------+-----------
| @ #  |   1  |                      | @ #  |   1  |
| 0x29 | 0x02 | ...                  | 0x56 | 0x02 | ...
+------+---+--+---+-------           +------+---+--+---+-------
|    TAB   |  Q   |                  |    TAB   |  Q   |
|    0x0F  | 0x10 | ...              |    0x0F  | 0x10 | ...
+----------+-+----+-+-----           +----------+-+----+-+-----
| Caps Lock  |  A   |                | Caps Lock  |  A   |
|    0x3A    | 0x1E | ...            |    0x3A    | 0x1E | ...
+---------+--+---+--+---+-           +---------+--+---+--+---+-
| L Shift | < >  |  Z   |            | L Shift | < >  |  Z   |
|   0x2A  | 0x56 | 0x2C |            |   0x2A  | 0x29 | 0x2C |
+---------++-----+--+---+-           +---------++-----+--+---+-
| Control  | Option |                | Control  | Option |
|   0x1D   | 0x38 a | ...            |   0x1D   | 0x38 a | ...
+----------+--------+-----           +----------+--------+-----
~~~

To face the two situations, this project proposes two versions of each
Apple keyboard. The version named "Apple VM" uses the swapped scan codes
for the keys `@`/`#` and `<`/`>`.

Important: This inversion has been noticed in various versions of the French
Apple keyboards. Because it is not possible for one single person to own all
international keyboards, the problem may be slightly different on other keyboards
and the corresponding "Apple VM" keyboard layout may not be correct. Please
report all discrepancies in the "Issues" area of this project or submit a
pull request with the fix if possible.

## Declaring a keyboard layout on windows

The way a keyboard layout DLL shall be installed is poorly documented.

- The [Windows Driver Samples](https://github.com/microsoft/Windows-driver-samples/tree/main/input/layout)
project on GitHub simply recommends to _"substitute the existing DLL installed by the operating system
with the customized DLL"_. It does not give any hint on how to install a new layout, in addition to
the existing ones.
- The Microsoft Keyboard Layout Creator utility (MSKLC) creates an MSI installer
for each keyboard which correctly installs the DLL but the way it is done is never
described.
- The problem has been discussed multiple times in various online forums
but no clear explanation or solution was proposed.

During the setup of this project, finding how to correctly install a keyboard
layout DLL was the hardest part. Since this is documented nowhere, this note
tries to provide the missing answers. However, note that this procedure was
found after various analyses and trials. It may be incomplete. It is provided
for information only.

The first step is copying the DLL (for instance `kbdusapple.dll`)
into `C:\Windows\System32` (or more precisely `%SystemRoot%\system32`).

The second step is declaring the DLL in the registry. This is the tricky part.

Let's start from a minimum example:
~~~
[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts\a0060409]
"Layout File"="kbdusapple.dll"
"Layout Text"="United States Apple (WKL)"
"Layout Id"="00f8"
~~~

1. Identify the "base language" for your keyboard. This is a 4-digit hexadecimal
   number, here `0409` for English. If you are not sure, browse through
   `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts` and
   spot the 4 LSB digits of a similar entry.
2. Build a unique 8-digit language id. The 4 LSB digits are the base language id.
   The 4 MSB digits must be chosen so that the resulting 8-digit id is unique in
   the system. Typically, during the installation, browse through the registry to
   find all existing keyboards with the same base language id and select the
   "next" 4-digit MSB part. In the example, the result is `a0060409`. Using `a`
   as most significant digit seems to be a convention but may be irrelevant.
3. Create the corresponding registry key. Inside the key, create the value
   "Layout File". It shall contain the DLL file name, without directory.
4. Create the value "Layout Text" with some short text which will appear in
   the list of keyboards in the System Settings application.
5. Create a unique 4-digit hexadecimal value for the value "Layout Id".
   This value is mandatory. Without this registry entry, your keyboard will
   be seen in the System Settings but will never be activated. This is the
   tricky part of the procedure. Typically, the installation procedure shall
   browse `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts`
   and allocate a new unique value.

In this project, all these steps are handled by the utility `kbdadmin -i`.
