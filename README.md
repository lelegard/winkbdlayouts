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
- The files `kbdXXYYY.def` and `kbdXXYYY.vcxproj` do not need any modification.
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

To build the project for all target architectures, run `build.ps1`. Run `install.ps1`
to install them.

If you have difficulties to collect the scan codes for your keyboard,
build and run the `tools\scancodes` application.

## Scan codes

The picture `tools\scancodes.jpg` shows the scan codes for a standard
101/102-key PC keyboards.

There are some specificities on the right-hand side of Apple keyboards:
~~~
F13...0x37.e  F14...None    F15...None      F16...0x67    F17...0x68  F18...0x69    F19...0x6A
fn....None    home..0x47.e  up....0x49.e    NLck..0x45.e  =.....0x59  /.....0x35.e  *.....0x37
del...0x53.e  end...0x4F.e  down..0x51.e    7.....0x47    8.....0x48  9.....0x49    -.....0x4A
                                            4.....0x4B    5.....0x4C  6.....0x4D    +.....0x4E
              up....0x48.e                  1.....0x4F    2.....0x50  3.....0x51
left..0x4B.e  down..0x50.e  right.0x4D.e    0.....0x52                ,.....0x53    Ent...0x1C.e
~~~
