#include "mrDataTypes.h"
#include <windows.h>
#include <dsound.h>
#include <assert.h>

#pragma once

class mrSoundPlayer
{
protected:
	/*Direct sound objects*/
	LPDIRECTSOUND8 m_lpkDSound;

	/*Singleton*/
	static mrSoundPlayer* m_pkSingleton;
public:
	/*Constructor and Destructor*/
	mrSoundPlayer(void);
	~mrSoundPlayer(void);

	/*Sound player manipulation routines*/
	mrError32 Init(HWND hWindow);
	/*Sound player maintenance routines*/
	LPDIRECTSOUND8 GetSound(void);
	
	/*Singleton*/
	static mrSoundPlayer* GetSingleton(void);

};
