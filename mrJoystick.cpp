#include "mrJoystick.h"

BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	LPDIRECTINPUTDEVICE8* lpkDevice = (LPDIRECTINPUTDEVICE8*)pContext;

	/*Create joystick device*/
	if(FAILED(mrInputManager::GetSingleton()->GetInput()->CreateDevice(
					pdidInstance->guidInstance, lpkDevice, NULL)))
	{
		return DIENUM_CONTINUE;
	}
	else
	{
		return DIENUM_STOP;
	}
}

/*Default constructor*/

mrJoystick::mrJoystick(void)
{
	m_lpkDiDevice = NULL;
	ZeroMemory(&m_kDeviceData, sizeof(DIDEVICEOBJECTDATA));
}

/*Default destructor*/

mrJoystick::~mrJoystick(void)
{
	if(m_lpkDiDevice != NULL)
	{
		m_lpkDiDevice->Unacquire();
		m_lpkDiDevice->Release();
		m_lpkDiDevice = NULL;
	}
}

/*Initialize the joysticks*/

mrError32 mrJoystick::Init(HWND hWindow, mrInt32 iMin, mrInt32 iMax, mrInt32 iDeadZone)
{
	/*Find first avaliable joystick*/
	if(FAILED(mrInputManager::GetSingleton()->GetInput()->EnumDevices(
					DI8DEVCLASS_GAMECTRL, EnumJoystickCallback, &m_lpkDiDevice,
					DIEDFL_ATTACHEDONLY)))
	{
		return mrErrorJoystickEnumDevices;
	}

	/*Set joystock data format*/
	if(FAILED(m_lpkDiDevice->SetDataFormat(&c_dfDIJoystick2)))
	{
		return mrErrorJoystickSetDataFormat;
	}
	
	/*Set joystick cooperative level*/
	if(FAILED(m_lpkDiDevice->SetCooperativeLevel(hWindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
	{
		return mrErrorJoystickSetCooperativeLevel;	
	}

	/*Set joystick axis range*/
	DIPROPRANGE kDIRange;

	kDIRange.diph.dwSize = sizeof(DIPROPRANGE);
	kDIRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	kDIRange.diph.dwHow = 0;
	kDIRange.diph.dwObj = DIPH_DEVICE;
	kDIRange.lMin = iMin;
	kDIRange.lMax = iMax;

	if(FAILED(m_lpkDiDevice->SetProperty(DIPROP_RANGE, &kDIRange.diph)))
	{
		return mrErrorJoystickSetProperty;
	}

	/*Set Joystick Dead Zone*/
	DIPROPDWORD kDIDeadZone;

	kDIDeadZone.diph.dwSize = sizeof(DIPROPDWORD);
	kDIDeadZone.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	kDIDeadZone.diph.dwHow = 0;
	kDIDeadZone.diph.dwObj = DIPH_DEVICE;
	kDIDeadZone.dwData = iDeadZone * 100;

	if(FAILED(m_lpkDiDevice->SetProperty(DIPROP_DEADZONE, &kDIDeadZone.diph)))
	{
		return mrErrorJoystickSetProperty;
	}

	/*Acquire joystick*/
	HRESULT hRet = m_lpkDiDevice->Poll();

	if(FAILED(hRet))
	{
		hRet = m_lpkDiDevice->Acquire();
		
		while(hRet == DIERR_INPUTLOST)
		{
			hRet = m_lpkDiDevice->Acquire();
		}
	}
	
	return mrNoError;
}

mrError32 mrJoystick::Update(void)
{
	/*Poll the Joystick*/
	if(FAILED(m_lpkDiDevice->Poll()))
	{
		/*Acquire joystick*/
		HRESULT hRet = m_lpkDiDevice->Acquire();

		if((FAILED(hRet)) && (hRet == DIERR_INPUTLOST))
		{
			m_lpkDiDevice->Acquire();
		}
		else
		{
			return mrErrorJoystickAcquire;
		}
	}

	/*Get Device Data*/
	if(FAILED(m_lpkDiDevice->GetDeviceState(sizeof(DIJOYSTATE2), &m_kDeviceData)))
	{
		return mrErrorJoystickGetDeviceState;
	}

	return mrNoError;
}

mrBool32 mrJoystick::IsButtonDown(mrUInt32 iButton)
{
	/*Check if button is pressed*/
	if(m_kDeviceData.rgbButtons[iButton]*(1<<7))
	{
		return mrTrue;
	}
	else
	{
		return mrFalse;
	}
}

mrBool32 mrJoystick::IsButtonUp(mrUInt32 iButton)
{
	/*Check if button is release*/
	if(m_kDeviceData.rgbButtons[iButton]*(1<<7))
	{
		return mrFalse;
	}
	else
	{
		return mrTrue;
	}
}

mrUInt32 mrJoystick::GetXAxis(void)
{
	return m_kDeviceData.lX;
}

mrUInt32 mrJoystick::GetYAxis(void)
{
	return m_kDeviceData.lY;
}

