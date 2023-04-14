#include <Windows.h>
#include <iostream>

#pragma comment(lib, "imm32")

#define IMC_GETCONVERSIONMODE 1
#define IMC_SETCONVERSIONMODE 2
#define IMC_GETOPENSTATUS 5
#define IMC_SETOPENSTATUS 6
#define IME_CMODE_OPENSTATUS 0x800

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
		DWORD code;
		code = atoi(argv[1]);
		IME::SetOpenStatus(IME_CMODE_OPENSTATUS & code, hwnd);
		IME::SetConversionMode(~IME_CMODE_OPENSTATUS & code, hwnd);
	}
	else {
		std::cout << ((IME::GetOpenStatus(hwnd) ? IME_CMODE_OPENSTATUS : 0) | IME::GetConversionMode(hwnd));
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
