#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>

void RegisterListCntrl();
void UnregisterListCntrl();

#define CUSTOM_LIST_CONTROL TEXT("ListControl")

class CListControl {	

public:
	CListControl();
	bool Paint(HWND hwnd);

	bool InsertItem(std::wstring item) 
	{ listStrings.push_back(item); }

	void OnScroll(WPARAM wParam);
	void OnMouseClick(LPARAM lParam);
	void moveUp(HWND hwnd);
	void moveDown(HWND hwnd);
	void SetHWND(HWND hwnd) { listHWND = hwnd; }

private:

	void updateScrollBar(RECT clientRect);
	void setFocus();

	enum {
		LINE_HEIGHT = 30
	};

	HWND listHWND;
	int currentIndex;
	int scrollBarOffset;
	std::vector<std::wstring> listStrings;
};
