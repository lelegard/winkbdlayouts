# Windows Keyboards Layouts (WKL)

The WKL project collects the source code of a few keyboard layouts which are not,
or no longer, included by default in Windows distributions.

**Contents:**

* [Installation](#installation)
* [Build instructions](#build-instructions)
* [New keyboard support and contributions](#new-keyboard-support-and-contributions)
  * [Initial steps: create the new directory](#initial-steps-create-the-new-directory)
  * [Option 1: duplicate a source file from this project](#option-1-duplicate-a-source-file-from-this-project)
  * [Option 2: reverse the content of an existing keyboard DLL](#option-2-reverse-the-content-of-an-existing-keyboard-dll)
  * [Final steps: add the project into the solution](#final-steps-add-the-project-into-the-solution)
* [Keyboard layout definition guidelines](#keyboard-layout-definition-guidelines)
  * [Keyboard concepts](#keyboard-concepts)
  * [Scan codes](#scan-codes)
  * [Keyboard layout source file overview](#keyboard-layout-source-file-overview)
  * [Key names](#key-names)
  * [Mapping scan codes to virtual keys](#mapping-scan-codes-to-virtual-keys)
  * [Definition of modifiers](#definition-of-modifiers)
  * [Character generation](#character-generation)
  * [Dead keys character generation](#dead-keys-character-generation)
  * [Ligatures](#ligatures)
* [Declaring a keyboard layout on windows](#declaring-a-keyboard-layout-on-windows)
* [Notes on Apple keyboards](#notes-on-apple-keyboards)
  * [Apple keyboards bottom row](#apple-keyboards-bottom-row)
  * [Apple keyboards numerical pad](#apple-keyboards-numerical-pad)
  * [Apple keyboards in Windows virtual machines](#apple-keyboards-in-windows-virtual-machines)
* [List of available keyboards](#list-of-available-keyboards)

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

## New keyboard support and contributions

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
  See "Keyboard layout guidelines" below.

- In the file `strings.h`, the `WKL_TEXT` string describes the layout and will be
  displayed in the System Settings for your keyboard. Update accordingly.

- In the file `strings.h`, the `WKL_LANG` string is the base language for which
  your keyboard is a variant. For instance, `040c` means French. If you do not
  know which id to choose, use the Registry Editor to browse through the existing
  keyboard layout ids at
  `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts`.
  The subkeys are 8-digit hexadecimal values for installed keyboards. The rightmost
  4 digits are the base language id.

- Before building the new keyboard for the first time, open `winkbdlayouts.sln` with
  Visual Studio, right-click on the solution => "Add" => "Existing Project" and
  select `kbdXXYYY.vcxproj`. Then "File" => "Save All".

The new keyboard is now part of the solution and will be built with the rest of it.

## Keyboard layout definition guidelines

Once you have a `kbdXXYYY.c` source file, either copied from another source
file or reversed from a keyboard DLL, it shall be modified according to your
keyboard. This section contains some guidelines to help you.

### Keyboard concepts

First, we need to introduce a few concepts:

- **Scan codes**: A scan code is a hardware concept. This is a 8-bit value which
  is generated by a physical key. For a given hardware model of keyboard, the key
  at a given physical position always generates the same scan code, regardless of the
  language layout.

- **Virtual keys**: A virtual key is a Windows concept. This is a 16-bit value
  with a more-or-less defined logical function. A language layout typically
  associates a _scan code_ with a _virtual key code_. A virtual key logically represents
  a key, without considering modifiers such as Shift or Control. Shift and Control
  are considered as separate keys, with their own scan codes and virtual keys.

- **Modifiers**: There are three main predefined modifiers: Shift, Control and Alt.
  Using any combination of modifiers (including no modifier), each virtual key
  may have up to 8 different interpretations. When a keyboard has an "AltGr" key,
  it is not a additional modifier, it is equivalent to a Control+Alt combination.
  Some keyboards have more than three modifiers (Asian keyboards).

- **Dead keys**: A dead key is a software concept in Windows keyboard layouts.
  This is a convenience for languages which use accentuated characters. For instance,
  '^' is defined as a dead key for languages using characters with circumflex
  accents. Hitting the '^' key initially gives nothing. When you type the next key,
  an accentuated character is produced if there is one (`^` followed by `e`
  gives `ê`, `^` followed by `o` gives `ô`). To generate the accent alone,
  simply hit space after it (`^` followed by ` ` gives `^`).

### Scan codes

The first step is to establish a physical map of the keyboard. This means
understanding which scan code is generated by each key.

The image [images\scancodes.jpg](images/scancodes.jpg) shows the scan codes for
a standard 101/102-key PC keyboard. Most PC keyboards are identical to this one.
The characters on the image correspond to a US keyboard. This is just for
reference. Keep in mind that all 101/102-key PC keyboards use the same scan
codes, regardless of the language layout.

If you have difficulties to collect the scan codes for your keyboard, run the
`scancodes` tool from this project. This is a console tool which opens a small
window. Click on that small window and type on the keyboard. The corresponding
scan codes and modifiers are displayed on the console.

### Keyboard layout source file overview

All keyboard-related data structures are declared in the standard header file named
`kbd.h`. It is typically found in a path similar to
`C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um`
(adapt the Windows kit version). This header is included in all keyboard layout
source files.

Starting from the end of the source file, the function `KbdLayerDescriptor()` is a
predefined name for all keyboard layout DLL's. It is called by the system and only
returns the address of a structure.

This structure is named `kbd_tables` and has type `KBDTABLES`. This is a dispatch
table which contains addresses of other structures.

Some of these structures are optional, for instance dead keys or ligatures.
This depends on the language. Starting from an existing keyboard for a similar
language increases your chances to have the right data structures already in place.
Thus, the function `KbdLayerDescriptor()` and the data structure `kbd_tables` do not
need any modification.

Now, we are going to review all data structures in their order in the source file,
starting from the beginning of the file. As mentioned above, some of these structures
may be omitted.

All data structures are arrays. The end of an array is usually identified by some "null"
entry (depending on the data type). By sure to keep this entry at the end of each
structure.

### Key names

The first three arrays give names to keys. This is informational only.

- `key_names` : Associate a scan code with a name. Use this for function keys only.
  Alphanumerical keys do not need a name, they are self-explanatory.
- `key_names_ext` : This is the same thing for the "extended keypad".
- `key_names_dead` : This is a list of names for dead keys. Each entry is only one
  string. It is usually presented as two consecutive literals (for instance `L"~" L"TILDE"`)
  but this is just a C syntax trick (this is equivalent to `L"~TILDE"`).

### Mapping scan codes to virtual keys

The array `scancode_to_vk` associates each scan code to a virtual key.

This is a simple array. This index in the array is the scan code,
starting from 0. There is no terminator, the size of the array is
explicitely computed in the `KBDTABLES` dispatch table.

You may increase or decrease the size of the array `scancode_to_vk` but
keep all consecutive entries. Also keep the hexadecimal index values in comments.
Each one is the index in the array and consequently the scan code value.
These comments are useful to edit the table.

The value of each entry is the 16-bit virtual key code. When a scan code
is non-existent on the keyboard and has no virtual key, use the symbol `VK__none_`.

Virtual key codes for alphanumerical keys are the correponding character
literals `'A'` to `'Z'` and `'0'` to `'9'`.

Other virtual key codes have symbols starting with `VK_`. Some symbols
are obvious to choose (`VK_ESCAPE`, `VK_RETURN` for instance). Others
are more ambiguous.

The list of the most common virtual key symbols are:
~~~
VK_CANCEL     VK_SNAPSHOT    VK_F9         VK_CONVERT              VK_OEM_3
VK_BACK       VK_INSERT      VK_F10        VK_NONCONVERT           VK_OEM_4
VK_TAB        VK_DELETE      VK_F11        VK_ACCEPT               VK_OEM_5
VK_CLEAR      VK_HELP        VK_F12        VK_MODECHANGE           VK_OEM_6
VK_RETURN     VK_SLEEP       VK_F13        VK_VOLUME_MUTE          VK_OEM_7
VK_SHIFT      VK_NUMLOCK     VK_F14        VK_VOLUME_DOWN          VK_OEM_8
VK_CONTROL    VK_SCROLL      VK_F15        VK_VOLUME_UP            VK_OEM_102
VK_ESCAPE     VK_NUMPAD0     VK_F16        VK_MEDIA_NEXT_TRACK     VK_ICO_CLEAR
VK_MENU       VK_NUMPAD1     VK_F17        VK_MEDIA_PREV_TRACK     VK_PACKET
VK_PAUSE      VK_NUMPAD2     VK_F18        VK_MEDIA_STOP           VK_OEM_RESET
VK_LWIN       VK_NUMPAD3     VK_F19        VK_MEDIA_PLAY_PAUSE     VK_OEM_JUMP
VK_RWIN       VK_NUMPAD4     VK_F20        VK_LAUNCH_MAIL          VK_OEM_PA1
VK_APPS       VK_NUMPAD5     VK_F21        VK_LAUNCH_MEDIA_SELECT  VK_OEM_PA2
VK_SPACE      VK_NUMPAD6     VK_F22        VK_LAUNCH_APP1          VK_OEM_PA3
VK_LSHIFT     VK_NUMPAD7     VK_F23        VK_LAUNCH_APP2          VK_OEM_WSCTRL
VK_RSHIFT     VK_NUMPAD8     VK_F24        VK_PROCESSKEY           VK_OEM_CUSEL
VK_LCONTROL   VK_NUMPAD9     VK_LBUTTON    VK_ICO_HELP             VK_OEM_ATTN
VK_RCONTROL   VK_MULTIPLY    VK_RBUTTON    VK_ICO_00               VK_OEM_FINISH
VK_LMENU      VK_ADD         VK_MBUTTON    VK_OEM_FJ_JISHO         VK_OEM_COPY
VK_RMENU      VK_SEPARATOR   VK_XBUTTON1   VK_OEM_FJ_MASSHOU       VK_OEM_AUTO
VK_PRIOR      VK_SUBTRACT    VK_XBUTTON2   VK_OEM_FJ_TOUROKU       VK_OEM_ENLW
VK_NEXT       VK_DECIMAL     VK_CAPITAL    VK_OEM_FJ_LOYA          VK_OEM_BACKTAB
VK_END        VK_DIVIDE      VK_KANA       VK_OEM_FJ_ROYA          VK_ATTN
VK_HOME       VK_F1          VK_HANGEUL    VK_OEM_NEC_EQUAL        VK_CRSEL
VK_LEFT       VK_F2          VK_HANGUL     VK_OEM_PLUS             VK_EXSEL
VK_UP         VK_F3          VK_IME_ON     VK_OEM_COMMA            VK_EREOF
VK_RIGHT      VK_F4          VK_JUNJA      VK_OEM_MINUS            VK_PLAY
VK_DOWN       VK_F5          VK_FINAL      VK_OEM_PERIOD           VK_ZOOM
VK_SELECT     VK_F6          VK_HANJA      VK_OEM_AX               VK_NONAME
VK_PRINT      VK_F7          VK_KANJI      VK_OEM_1                VK_PA1
VK_EXECUTE    VK_F8          VK_IME_OFF    VK_OEM_2                VK_OEM_CLEAR
~~~
In practice, keys which produces actual characters can be associated to any
virtual key code since another table will indicate which combination of
virtual key code and modifiers produces which characters. The idea is
to remain rational and use the appropriate names.

Keys which generate punctuation marks only have no obvious `VK_` symbol
because the way they are combined with the Shift key is highly dependent
on the language. For those, use one of `VK_OEM_1` to `VK_OEM_8`.

All `VK_` symbols are 8-bit values. A virtual key code can be or'ed with one
of the following 8 flags in the next byte. The meaning of these flags is not
fully clear.
~~~
KBDEXT           extended keyboard (?)
KBDMULTIVK
KBDSPECIAL       special keyboard (?)
KBDNUMPAD        key on the numerical keypad
KBDUNICODE
KBDINJECTEDVK
KBDMAPPEDVK
KBDBREAK
~~~

Additional virtual key symbols for special keyboards:
~~~
VK_NAVIGATION_VIEW     VK_BROWSER_FAVORITES        VK_GAMEPAD_DPAD_RIGHT
VK_NAVIGATION_MENU     VK_BROWSER_HOME             VK_GAMEPAD_MENU
VK_NAVIGATION_UP       VK_GAMEPAD_A                VK_GAMEPAD_VIEW
VK_NAVIGATION_DOWN     VK_GAMEPAD_B                VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON
VK_NAVIGATION_LEFT     VK_GAMEPAD_X                VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON
VK_NAVIGATION_RIGHT    VK_GAMEPAD_Y                VK_GAMEPAD_LEFT_THUMBSTICK_UP
VK_NAVIGATION_ACCEPT   VK_GAMEPAD_RIGHT_SHOULDER   VK_GAMEPAD_LEFT_THUMBSTICK_DOWN
VK_NAVIGATION_CANCEL   VK_GAMEPAD_LEFT_SHOULDER    VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT
VK_BROWSER_BACK        VK_GAMEPAD_LEFT_TRIGGER     VK_GAMEPAD_LEFT_THUMBSTICK_LEFT
VK_BROWSER_FORWARD     VK_GAMEPAD_RIGHT_TRIGGER    VK_GAMEPAD_RIGHT_THUMBSTICK_UP
VK_BROWSER_REFRESH     VK_GAMEPAD_DPAD_UP          VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN
VK_BROWSER_STOP        VK_GAMEPAD_DPAD_DOWN        VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT
VK_BROWSER_SEARCH      VK_GAMEPAD_DPAD_LEFT        VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT
~~~

The lists `scancode_to_vk_e0` and `scancode_to_vk_e1` serve the same purpose
for scan codes with E0 and E1 prefix, respectively. The lists are short.
Consequently, the values of the scan codes are explictely specified in the list
instead of being implicitely an index in an array.

### Definition of modifiers

For western keyboards, the three modifiers are Shift, Control and Alt.
AltGr, when available, is equivalent to Control+Alt.

The structure `char_modifiers` defines how modifiers are handled in this keyboard layout.

First, the structure `vk_to_bits` defines how virtual key codes are mapped to bits in
modifier masks. For western keyboards, the structure is always the following:
~~~
static VK_TO_BIT vk_to_bits[] = {
    {VK_SHIFT,   KBDSHIFT},  // KBDSHIFT = 0b001
    {VK_CONTROL, KBDCTRL},   // KBDCTRL  = 0b010
    {VK_MENU,    KBDALT},    // KBDALT   = 0b100
    {0,          0}
};
~~~

Second, the structure `char_modifiers` defines which combinations of modifiers are
valid for this keyboard. These combinations are listed in the `ModNumber` array field.
The index of the last value is in the `wMaxModBits` field. Note that this is an index,
not the size of the `ModNumber` array. It must be the size minus 1.

The indexes in the `ModNumber` array are a combination of the `KBDxxx` modifier masks.
The values in the `ModNumber` array are "modification numbers" which will be used later.

Here is an example:
~~~
static MODIFIERS char_modifiers = {
    .pVkToBit    = vk_to_bits,
    .wMaxModBits = 6, // index of the last element 
    .ModNumber   = {
        0,            // index 0b000                      = <none>
        1,            // index 0b001 = KBDSHIFT           = Shift
        2,            // index 0b010 = KBDCTRL            = Control
        4,            // index 0b011 = KBDSHIFT | KBDCTRL = Shift Control
        SHFT_INVALID, // index 0b100 = KBDALT             = Alt
        SHFT_INVALID, // index 0b101 = KBDSHIFT | KBDALT  = Shift Alt
        3,            // index 0b110 = KBDCTRL | KBDALT   = Control Alt (AltGr)
    }
};
~~~
Notes on the example:
- There are 7 entries in `ModNumber` and `wMaxModBits` is consequently 6 (last index = size - 1).
- The index in `ModNumber` is a bit mask of modifiers, here from 0b000 (0) to 0b110 (6).
- Some combinations of modifiers may not be used in a keyboard. Index 7 = 0b111 = `KBDSHIFT | KBDCTRL | KBDALT`
  is out of range and not used. Indexes 0b100 and 0b101 are within range but the "modification number"
  is the special value `SHFT_INVALID` which means that this combination is not used.
- Other entries have valid "modification numbers". All modification numbers must be globally
  contiguous (here 0, 1, 2, 3, 4) but not necessarily in increasing order.
- As an example, the modification number 3 is at index 6 = 0b110. Modification number 3
  consequently means Control+Alt (which is the equivalent of AltGr on keyboards with an AltGr key).

### Character generation

The next tables define which characters are generated for a virtual key code
and all its combinations of modifiers.

One virtual key may generate one character only (numerical keypad for instance).
Or two if using Shift produces a distinct character (letter keys for instance).
Or three if there is a third AltGr possibility. Or more...

This is were it becomes complicated.

Each entry of the character generation table has:
- A virtual key code. This is either a `VK_` symbol, `'A'` to `'Z'`, or `'0'` to `'9'`.
- A bit mask of attributes.
  - `CAPLOK` : When "Caps Lock" is on, use an implicit Shift.
  - `SGCAPS`
  - `CAPLOKALTGR` : When "Caps Lock" is on and AltGr is pressed, use an implicit AltGr+Shift.
  - `KANALOK`
  - `GRPSELTAP`
- An array of _N_ characters (16-bit wide characters). This is where the "modification
  numbers" are used as index in the array.

If the previous example, we had 5 different modification numbers, 0 to 4. They are used
as index in the character array of each entry of the character generation tables.

- At index 0 : virtual key without modifier
- At index 1 : virtual key with Shift
- At index 2 : virtual key with Control
- At index 3 : virtual key with Control+Alt (or AltGr)
- At index 4 : virtual key with Shift+Control

This is were it becomes even more complicated.

To optimize space, there is not one single character generation table. There are several of
them. The idea is that the "modification numbers" which are the less used for that keyboard
are at the end (number 4 in the example). We create a table of all characters with modification
number 0 to 4, another one (with shorter entries) with modification numbers 0 to 3, etc.

The tables with trimmed entries have types `VK_TO_WCHARS1`, `VK_TO_WCHARS2`, `VK_TO_WCHARS3`,
etc. where the last digit is the number of characters which can be generated for the virtual
key of this entry.

Since all entries do not have the same type, they are grouped in distinct tables,
one per entry size. The data structure `vk_to_wchar` is the master character
generation structures. It contains a list of pointers to the various character
generation tables. For each table, the number of characters in each entry
and the size in bytes of an entry are specified.

Let's take an example to illustrate two other specificities:
~~~
static VK_TO_WCHARS4 vk_to_wchar4[] = {
    //                               Shift     Ctrl      Ctrl/Alt
    //                               -----     ----      --------
    {'2',         CAPLOK, {0x00E9,   L'2',     WCH_NONE, WCH_DEAD}}, // e acute
    {VK__none_,   0x00,   {WCH_NONE, WCH_NONE, WCH_NONE, L'~'}},
    {'3',         CAPLOK, {L'"',     L'3',     WCH_NONE, L'#'}},
    ...
~~~

The table uses type `VK_TO_WCHARS4` which contains 4 characters per entry.
These characters are indexed from 0 to 3. These indexes are "modification numbers"
from the `char_modifiers` structure. Looking up these modification numbers in
`char_modifiers`, we see that number 0 has no modifier, number 1 uses Shift,
number 2 uses Control and number 3 uses Control+ Alt. This is reminded in the
comments on top of the columns.

On a French keyboard, the key '2' generates an "e acute" (unicode 0x00E9). Shift+'2'
generates a "2". Control+'2' does nothing and the symbol `WCH_NONE` is used. AltGr+'2'
(or Control+Alt+'2') generates a tilde '\~'. However, tilde is a dead key. This is indicated
by the symbol `WCH_DEAD` instead of '\~'. So, how do you indicate that the corresponding
dead key is a tilde and not some other accent? We add a dummy entry immediately following
any entry with a `WCH_DEAD`. This entry has virtual key `VK__none_` and all characters
with a `WCH_DEAD` on the preceding line are replaced with the actual dead key, here a '\~'.
Any other character in this dummy line is ignored and set to `WCH_NONE`.

Pretty weird, isn't it?

### Dead keys character generation

Now that we have defined some dead keys (such as '~' in the previous example),
how do we specify which accentuated characters can be generated following each
dead key?

This is done in table `dead_keys`. Each entry defines the combination of a letter
(`n` for instance), an accent (the dead key, `~` in the example) and the actual
character which is produced when hitting the dead key followed by the letter
(`ñ` in that case).
 
The last field of each entry in `dead_keys` is a mask of flags. The only defined
flag is `DKF_DEAD` which is used for dead key chaining. As recommended by Dimitriy Ryazantcev
in [issue #1](https://github.com/lelegard/winkbdlayouts/issues/1), more information can be found
[here](http://archives.miloush.net/michkap/archive/2011/04/16/10154700.html) and
[here](http://www.kbdedit.com/manual/dead_chained.html).

### Ligatures

The last table is rarely used. It defines "ligatures", sequences of characters
which are sent from one key.

In this project, the keyboard layout `kbdfrnodead` (French keyboard without dead keys)
contains an example of this: The combination AltGr-'T' generates "test", a string of
four letters. This is useless, just an example of ligature for reference.

## Declaring a keyboard layout on windows

The way a keyboard layout DLL shall be installed on Windows is poorly documented.

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

Note that this registration cannot be statically defined. It is not possible
to provide a predefined `.reg` file to be installed everywhere for a given keyboard.
There is a dynamic part to be executed by an installation program, typically to
find unique identifiers on the target system.

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
   spot the 4 LSB digits of an entry for the same language (or similar).
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
   and allocate a new unique value. This must be a unique value among _all_
   installed keyboards, not only inside the same base language.

In this project, all these steps are handled by the utility `kbdadmin -i`.

Developers who want to install their own keyboard layouts, independently of
this project, may use the `libtools` library. See the functions `InstallKeyboardLayout()`
and `UninstallKeyboardLayout()` in `kbdinstall.h`.

## Notes on Apple keyboards

The Apple keyboards as found on Mac systems are notoriously different from
PC keyboards. When running a Windows virtual machine on a Mac, the default
Windows keyboard layouts are not appropriate.

On Windows 11, the Apple keyboards are not available by default and need custom
mappings. The hypervisor hosting the Windows virtual machine may provide specific
mappings for Apple keyboards, typically as part of "guest tools" to be installed
on the Windows virtual machine. This is the case for Parallels Desktop for instance.
However, with UTM/Qemu, there are no guest tools with Apple keyboard layouts.
This specific issue was the initial reason to create the WKL project.

In the diagrams below, the hexadecimal values are the scan codes of the keys,
`e` means "Extended" (Ctrl or Right-Alt) and `a` means "Alt".

### Apple keyboards bottom row

~~~
+---------+--------+---------+-------------------------+---------+--------+---------+
| control | option | command |          space          | command | option | control |
|  0x1D   | 0x38 a | 0x5B e  |          0x39           | 0x5C e  | 0x38 e | 0x1D e  |
+---------+--------+---------+-------------------------+---------+--------+---------+
~~~

### Apple keyboards numerical pad

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
`<`/`>` are sometimes inverted. The presence of these keys depends on the
language layout.

This situation is the result of a long history of keyboards in Apple systems.
An explanation has been proposed by Eugene Golushkov in
[issue #2](https://github.com/lelegard/winkbdlayouts/issues/2).

We observe that the problem appears on some hypervisors and not on others.
Using Parallels Desktop, there is no problem. The two keys send their expected scan codes.
On the other hand, using VMware or UTM/Qemu, the two keys swap their scan codes.

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

## List of available keyboards

The following table lists the currently available keyboards in this project.
The base language is a 4-digit hexadecimal number.

| DLL name     | Base | Description
| ------------ | ---- | -----------
| kbdarapple   | 0401 | Arabic Apple
| kbdarapplevm | 0401 | Arabic Apple VM
| kbdbeapple   | 0813 | Belgian (period) Apple
| kbdbeapplevm | 0813 | Belgian (period) Apple VM
| kbdbzapple   | 0416 | Portuguese (Brazil) Apple
| kbdbzapplevm | 0416 | Portuguese (Brazil) Apple VM
| kbdcaapple   | 0c0c | Canadian French Apple
| kbdcaapplevm | 0c0c | Canadian French Apple VM
| kbdczapple   | 0405 | Czech Apple
| kbdczapplevm | 0405 | Czech Apple VM
| kbddaapple   | 0406 | Danish Apple
| kbddaapplevm | 0406 | Danish Apple VM
| kbddeapple   | 0407 | German Apple
| kbddeapplevm | 0407 | German Apple VM
| kbdduapple   | 0413 | Dutch Apple
| kbdduapplevm | 0413 | Dutch Apple VM
| kbddvapple   | 0409 | United States (Dvorak) Apple
| kbddvapplevm | 0409 | United States (Dvorak) Apple VM
| kbdejapple   | 0809 | English Japanese Apple
| kbdejapplevm | 0809 | English Japanese Apple VM
| kbdfiapple   | 040b | Finnish Apple
| kbdfiapplevm | 040b | Finnish Apple VM
| kbdfnapple   | 040c | French (Numerical) Apple
| kbdfnapplevm | 040c | French (Numerical) Apple VM
| kbdfrapple   | 040c | French Apple
| kbdfrapplevm | 040c | French Apple VM
| kbdfrnodead  | 040c | French - No Dead Key
| kbditapple   | 0410 | Italian Apple
| kbditapplevm | 0410 | Italian Apple VM
| kbdnoapple   | 0414 | Norwegian Apple
| kbdnoapplevm | 0414 | Norwegian Apple VM
| kbdplapple   | 0415 | Polish (Programmers) Apple
| kbdplapplevm | 0415 | Polish (Programmers) Apple VM
| kbdpoapple   | 0816 | Portuguese Apple
| kbdpoapplevm | 0816 | Portuguese Apple VM
| kbdruapple   | 0419 | Russian Apple
| kbdruapplevm | 0419 | Russian Apple VM
| kbdsgapple   | 0807 | Swiss German Apple
| kbdsgapplevm | 0807 | Swiss German Apple VM
| kbdspapple   | 040a | Spanish Apple
| kbdspapplevm | 040a | Spanish Apple VM
| kbdswapple   | 041d | Swedish Apple
| kbdswapplevm | 041d | Swedish Apple VM
| kbdszapple   | 100c | Swiss French Apple
| kbdszapplevm | 100c | Swiss French Apple VM
| kbduaapple   | 0409 | Ukrainian Apple
| kbduaapplevm | 0409 | Ukrainian Apple VM
| kbduiapple   | 0409 | United States International Apple
| kbduiapplevm | 0409 | United States International Apple VM
| kbdukapple   | 0809 | United Kingdom Apple
| kbdukapplevm | 0809 | United Kingdom Apple VM
| kbdurapple   | 0409 | United States ISO/RU Apple
| kbdurapplevm | 0409 | United States ISO/RU Apple VM
| kbdusapple   | 0409 | United States Apple
| kbdusapplevm | 0409 | United States Apple VM
