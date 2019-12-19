#include "mrWindow.h"

mrWindow::mrWindow(void)
{
}

mrWindow::~mrWindow(void)
{
}

mrError32 mrWindow::Create(HINSTANCE hInstance, const char* szTitle, mrInt iWidth, mrInt iHeight, mrUInt32 iStyle)
{
	//Visual properties
	m_kWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_kWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_kWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	//System properties
	m_kWndClass.hInstance = hInstance;
	m_kWndClass.lpfnWndProc = WndProc;
	m_kWndClass.lpszClassName = "Mirus Window";

	//Extra properties
	m_kWndClass.lpszMenuName = NULL;
	m_kWndClass.cbClsExtra = NULL;
	m_kWndClass.cbWndExtra = NULL;

	//Try to register Class
	if (!RegisterClass(&m_kWndClass))
	{
		return mrErrorRegisterClass;
	}

	//Create the window
	m_hWindow = CreateWindow("Mirus Window", szTitle, iStyle, CW_USEDEFAULT,
		CW_USEDEFAULT, iWidth, iHeight, NULL, NULL, hInstance, (void*)this); 
	
	SetWindowText(m_hWindow, szTitle);

	return mrNoError;
}

LRESULT mrWindow::WndProc(HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	mrWindow* pkWindow = NULL;
	mrBool32 bProcessed = mrFalse;

	switch (iMessage)
	{
		//Windows is creating
	case WM_NCCREATE:
		SetWindowLong(hWindow, GWL_USERDATA, (long)(LPCREATESTRUCT(lParam))->lpCreateParams);
		break;

		//Window message let our own handler precess it
	default:
		pkWindow = (mrWindow*)GetWindowLong(hWindow, GWL_USERDATA);
		if (pkWindow != NULL)
		{
			bProcessed = pkWindow->MessageHandler(iMessage, wParam, lParam);
		}
		break;
	}

	//Message not precessed let windows handle it
	if (bProcessed == mrFalse)
	{
		return DefWindowProc(hWindow, iMessage, wParam ,lParam);
	}

	return 0;
}

void mrWindow::Run(void)
{
	while (1)
	{
		//Query to see if there any message in the queue
		if (PeekMessage(&m_kMessage, m_hWindow, 0, 0, PM_REMOVE))
		{
			if (m_kMessage.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&m_kMessage);
				DispatchMessageA(&m_kMessage);
			}
		}
		else
		{
			Frame();
		}
	}
}

mrBool32 mrWindow::MessageHandler(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return mrTrue;
		break;

	//Not handled let windows handle
	default:
		return mrFalse;
		break;
	}
}

void mrWindow::SetPosition(mrInt iX, mrInt iY)
{
	SetWindowPos(m_hWindow, HWND_TOP, iX, iY, 0, 0, SWP_NOSIZE);
}

void mrWindow::GetPosition(POINT* pkPosition)
{
	RECT rcWinow;
	POINT pPosition;

	//Get Window Position
	GetWindowRect(m_hWindow, &rcWinow);
	pPosition.x = rcWinow.left;
	pPosition.y - rcWinow.top;

	memcpy(pkPosition, &pPosition, sizeof(POINT));
}

void mrWindow::SetSize(mrInt iWidth, mrInt iHeight)
{
	SetWindowPos(m_hWindow, HWND_TOP, 0, 0, iWidth, iHeight, SWP_NOMOVE);
}

void mrWindow::GetSize(POINT* pkSize)
{
	RECT rcWindow;
	POINT pSize;
	/* Get window position */
	GetWindowRect(m_hWindow, &rcWindow);
	pSize.x = rcWindow.right - rcWindow.left;
	pSize.y = rcWindow.bottom - rcWindow.top;

	memcpy(pkSize, &pSize, sizeof(POINT));	
}