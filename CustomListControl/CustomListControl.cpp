#include "ListControlView.h"

#define CUSTOM_ID     100
#define MARGIN          7

// Global Variables:
HINSTANCE hInst;
HWND ListHwnd;

bool initMainWindow();

static LRESULT CALLBACK
MainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;
	HWND mainWinHWND;
	MSG msg;

	RegisterListCntrl();
	initMainWindow();

	mainWinHWND = CreateWindow(_T("MainWindow"), _T("App Name"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 350, 250, NULL, NULL, hInstance, NULL);
	ShowWindow(mainWinHWND, nCmdShow);

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
		if (IsDialogMessage(mainWinHWND, &msg))
			continue;

		TranslateMessage(&msg);
		DispatchMessage(&msg);     
    }

	UnregisterListCntrl();

    return (int) msg.wParam;
}

static LRESULT CALLBACK
MainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_SIZE:
		if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED) {
			WORD cx = LOWORD(lParam);
			WORD cy = HIWORD(lParam);
			SetWindowPos(ListHwnd, NULL, MARGIN, MARGIN,
				cx - 2 * MARGIN, cy - 2 * MARGIN, SWP_NOZORDER);
		}
		break;

	case WM_CREATE:
		ListHwnd = CreateWindow(CUSTOM_LIST_CONTROL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | SS_OWNERDRAW,
			0, 0, 0, 0, hwnd, (HMENU)CUSTOM_ID, hInst, NULL);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


bool initMainWindow()
{
	WNDCLASS wc = { 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.lpszClassName = _T("MainWindow");

	RegisterClass(&wc);

	return true;
}