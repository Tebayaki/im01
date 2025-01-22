#include <tchar.h>
#include <stdio.h>
#include <getopt.h>
#include <Windows.h>

#pragma comment(lib, "imm32")

#define IMC_GETCONVERSIONMODE 1
#define IMC_SETCONVERSIONMODE 2
#define IMC_GETOPENSTATUS 5
#define IMC_SETOPENSTATUS 6

HWND GetFocusedWindow();
BOOL GetOpenStatus(HWND hwnd);
void SetOpenStatus(BOOL status, HWND hwnd);
DWORD GetConversionMode(HWND hwnd);
void SetConversionMode(DWORD mode, HWND hwnd);

void help();

int _tmain(int argc, _TCHAR *argv[]) {
    HWND hwnd = GetFocusedWindow();

    const _TCHAR *optstring = TEXT("gs:");

    int opt;
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case 'g':
                _tprintf(TEXT("%d-%lu\n"), GetOpenStatus(hwnd), GetConversionMode(hwnd));
                break;
            case 's': {
                DWORD status, mode;
                int cnt = _stscanf_s(optarg, TEXT("%lu-%lu"), &status, &mode);
                if (cnt > 0) {
                    SetOpenStatus(!!status, hwnd);
                    if (cnt > 1) {
                        SetConversionMode(mode, hwnd);
                    }
                }
            } break;
            case '?':
                help();
                break;
        }
    }
    if (optind <= 1) {
        help();
    }
    return 0;
}

void help() {
    _tprintf_s(TEXT(
        "Options:\n"
        "    -g    Get foreground window's IME status and conversion mode\n"
        "    -s    Set foreground window's IME status and conversion mode\n"
    ));
}

HWND GetFocusedWindow() {
    HWND foreHwnd = GetForegroundWindow();
    if (foreHwnd != NULL) {
        GUITHREADINFO guiThreadInfo = {sizeof(GUITHREADINFO)};
        if (GetGUIThreadInfo(GetWindowThreadProcessId(foreHwnd, NULL), &guiThreadInfo)) {
            return guiThreadInfo.hwndFocus ? guiThreadInfo.hwndFocus : foreHwnd;
        }
        return foreHwnd;
    }
    return NULL;
}

BOOL GetOpenStatus(HWND hwnd) {
    DWORD_PTR status;
    SendMessageTimeoutW(ImmGetDefaultIMEWnd(hwnd), WM_IME_CONTROL, IMC_GETOPENSTATUS, 0, 0, 200, &status);
    return status ? TRUE : FALSE;
}

void SetOpenStatus(BOOL status, HWND hwnd) {
    SendMessageTimeoutW(ImmGetDefaultIMEWnd(hwnd), WM_IME_CONTROL, IMC_SETOPENSTATUS, status, 0, 200, NULL);
}

DWORD GetConversionMode(HWND hwnd) {
    DWORD_PTR mode;
    SendMessageTimeoutW(ImmGetDefaultIMEWnd(hwnd), WM_IME_CONTROL, IMC_GETCONVERSIONMODE, 0, 0, 200, &mode);
    return mode;
}

void SetConversionMode(DWORD mode, HWND hwnd) {
    SendMessageTimeoutW(ImmGetDefaultIMEWnd(hwnd), WM_IME_CONTROL, IMC_SETCONVERSIONMODE, mode, 0, 200, NULL);
}
