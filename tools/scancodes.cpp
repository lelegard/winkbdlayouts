//---------------------------------------------------------------------------
// Utility to display scan codes and virtual keys.
// Useful to explore a new keyboard.
//---------------------------------------------------------------------------

#include <stdio.h>
#include <windows.h>


//---------------------------------------------------------------------------
// Table of virtual key names.
//---------------------------------------------------------------------------

static const char* const vk_names[256] = {
    /*00*/ nullptr,
    /*01*/ "LBUTTON",
    /*02*/ "RBUTTON",
    /*03*/ "CANCEL",
    /*04*/ "MBUTTON",
    /*05*/ "XBUTTON1",
    /*06*/ "XBUTTON2",
    /*07*/ nullptr,
    /*08*/ "BACK",
    /*09*/ "TAB",
    /*0A*/ nullptr,
    /*0B*/ nullptr,
    /*0C*/ "CLEAR",
    /*0D*/ "RETURN",
    /*0E*/ nullptr,
    /*0F*/ nullptr,
    /*10*/ "SHIFT",
    /*11*/ "CONTROL",
    /*12*/ "MENU",
    /*13*/ "PAUSE",
    /*14*/ "CAPITAL",
    /*15*/ "KANA",
    /*16*/ "IME_ON",
    /*17*/ "JUNJA",
    /*18*/ "FINAL",
    /*19*/ "KANJI",
    /*1A*/ "IME_OFF",
    /*1B*/ "ESCAPE",
    /*1C*/ "CONVERT",
    /*1D*/ "NONCONVERT",
    /*1E*/ "ACCEPT",
    /*1F*/ "MODECHANGE",
    /*20*/ "SPACE",
    /*21*/ "PRIOR",
    /*22*/ "NEXT",
    /*23*/ "END",
    /*24*/ "HOME",
    /*25*/ "LEFT",
    /*26*/ "UP",
    /*27*/ "RIGHT",
    /*28*/ "DOWN",
    /*29*/ "SELECT",
    /*2A*/ "PRINT",
    /*2B*/ "EXECUTE",
    /*2C*/ "SNAPSHOT",
    /*2D*/ "INSERT",
    /*2E*/ "DELETE",
    /*2F*/ "HELP",
    /*30*/ "'0'",
    /*31*/ "'1'",
    /*32*/ "'2'",
    /*33*/ "'3'",
    /*34*/ "'4'",
    /*35*/ "'5'",
    /*36*/ "'6'",
    /*37*/ "'7'",
    /*38*/ "'8'",
    /*39*/ "'9'",
    /*3A*/ nullptr,
    /*3B*/ nullptr,
    /*3C*/ nullptr,
    /*3D*/ nullptr,
    /*3E*/ nullptr,
    /*3F*/ nullptr,
    /*40*/ nullptr,
    /*41*/ "'A'",
    /*42*/ "'B'",
    /*43*/ "'C'",
    /*44*/ "'D'",
    /*45*/ "'E'",
    /*46*/ "'F'",
    /*47*/ "'G'",
    /*48*/ "'H'",
    /*49*/ "'I'",
    /*4A*/ "'J'",
    /*4B*/ "'K'",
    /*4C*/ "'L'",
    /*4D*/ "'M'",
    /*4E*/ "'N'",
    /*4F*/ "'O'",
    /*50*/ "'P'",
    /*51*/ "'Q'",
    /*52*/ "'R'",
    /*53*/ "'S'",
    /*54*/ "'T'",
    /*55*/ "'U'",
    /*56*/ "'V'",
    /*57*/ "'W'",
    /*58*/ "'X'",
    /*59*/ "'Y'",
    /*5A*/ "'Z'",
    /*5B*/ "LWIN",
    /*5C*/ "RWIN",
    /*5D*/ "APPS",
    /*5E*/ nullptr,
    /*5F*/ "SLEEP",
    /*60*/ "NUMPAD0",
    /*61*/ "NUMPAD1",
    /*62*/ "NUMPAD2",
    /*63*/ "NUMPAD3",
    /*64*/ "NUMPAD4",
    /*65*/ "NUMPAD5",
    /*66*/ "NUMPAD6",
    /*67*/ "NUMPAD7",
    /*68*/ "NUMPAD8",
    /*69*/ "NUMPAD9",
    /*6A*/ "MULTIPLY",
    /*6B*/ "ADD",
    /*6C*/ "SEPARATOR",
    /*6D*/ "SUBTRACT",
    /*6E*/ "DECIMAL",
    /*6F*/ "DIVIDE",
    /*70*/ "F1",
    /*71*/ "F2",
    /*72*/ "F3",
    /*73*/ "F4",
    /*74*/ "F5",
    /*75*/ "F6",
    /*76*/ "F7",
    /*77*/ "F8",
    /*78*/ "F9",
    /*79*/ "F10",
    /*7A*/ "F11",
    /*7B*/ "F12",
    /*7C*/ "F13",
    /*7D*/ "F14",
    /*7E*/ "F15",
    /*7F*/ "F16",
    /*80*/ "F17",
    /*81*/ "F18",
    /*82*/ "F19",
    /*83*/ "F20",
    /*84*/ "F21",
    /*85*/ "F22",
    /*86*/ "F23",
    /*87*/ "F24",
    /*88*/ nullptr,
    /*89*/ nullptr,
    /*8A*/ nullptr,
    /*8B*/ nullptr,
    /*8C*/ nullptr,
    /*8D*/ nullptr,
    /*8E*/ nullptr,
    /*8F*/ nullptr,
    /*90*/ "NUMLOCK",
    /*91*/ "SCROLL",
    /*92*/ "OEM_92",
    /*93*/ "OEM_93",
    /*94*/ "OEM_94",
    /*95*/ "OEM_95",
    /*96*/ "OEM_96",
    /*97*/ nullptr,
    /*98*/ nullptr,
    /*99*/ nullptr,
    /*9A*/ nullptr,
    /*9B*/ nullptr,
    /*9C*/ nullptr,
    /*9D*/ nullptr,
    /*9E*/ nullptr,
    /*9F*/ nullptr,
    /*A0*/ "LSHIFT",
    /*A1*/ "RSHIFT",
    /*A2*/ "LCONTROL",
    /*A3*/ "RCONTROL",
    /*A4*/ "LMENU",
    /*A5*/ "RMENU",
    /*A6*/ "BROWSER_BACK",
    /*A7*/ "BROWSER_FORWARD",
    /*A8*/ "BROWSER_REFRESH",
    /*A9*/ "BROWSER_STOP",
    /*AA*/ "BROWSER_SEARCH",
    /*AB*/ "BROWSER_FAVORITES",
    /*AC*/ "BROWSER_HOME",
    /*AD*/ "VOLUME_MUTE",
    /*AE*/ "VOLUME_DOWN",
    /*AF*/ "VOLUME_UP",
    /*B0*/ "MEDIA_NEXT_TRACK",
    /*B1*/ "MEDIA_PREV_TRACK",
    /*B2*/ "MEDIA_STOP",
    /*B3*/ "MEDIA_PLAY_PAUSE",
    /*B4*/ "LAUNCH_MAIL",
    /*B5*/ "LAUNCH_MEDIA_SELECT",
    /*B6*/ "LAUNCH_APP1",
    /*B7*/ "LAUNCH_APP2",
    /*B8*/ nullptr,
    /*B9*/ nullptr,
    /*BA*/ "OEM_1",
    /*BB*/ "OEM_PLUS",
    /*BC*/ "OEM_COMMA",
    /*BD*/ "OEM_MINUS",
    /*BE*/ "OEM_PERIOD",
    /*BF*/ "OEM_2",
    /*C0*/ "OEM_3",
    /*C1*/ nullptr,
    /*C2*/ nullptr,
    /*C3*/ nullptr,
    /*C4*/ nullptr,
    /*C5*/ nullptr,
    /*C6*/ nullptr,
    /*C7*/ nullptr,
    /*C8*/ nullptr,
    /*C9*/ nullptr,
    /*CA*/ nullptr,
    /*CB*/ nullptr,
    /*CC*/ nullptr,
    /*CD*/ nullptr,
    /*CE*/ nullptr,
    /*CF*/ nullptr,
    /*D0*/ nullptr,
    /*D1*/ nullptr,
    /*D2*/ nullptr,
    /*D3*/ nullptr,
    /*D4*/ nullptr,
    /*D5*/ nullptr,
    /*D6*/ nullptr,
    /*D7*/ nullptr,
    /*D8*/ nullptr,
    /*D9*/ nullptr,
    /*DA*/ nullptr,
    /*DB*/ "OEM_4",
    /*DC*/ "OEM_5",
    /*DD*/ "OEM_6",
    /*DE*/ "OEM_7",
    /*DF*/ "OEM_8",
    /*E0*/ nullptr,
    /*E1*/ "OEM_E1",
    /*E2*/ "OEM_102",
    /*E3*/ nullptr,
    /*E4*/ nullptr,
    /*E5*/ "PROCESSKEY",
    /*E6*/ "OEM_E6"
    /*E7*/ "PACKET",
    /*E8*/ nullptr,
    /*E9*/ nullptr,
    /*EA*/ nullptr,
    /*EB*/ nullptr,
    /*EC*/ nullptr,
    /*ED*/ nullptr,
    /*EE*/ nullptr,
    /*EF*/ nullptr,
    /*F0*/ nullptr,
    /*F1*/ nullptr,
    /*F2*/ nullptr,
    /*F3*/ nullptr,
    /*F4*/ nullptr,
    /*F5*/ nullptr,
    /*F6*/ "ATTN",
    /*F7*/ "CRSEL",
    /*F8*/ "EXSEL",
    /*F9*/ "EREOF",
    /*FA*/ "PLAY",
    /*FB*/ "ZOOM",
    /*FC*/ "NONAME",
    /*FD*/ "PA1",
    /*FE*/ "OEM_CLEAR",
    /*FF*/ nullptr
};


//---------------------------------------------------------------------------
// Display one key message.
//---------------------------------------------------------------------------

static void print_key(const char* name, WPARAM wParam, LPARAM lParam)
{
    const int vk = int(wParam);
    const int scancode = int((lParam >> 16) & 0xFF);
    const int ext = int((lParam >> 24) & 0x01);
    const int alt = int((lParam >> 29) & 0x01);
    printf("%-10s  Scan code: 0x%02X, Ext (Ctrl, Right-Alt): %d, Alt: %d, VK: 0x%04X", name, scancode, ext, alt, vk);
    if (vk < 256 && vk_names[vk] != nullptr) {
        printf(" (%s)", vk_names[vk]);
    }
    printf("\n");
}


//---------------------------------------------------------------------------
// Application entry point.
//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    WNDCLASS wclass;
    memset(&wclass, 0, sizeof(wclass));
    wclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wclass.lpfnWndProc = DefWindowProc;
    wclass.lpszClassName = L"ScanCodesClass";
    wclass.hCursor = LoadCursor(0, IDC_ARROW);
    wclass.cbWndExtra = 0;
    RegisterClass(&wclass);

    HWND window = CreateWindowExA(0, "ScanCodesClass", "ScanCodes", WS_OVERLAPPEDWINDOW, 0, 0, 200, 200, 0, 0, 0, 0);
    ShowWindow(window, SW_SHOW);

    printf("Click on the small window and press keys to see their scan codes.\n");
    printf("Close this shell window to stop the application.\n");

    MSG msg;
    while (GetMessageA(&msg, window, 0, 0) > 0) {
        switch (msg.message) {
            case WM_SYSKEYDOWN:
                print_key("SYSKEYDOWN", msg.wParam, msg.lParam);
                break;
            case WM_SYSKEYUP:
                print_key("SYSKEYUP", msg.wParam, msg.lParam);
                break;
            case WM_KEYDOWN:
                print_key("KEYDOWN", msg.wParam, msg.lParam);
                break;
            case WM_KEYUP:
                print_key("KEYUP", msg.wParam, msg.lParam);
                break;
            case WM_PAINT: {
                // Make sure the window stops complaining about paint.
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(msg.hwnd, &ps);
                EndPaint(msg.hwnd, &ps);
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
