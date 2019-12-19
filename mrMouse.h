#include "mrDataTypes.h"
#include "mrInputManager.h"
#include <windows.h>
#include <dinput.h>

#pragma once
class mrMouse
{
protected:
	/*Direct input objects*/
	LPDIRECTINPUTDEVICE8 m_lpkDiDevice;
	DIDEVICEOBJECTDATA m_akDeviceData[8];
public:
	/*Constructor and destructor*/
	mrMouse(void);
	~mrMouse(void);

	/*Mouse manipulation routines*/
	mrError32 Init(HWND hWindow);
	mrError32 Update(void);

	mrBool32 IsButtonDown(mrUInt32 iButton);
	mrBool32 IsButtomUp(mrUInt32 iButton);
	mrUInt32 GetXAxis(void);
	mrUInt32 GetYAxis(void);
	
	mrError32 Clear(void);
};

