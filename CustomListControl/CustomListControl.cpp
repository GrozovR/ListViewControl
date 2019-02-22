#include "ListControlView.h"
#include "resource.h"

#define CUSTOM_ID     100
#define MARGIN          7

HINSTANCE hInst;
HWND ListHwnd;

INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;
	HWND dialogHWND;
	MSG msg;

	dialogHWND = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
	ShowWindow(dialogHWND, nCmdShow);
    
	while (GetMessage(&msg, nullptr, 0, 0))
    {	
		if (msg.message == WM_KEYDOWN) {
			
			continue;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);	   
		
    }
	
    return (int) msg.wParam;
}

INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		int msg;
		
	case (WM_KEYDOWN): {
			if (wParam == VK_UP || wParam == VK_DOWN)
			{
				PostMessage(ListHwnd, uMsg, wParam, lParam);
				break;
			}
		}
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDOK:
			PostQuitMessage(0);
			break;
		case IDCANCEL:
			PostQuitMessage(0);
			break;
		}
	}
	case WM_INITDIALOG:
		RegisterListCntrl();
		RECT ClientRect;
		GetClientRect(hwnd, &ClientRect);
		ListHwnd = CreateWindow(CUSTOM_LIST_CONTROL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | SS_OWNERDRAW,
			0, 0, ClientRect.right, (ClientRect.bottom - 40), hwnd, (HMENU)CUSTOM_ID, hInst, NULL);
		break;

	case WM_CLOSE:
		UnregisterListCntrl();
 		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
