#include "ListControlView.h"
#include "resource.h"

HINSTANCE hInst;
HWND ListHwnd;

INT_PTR CALLBACK DialogProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

int APIENTRY wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );
	hInst = hInstance;
	HWND dialogHWND;
	MSG msg;

	dialogHWND = CreateDialog( hInstance, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, DialogProc );
	ShowWindow( dialogHWND, nCmdShow );

	while( GetMessage( &msg, nullptr, 0, 0 ) )
	{
		/* Для поддержки keyboard interface в диалоге
		if( !IsDialogMessage( dialogHWND, &msg ) ) {
			TranslateMessage( &msg ); 
			DispatchMessage( &msg );
		}*/		
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return (int)msg.wParam;
}

INT_PTR CALLBACK DialogProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg ) 
	{

	case WM_COMMAND: {
		int wmId = LOWORD( wParam );
		switch( wmId )
		{
		case IDOK :
			PostMessage( hwnd, WM_CLOSE, 0, 0 );
			return TRUE;
		case IDCANCEL:
			PostMessage( hwnd, WM_CLOSE, 0, 0 );
			return TRUE;
		}
	}
	case WM_INITDIALOG:
	{
		RegisterListCntrl();
		RECT ClientRect;
		GetClientRect( hwnd, &ClientRect );
		ListHwnd = CreateWindow( CUSTOM_LIST_CONTROL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL,
			0, 0, ClientRect.right, ( ClientRect.bottom - 40 ), hwnd, NULL, hInst, NULL );
		SetFocus( ListHwnd );
		return FALSE; // Что бы система не установила default focus
	}
	case WM_CLOSE:
	{
		UnregisterListCntrl();
		PostQuitMessage( 0 );
		return TRUE;
	}
	default:
		return FALSE;
	}
}
