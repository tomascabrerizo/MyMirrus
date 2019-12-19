#include "mrKeyboard.h"

mrKeyboard::mrKeyboard(void)
{
	m_lpkDIDevice = NULL;
	ZeroMemory(m_iKeyBuffer, sizeof(mrUInt8) * 256);
}

mrKeyboard::~mrKeyboard(void)
{
	if (m_lpkDIDevice != NULL)
	{
		m_lpkDIDevice->Unacquire();
		m_lpkDIDevice->Release();
		m_lpkDIDevice = NULL;
	}
}

mrError32 mrKeyboard::Init(HWND hWindow)
{
	/*Initializes the keyboard*/
	/*Create keyboard device*/
	if (FAILED(mrInputManager::GetSingleton()->GetInput()->CreateDevice(
		GUID_SysKeyboard, &m_lpkDIDevice, NULL)))
	{
		return mrErrorKeyboardCreateDevice;
	}

	/*Set Keyboard data format*/
	if (FAILED(m_lpkDIDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return mrErrorKeyboardSetDataFormat;
	}

	/*Set Keyboard cooperative level*/
	if (FAILED(m_lpkDIDevice->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return mrErrorKeyboardSetCooperativeLevel;
	}

	/*Acquire keyboard*/
	m_lpkDIDevice->Acquire();

	return mrNoError;
}

mrError32 mrKeyboard::Update(void)
{
	/*Get Device Data*/
	HRESULT hRet = m_lpkDIDevice->GetDeviceState(sizeof(mrUInt8) * 256, (LPVOID)&m_iKeyBuffer);

	if ((FAILED(hRet)) && (hRet == DIERR_INPUTLOST))
	{
		/*try to acquire keyboard*/
		if (FAILED(m_lpkDIDevice->Acquire()))
		{
			return mrErrorKeyboardAcquire;
		}
	}
	else
	{
		mrErrorKeyboardGetDeviceData;
	}

	return mrNoError;
}

mrBool32 mrKeyboard::IsButtonDown(mrUInt32 iButton)
{
	/*Returns if a button is down*/
	/*Checks if button is press*/
	if (m_iKeyBuffer[iButton]&(1<<7))
	{
		return mrTrue;
	}
	else
	{
		return mrFalse;
	}

}

mrBool32 mrKeyboard::IsButtonUp(mrUInt32 iButton)
{
	/*Checks if a button isnt pressed*/
	if (!(m_iKeyBuffer[iButton] & (1 << 7)))
	{
		return mrTrue;
	}
	else
	{
		return mrFalse;
	}
}
