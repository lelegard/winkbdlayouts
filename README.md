# Windows Keyboards Layouts (WKL)

The WKL project collects a few keyboard layouts which are not,
or no longer, included by default in Windows distributions.

Specifically, starting with Windows 11, the Apple keyboards
are no longer available by default and need custom mappings.
When Windows 11 is installed as a virtual machine on a Mac,
the hypervisor may include specific mappings for the Apple
keyboards. This is the case for Parallels but not UTM/Qemu.
In the latter case, this project is useful.

## Build instructions

The project is built for x86, x64 and arm64 Windows systems.
The released archive contains binaries for the three architectures.

To build this projet, make sure to install all build tools
(compilers and libraries) for all targets when installing or
updating Visual Studio.

## Language support and contributions

At the creation of this project, the only supported layout was the
French Apple keyboard, having no other physical keyboard for testing.

New layouts are welcome as contributions. Please post a pull request
with your implementation of new layouts.

To add support for a new keyboard layout:

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
  Typically start from the "standard" id of the keyboard language (for instance
  `0000040c` for the standard French PC keyboard), and shift it into the private
  range (for instance `b000040c` for the French Apple keyboard in this project).
  Use the Registry Editor to browse through the existing keyboard layout ids at
  `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts`.
- If possible, add a photo of the physical keyboard layout in the `images`
  subdirectory. For reference only.

To build the project for all target architectures, run `build.ps1`.
Run `install.ps1` to install them.

## Scan codes

If you have difficulties to collect the scan codes for your keyboard,
build and run the `tools\scancodes` application.

The image `scancodes.jpg` shows the scan codes for a standard
101/102-key PC keyboards.

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
