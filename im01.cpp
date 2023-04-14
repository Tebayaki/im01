#include <Windows.h>
#include <iostream>

#pragma comment(lib, "imm32")

#define IMC_GETCONVERSIONMODE 1
#define IMC_SETCONVERSIONMODE 2
#define IMC_GETOPENSTATUS 5
#define IMC_SETOPENSTATUS 6

namespace IME {
	HWND GetFocusedWindow();
	BOOL GetOpenStatus(HWND hwnd);
	void SetOpenStatus(BOOL status, HWND hwnd);
	DWORD GetConversionMode(HWND hwnd);
	void SetConversionMode(DWORD mode, HWND hwnd);
};

int main(int argc, char* argv[]) {
	HWND hwnd = IME::GetFocusedWindow();
	if (argc > 1) {
		IME::SetOpenStatus(argv[1][0] != '0' ? TRUE : FALSE, hwnd);
		if (argv[1][1] && argv[1][2]) {
			DWORD mode = atoi(argv[1] + 2);
			IME::SetConversionMode(mode, hwnd);
		}
	}
	else {
		std::cout << IME::GetOpenStatus(hwnd) << "-" << IME::GetConversionMode(hwnd);
	}
	return 0;
}

namespace IME {
	HWND GetFocusedWindow() {
		HWND foreHwnd = GetForegroundWindow();
		if (foreHwnd != nullptr) {
			GUITHREADINFO guiThreadInfo = {sizeof(GUITHREADINFO)};
			if (GetGUIThreadInfo(GetWindowThreadProcessId(foreHwnd, nullptr), &guiThreadInfo)) {
				return guiThreadInfo.hwndFocus ? guiThreadInfo.hwndFocus : foreHwnd;
			}
			return foreHwnd;
		}
		return nullptr;
	}

	BOOL GetOpenStatus(HWND hwnd) {
		DWORD_PTR status;
		SendMessageTimeoutW(ImmGetDefaultIMEWnd(hwnd), WM_IME_CONTROL, IMC_GETOPENSTATUS, 0, 0, 200, &status);
		return status ? TRUE : FALSE;
	}

	void SetOpenStatus(BOOL status, HWND hwnd) {
		SendMessageTimeoutW(ImmGetDefaultIMEWnd(hwnd), WM_IME_CONTROL, IMC_SETOPENSTATUS, status, 0, 200, nullptr);
	}

	DWORD GetConversionMode(HWND hwnd) {
		DWORD_PTR mode;
		SendMessageTimeoutW(ImmGetDefaultIMEWnd(hwnd), WM_IME_CONTROL, IMC_GETCONVERSIONMODE, 0, 0, 200, &mode);
		return mode;
	}

	void SetConversionMode(DWORD mode, HWND hwnd) {
		SendMessageTimeoutW(ImmGetDefaultIMEWnd(hwnd), WM_IME_CONTROL, IMC_SETCONVERSIONMODE, mode, 0, 200, nullptr);
	}
}
