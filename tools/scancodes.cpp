//---------------------------------------------------------------------------
// Utility to display scan codes and virtual keys.
// Useful to explore a new keyboard.
//---------------------------------------------------------------------------

#include <stdio.h>
#include <windows.h>

static void print_key(const char* name, WPARAM wParam, LPARAM lParam)
{
    printf("Scan code: 0x%02X, Ext (Ctrl, Right-Alt): %d, Alt: %d, VK: 0x%04X, %s\n",
           int((lParam >> 16) & 0xFF), int((lParam >> 24) & 0x01), int((lParam >> 29) & 0x01), int(wParam), name);
}

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

    HWND window = CreateWindowEx(0, L"ScanCodesClass", L"ScanCodes", WS_OVERLAPPEDWINDOW, 0, 0, 200, 200, 0, 0, 0, 0);
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
