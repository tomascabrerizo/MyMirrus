#include "mrSound.h"

/*Dedault Constructor*/
mrSound::mrSound(void)
{
	m_lpkSound = NULL;
}

/*Default Destructor*/
mrSound::~mrSound(void)
{
	if(m_lpkSound != NULL)
	{
		m_lpkSound = NULL;
	}
}

mrError32 mrSound::LoadFromFile(LPSTR lpszFilename)
{
	fstrean kWave;

	/*Open the wave file*/
	kWave.open(lpszFilename, ios::in | ios::binary);

	if(kWave.is_open())
	{
		mrInt8 aiID[5];
		/*Read the string RIFF identifier*/
		kWave.read(aiID, sizeof(mrInt8)*4);
		aiID[4] = '\0';

	}
}


