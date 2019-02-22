#include "ListControlView.h"

CListControl listControl;

static LRESULT CALLBACK ListCntrlProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg ) {

	case WM_CREATE:
		listControl.SetHWND( hwnd );
		break;

	case WM_PAINT:
		listControl.Paint( hwnd );
		break;

	case WM_LBUTTONDOWN:
		listControl.OnMouseClick( lParam );
		break;

	case WM_VSCROLL:
		listControl.OnScroll( wParam );
		break;

	case WM_KEYUP: {
		if( wParam == VK_UP ) {
			listControl.MoveUp();
		} else if( wParam == VK_DOWN ) {
			listControl.MoveDown();
		}
		break; 
	}
	}
	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

void RegisterListCntrl()
{
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ListCntrlProc;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.lpszClassName = CUSTOM_LIST_CONTROL;
	RegisterClass( &wc );
}

void UnregisterListCntrl()
{
	UnregisterClass( CUSTOM_LIST_CONTROL, NULL );
}

CListControl::CListControl()
{
	currentIndex = 0;
	scrollBarOffset = 0;
	listStrings.push_back( TEXT( "one" ) );
	listStrings.push_back( TEXT( "two" ) );
	listStrings.push_back( TEXT( "three" ) );
	listStrings.push_back( TEXT( "four" ) );
	listStrings.push_back( TEXT( "five" ) );
	listStrings.push_back( TEXT( "six" ) );
	listStrings.push_back( TEXT( "seven" ) );
	listStrings.push_back( TEXT( "eight" ) );
	listStrings.push_back( TEXT( "nine" ) );
	listStrings.push_back( TEXT( "ten" ) );
	listStrings.push_back( TEXT( "eleven" ) );
	listStrings.push_back( L"twelve" );
}

bool CListControl::Paint( HWND hwnd )
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	listHWND = hwnd;

	GetClientRect( hwnd, &rect );

	hdc = BeginPaint( hwnd, &ps );

	SetTextColor( hdc, RGB( 0, 0, 0 ) );
	SetBkMode( hdc, TRANSPARENT );
	FillRect( hdc, &rect, (HBRUSH)COLOR_WINDOW );

	updateScrollBar( rect );

	RECT itemRect;
	itemRect.right = rect.right;
	itemRect.left = rect.left;

	for( int i{ 0 }; i < listStrings.size(); i++ ) {

		itemRect.top = rect.top + ((i - scrollBarOffset) * LINE_HEIGHT);
		itemRect.bottom = (i + 1 - scrollBarOffset)*LINE_HEIGHT;

		if( i == currentIndex ) {
			FillRect( hdc, &itemRect, CreateSolidBrush( RGB( LINE_CLR_R, LINE_CLR_G, LINE_CLR_B ) ) );
		}
		DrawText( hdc, listStrings[i].c_str(), -1, &itemRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
	}

	EndPaint( hwnd, &ps );

	return false;
}

void CListControl::OnScroll( WPARAM wParam )
{
	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask = SIF_ALL;
	GetScrollInfo( listHWND, SB_VERT, &si );

	scrollBarOffset = si.nPos;

	switch( LOWORD( wParam ) ) {
		// User clicked the top arrow.
	case SB_LINEUP:
		if( si.nPos != si.nMin )
			si.nPos -= 1;
		break;

		// User clicked the bottom arrow.
	case SB_LINEDOWN:
		if( si.nPos != si.nMax )
			si.nPos += 1;
		break;

		// User clicked the scroll bar shaft above the scroll box.
	case SB_PAGEUP:
		si.nPos -= si.nPage;
		break;

		// User clicked the scroll bar shaft below the scroll box.
	case SB_PAGEDOWN:
		si.nPos += si.nPage;
		break;

	case SB_THUMBPOSITION:
		si.nPos = HIWORD( wParam );

	default:
		break;
	}

	si.fMask = SIF_POS;
	SetScrollInfo( listHWND, SB_VERT, &si, TRUE );
	GetScrollInfo( listHWND, SB_VERT, &si );

	if( si.nPos != scrollBarOffset )
	{
		scrollBarOffset = si.nPos;
		InvalidateRgn( listHWND, NULL, FALSE );
	}
}

void CListControl::OnMouseClick( LPARAM lParam )
{
	int y = HIWORD( lParam );
	int itemNum = (y / LINE_HEIGHT) + scrollBarOffset;
	currentIndex = itemNum;
	InvalidateRgn( listHWND, NULL, FALSE );
}

void CListControl::MoveUp()
{
	if( currentIndex == 0 )
		return;
	currentIndex--;
	setFocus();
	InvalidateRgn( listHWND, NULL, FALSE );
}

void CListControl::MoveDown()
{
	if( currentIndex == listStrings.size() - 1 )
		return;
	currentIndex++;
	setFocus();
	InvalidateRgn( listHWND, NULL, FALSE );
}

void CListControl::updateScrollBar( RECT clientRect )
{
	int RectHeight = clientRect.bottom - clientRect.top;
	int listHeight = listStrings.size() * LINE_HEIGHT;

	if( RectHeight < listHeight ) {

		SCROLLINFO si;
		si.cbSize = sizeof( si );
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = listStrings.size() - 1;
		si.nPage = RectHeight / LINE_HEIGHT;
		si.nPos = scrollBarOffset;
		SetScrollInfo( listHWND, SB_VERT, &si, TRUE );
		ShowScrollBar( listHWND, SB_VERT, TRUE );

	} else {
		ShowScrollBar( listHWND, SB_VERT, FALSE );
		scrollBarOffset = 0;
	}
}

void CListControl::setFocus()
{
	RECT rect;
	GetClientRect( listHWND, &rect );
	int selectedItemTop = (currentIndex - scrollBarOffset)* LINE_HEIGHT;

	while( selectedItemTop < rect.top ) {
		SendMessage( listHWND, WM_VSCROLL, SB_LINEUP, NULL );
		//InvalidateRgn(listHWND, NULL, FALSE);
		GetClientRect( listHWND, &rect );
		selectedItemTop = (currentIndex - scrollBarOffset)* LINE_HEIGHT;
	}

	while( (selectedItemTop + LINE_HEIGHT) > rect.bottom ) {
		SendMessage( listHWND, WM_VSCROLL, SB_LINEDOWN, NULL );
		//InvalidateRgn(listHWND, NULL, FALSE);
		GetClientRect( listHWND, &rect );
		selectedItemTop = (currentIndex - scrollBarOffset)* LINE_HEIGHT;
	}
}
