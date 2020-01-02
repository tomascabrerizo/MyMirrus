#include "mrDataTypes.h"
#include "mrSoundPlayer.h"
#include "windows.h"
#include <fstream.h>
#include <dsound.h>

#pragma once

class mrSound
{
protected:
	/*Direct Sound Objetcs*/
	LPDIRECTSOUNDBUFFER m_lpkSound;

public:
	/*Constructor and Destructor*/
	mrSound(void);
	~mrSound(void);

	/*Sound manipulation routines*/
	mrError32 LoadFromFile(LPSTR lpszFilename);
	mrError32 SetVolume(mrUInt32 iVolume);
	mrError32 Play(mrUInt32 iLoop);
	mrError32 Stop(void);
};
