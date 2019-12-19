#include "mrDataTypes.h"
#include <windows.h>
#include <dinput.h>
#include <assert.h>

#pragma once
class mrInputManager
{
protected:
	LPDIRECTINPUT8 m_lpkDInput;

	/*Singleton*/
	static mrInputManager* m_pkSingleton;

public:
	mrInputManager(void);
	~mrInputManager(void);

	/*Input device manipulation routines*/
	mrError32 Init(HINSTANCE hInstance);
	LPDIRECTINPUT8 GetInput(void);

	/*Singleton*/
	static mrInputManager* GetSingleton(void);
};

