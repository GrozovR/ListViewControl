#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>

#define CUSTOM_LIST_CONTROL TEXT("ListControl")
#define CUSTOM_ID     100

void RegisterListCntrl();
void UnregisterListCntrl();

class CListControl {

public:
	CListControl();
	bool Paint( HWND hwnd );

	bool InsertItem( std::wstring item )
	{
		listStrings.push_back( item );
	}
	void SetHWND( HWND hwnd )
	{
		listHWND = hwnd;
	}

	void OnScroll( WPARAM wParam );
	void OnMouseClick( LPARAM lParam );
	void MoveUp();
	void MoveDown();

private:

	void updateScrollBar( RECT clientRect );
	void setFocus();

	enum {
		LINE_HEIGHT = 30,
		LINE_CLR_R = 200,
		LINE_CLR_G = 200,
		LINE_CLR_B = 200
	};

	HWND listHWND;
	int currentIndex;
	int scrollBarOffset;
	std::vector<std::wstring> listStrings;
};
