#include "mrDataTypes.h"
#include "mrInputManager.h"
#include <windows.h>
#include <dinput.h>

#pragma once
class mrKeyboard
{
protected:
	/*Direct Input Objects*/
	LPDIRECTINPUTDEVICE8 m_lpkDIDevice;
	/*Our key buffer*/
	mrUInt8 m_iKeyBuffer[256];

public:

	mrKeyboard(void);
	~mrKeyboard(void);

	/*Keyboard manipulation routines*/
	mrError32 Init(HWND hWindow);
	mrError32 Update(void);

	mrBool32 IsButtonDown(mrUInt32 iButton);
	mrBool32 IsButtonUp(mrUInt32 iButton);
};

