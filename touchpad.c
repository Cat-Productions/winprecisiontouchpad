#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

typedef void (__stdcall *TouchCallback)(int x, int y);

static TouchCallback g_callback = NULL;

static const int MAX_X = 4024;
static const int MAX_Y = 2344;

static unsigned short read_u16(const unsigned char* d) {
    return (unsigned short)(d[0] | (d[1] << 8));
}

static void emit(int x, int y) {
    if (g_callback) g_callback(x, y);
}

static void process(const unsigned char* data) {

    int x = read_u16(data + 2);
    int y = read_u16(data + 4);

    // normalize to screen later in Python or here
    emit(x, y);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {

    case WM_INPUT: {

        UINT size = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size,
                        sizeof(RAWINPUTHEADER));

        RAWINPUT* buffer = (RAWINPUT*)HeapAlloc(GetProcessHeap(), 0, size);
        if (!buffer) return 0;

        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
                            buffer, &size,
                            sizeof(RAWINPUTHEADER)) == size) {

            if (buffer->header.dwType == RIM_TYPEHID) {

                RAWHID* hid = &buffer->data.hid;

                for (DWORD i = 0; i < hid->dwCount; i++) {

                    const unsigned char* d =
                        (const unsigned char*)hid->bRawData +
                        i * hid->dwSizeHid;

                    process(d);
                }
            }
        }

        HeapFree(GetProcessHeap(), 0, buffer);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

DWORD WINAPI message_thread(LPVOID arg) {

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "TP_DLL";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, wc.lpszClassName,
        "tp",
        WS_OVERLAPPEDWINDOW,
        0,0,0,0,
        NULL,NULL,wc.hInstance,NULL
    );

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x0D;
    rid.usUsage = 0x05;
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = hwnd;

    RegisterRawInputDevices(&rid, 1, sizeof(rid));

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

__declspec(dllexport)
void start_touchpad(TouchCallback cb) {
    g_callback = cb;
    CreateThread(NULL, 0, message_thread, NULL, 0, NULL);
}