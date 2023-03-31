# Apple keyboards layouts for Windows

Can be built for x86, x64 and arm64 Windows systems. Make sure to
install all build tools (compilers and libraries) for all targets
when installing or updating Visual Studio.

## Language support

As of today, the only supported layout is the French keyboard, having
no other physical keyboard for testing.

To add support for a new keyboard layout:

- Duplicate the `kbdfrapple` directory under the name `kbdXXapple` where
  `XX` is the language code for your keyboard (not necessarily two letters,
  this is just a convention).
- Rename all files as `kbdXXapple.*` in the new directory.
- Update the files `kbdXXapple.rc` and `kbdXXapple.reg` with the appropriate
  names and descriptions.
- Update the key tables in `kbdXXapple.c`.
- The files `kbdXXapple.def` and `kbdXXapple.vcxproj` do not need any modification.
- Before building it for the first time, open `windows-apple-keyboards.sln` with
  Visual Studio, right-click on the solution => "Add" => "Existing Project" and
  select `kbdXXapple.vcxproj`. Then, "File" => "Save All".

That's all. Just run `build.ps1` to build the new layouts and `install.ps1` to
install them.

If you have difficulties to collect the scan codes for your keyboard,
build and run the `tools\scancodes` application.

## Scan codes

The picture `tools\scancodes.jpg` shows the scan codes for a standard 101+102-key
PC keyboards. There are some specificities in Apple keyboards.

~~~
Middle pad:
  F13...0x37.e    F14...None    F18...None
  fn....None      home..0x47.e  up....0x49.e
  del...0x53.e    end...0x4F.e  down..0x51.e
                  up....0x48.e
  left..0x4B.e    down..0x50.e  right.0x4D.e


Numeric pad:
  F16...0x67      F17...0x68    F18...0x69      F19...0x6A
  NLck..0x45.e    =.....0x59    /.....0x35.e    *.....0x37
  7.....0x47      8.....0x48    9.....0x49      -.....0x4A
  4.....0x4B      5.....0x4C    6.....0x4D      +.....0x4E
  1.....0x4F      2.....0x50    3.....0x51
  0.....0x52                    ,.....0x53      Ent...0x1C.e
~~~
