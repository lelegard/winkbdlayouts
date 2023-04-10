# Windows Keyboards Layouts (WKL)

The WKL project collects a few keyboard layouts which are not, or no longer,
included by default in Windows distributions.

Specifically, at least on Windows 11, the Apple keyboards are not available
by default and need custom mappings. When Windows 11 is installed as a virtual
machine on a Mac, the hypervisor may include specific mappings for the Apple
keyboards. This is the case for Parallels but not UTM/Qemu. When the hypervisor
does not provide mapping for Mac keyboards, this project is useful.

## Build instructions

The project is built for x86, x64 and arm64 Windows systems. The released archive
contains binaries for the three architectures.

To build this projet, make sure to install all build tools (compilers and libraries)
for all targets when installing or updating Visual Studio.

Available PowerShell scripts:
- `build.ps1` : Build the binaries for all architectures, build the distribution archive.
- `install.ps1` : Install the keyboards on the current system after rebuild.
- `clean.ps1` : Cleanup all generated files.

## Language support and contributions

New layouts are welcome as contributions. Please post a pull request with your
implementation of new layouts.

To add support for a new keyboard layout, start from an existing keyboard in
this project, for instance `kbdfrapple`.

- Duplicate the `kbdfrapple` directory under the name `kbdXXYYY` where
  `XX` is the language code for your keyboard (not necessarily two letters,
  this is just a convention) and `YYY` is the type or brand of keyboard
  (for instance `apple`).
- Rename all files as `kbdXXYYY.*` in the new directory.
- Update the files `kbdXXYYY.rc` and `kbdXXYYY.reg` with the appropriate
  names and descriptions.
- Update the key tables in `kbdXXYYY.c`.
- The file `kbdXXYYY.vcxproj` does not need any modification.
- Before building it for the first time, open `winkbdlayouts.sln` with
  Visual Studio, right-click on the solution => "Add" => "Existing Project" and
  select `kbdXXYYY.vcxproj`. Then, "File" => "Save All".

Additional notes:

- In the registry file `kbdXXYYY.reg`, carefully select a unique keyboard id.
  Use the Registry Editor to browse through the existing keyboard layout ids at
  `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts`.
  Typically start from the "standard" id of the keyboard language (for instance
  `0000040c` for the standard French PC keyboard), and shift it into the private
  range `b0XXXXXX` (for instance `b000040c` for the French Apple keyboard in this
  project). In practice the last 4 digits indicate a language (`040c` in the example
  means French) and the first 4 digits indicate a flavour. By convention, all
  flavours in this project start with `b0`.

- The key tables in `kbdXXYYY.c` can be manually updated from an existing keyboard
  source file in this project. However, if you already have a valid DLL for that
  keyboard on one system, use the `kbdreverse` tool to extract the keyboard definition
  and rebuild a source file from it. The source file will then be recompiled for all
  architectures.
  
## Scan codes

If you have difficulties to collect the scan codes for your keyboard, build and run
the `scancodes` tool.

The image `scancodes.jpg` shows the scan codes for a standard 101/102-key PC keyboard.

There are some specificities on the bottom row of Apple keyboards:
~~~
+--------+--------+--------+-----------------+--------+--------+--------+
|control | option |command |      space      |command | option |control |
|  0x1D  | 0x38 a | 0x5B e |      0x39       | 0x5C e | 0x38 e | 0x1D e |
+--------+--------+--------+-----------------+--------+--------+--------+
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
