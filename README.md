# Windows Keyboards Layouts (WKL)

The WKL project collects the source code of a few keyboard layouts which are not,
or no longer, included by default in Windows distributions.

Specifically, at least on Windows 11, the Apple keyboards are not available
by default and need custom mappings. When Windows 11 is installed as a virtual
machine on a Mac, the hypervisor may include specific mappings for the Apple
keyboards. This is the case for Parallels but not UTM/Qemu. When the hypervisor
does not provide mapping for Mac keyboards, this project is useful.

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

- Create a directory with name `kbdXXYYY` where `XX` is the language code for
  your keyboard (not necessarily two letters, this is just a convention) and
  `YYY` is the type or brand of keyboard (for instance `apple`).

- Copy any project file from an existing keyboard, for instance `kbdfrapple\kbdfrapple.vcxproj`,
  as `kbdXXYYY\kbdXXYYY.vcxproj`. No need to update the content, Visual Studio
  will adjust the project GUID the first time it is used.

### Option 1: duplicate a source file from this project

Let's assume you start from `kbdfrapple`.

- Copy `kbdfrapple\kbdfrapple.c` as `kbdXXYYY\kbdXXYYY.c`. Update the key tables
  according to your keyboard.
- Copy `kbdfrapple\strings.rc` as `kbdXXYYY\strings.rc`. Update the two strings
  in that file.

### Option 2: reverse the content of an existing keyboard DLL

- Use the `kbdreverse` tool in this project to extract the layout definition
  of an installed keyboard and rebuild a source file from that keyboard.

Example: To rebuild source files for a French keyboard (id `fr`), use these commands:
~~~
kbdreverse fr -o kbdXXYYY\kbdXXYYY.c
kbdreverse fr -r -o kbdXXYYY\strings.rc
~~~

Using the parameter `fr` means reversing the file `C:\Windows\System32\kbdfr.dll`.
To reverse a keyboard DLL from another location, specify the full path of the DLL file.

- Update the two new source files according to your keyboard.

### Final steps: add the project into the solution

- In the file `strings.rc`, the `WKL_LANG` string is the base language for which
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

The image `tools\scancodes.jpg` shows the scan codes for a standard 101/102-key PC keyboard.

There are some specificities on the bottom row of Apple keyboards:
~~~
+--------+--------+--------+---------------------+--------+--------+--------+
|control | option |command |        space        |command | option |control |
|  0x1D  | 0x38 a | 0x5B e |        0x39         | 0x5C e | 0x38 e | 0x1D e |
+--------+--------+--------+---------------------+--------+--------+--------+
~~~
Where `e` means "Extended" (Ctrl or Right-Alt) and `a` means "Alt".

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
