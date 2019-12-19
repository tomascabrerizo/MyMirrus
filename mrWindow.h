#include "mrDataTypes.h"
#include <windows.h>

#pragma once

class mrWindow
{
protected:
	WNDCLASS m_kWndClass;
	HWND m_hWindow;
	MSG m_kMessage;

public:
	mrWindow(void);
	~mrWindow(void);

	//Window manipulation functions
	mrError32 Create(HINSTANCE hInstance, const char* szTitle,
		mrInt iWidth = CW_USEDEFAULT,
		mrInt iHeight = CW_USEDEFAULT,
		mrUInt32 iStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	static LRESULT CALLBACK WndProc(HWND hWindow, UINT iMessage,
		WPARAM wParam, LPARAM lParam);

	void Run(void);

	//Custom functions
	virtual mrBool32 MessageHandler(UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual mrBool32 Frame(void) = 0;

	void SetPosition(mrInt iX, mrInt iY);
	void GetPosition(POINT* pkPosition);
	void SetSize(mrInt iWidth, mrInt iHeight);
	void GetSize(POINT* pkSize);

};

