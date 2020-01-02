#include "mrDataTypes.h"
#include "mrInputManager.h"

#include <windows.h>
#include <dinput.h>

#pragma once

/*Joystick enumeration callback*/
BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

/*Mirrus Joystick Class*/

class mrJoystick
{
protected:
	LPDIRECTINPUTDEVICE8 m_lpkDiDevice;
	DIJOYSTATE2 m_kDeviceData;

public:
	/*Constructor Destructor*/
	mrJoystick(void);
	~mrJoystick(void);

	/*Joysticks manipulation methods*/
	mrError32 Init(HWND hWindow, mrInt32 iMin, mrInt32 iMax, mrInt32 iDeadZone);
	mrError32 Update(void);

	mrBool32 IsButtonDown(mrUInt32 iButton);
	mrBool32 IsButtonUp(mrUInt32 iButton);

	mrUInt32 GetXAxis(void);
	mrUInt32 GetYAxis(void);

};
