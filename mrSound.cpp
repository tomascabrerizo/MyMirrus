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
	std::fstream kWave;

	/*Open the wave file*/
	kWave.open(lpszFilename, std::ios::in | std::ios::binary);

	if(kWave.is_open())
	{
		mrInt8 aiID[5];
		/*Read the string RIFF identifier*/
		kWave.read(aiID, sizeof(mrInt8)*4);
		aiID[4] = '\0';
		/*If not RIFF is not supported*/
		if(0 != strcmp(aiID, "RIFF"))
		{
			return mrErrorWaveNotSupported;
		}

		mrUInt32 iSize;
		/*Read the size of the wave*/
		kWave.read((mrInt8*)&iSize, sizeof(mrUInt32));

		/*Read the string WAVE identifire*/
		kWave.read(aiID, sizeof(mrInt8)*4);
		aiID[4] = '\0';
		/*if not WAVE not supported*/
		if(0 != strcmp(aiID, "WAVE"))
		{
			return mrErrorWaveNotSupported;
		}

		/*ignore 'fmt' string*/
		kWave.seekg(4, std::ios::cur);
		/*Read the 'fmt' chunk lenght*/
		mrUInt32 iFormatLenght;
		kWave.read((mrInt8*)&iFormatLenght, sizeof(mrUInt32));

		/*Read the WAVEFORMATEX structure*/
		WAVEFORMATEX kWaveFormat;
		kWave.read((mrInt8*)&kWaveFormat, sizeof(WAVEFORMATEX));

		kWave.seekg(2, std::ios::cur);
		kWaveFormat.cbSize = 0;

		/*Read the size of the wave data*/
		mrUInt32 iDataSize;
		kWave.read((mrInt8*)iDataSize, sizeof(mrUInt32));

		/*Read the sound data*/
		mrUInt8* pkSondBuffer = new mrUInt8[iDataSize];
		kWave.read((mrInt8*)pkSondBuffer, iDataSize);

		/*Fill direct sound buffer descripcion*/
		DSBUFFERDESC kBufferDesc;

		ZeroMemory(&kBufferDesc, sizeof(DSBUFFERDESC));
		kBufferDesc.dwSize = sizeof(DSBUFFERDESC);
		kBufferDesc.dwBufferBytes = iDataSize;
		kBufferDesc.lpwfxFormat = &kWaveFormat;
		kBufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;

		/*Create sound buffer*/
		if(FAILED(mrSoundPlayer::GetSingleton()->GetSound()->
					CreateSoundBuffer(&kBufferDesc, &m_lpkSound, NULL)))
		{
			return mrErrorCreateSoundBuffer;
		}

		/*Lock the sound buffer*/
		LPVOID lpvAudio;
		DWORD dwBytes;

		if(FAILED(m_lpkSound->Lock(0, 0, &lpvAudio, &dwBytes, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		{
			return mrErrorSoundBufferLock;
		}
		
		/*Copy the wave data to the direct sound buffer*/
		memcpy(lpvAudio, pkSondBuffer, dwBytes);
		m_lpkSound->Unlock(lpvAudio, dwBytes, NULL, 0);

		/*Delete the memory used by the wave data and close the file*/
		delete[] pkSondBuffer;
		kWave.close();
	}

	return mrNoError;	
}


mrError32 mrSound::SetVolume(mrUInt32 iVolume)
{
	if(FAILED(m_lpkSound->SetVolume((100-iVolume)*100)))
	{
		return mrErrorSoundSetVolume;
	}
	return mrNoError;
}

mrError32 mrSound::Play(mrUInt32 iLoop)
{
	/*Go to the begining of the sound*/
	m_lpkSound->SetCurrentPosition(0);

	/*Play sond*/
	if(FAILED(m_lpkSound->Play(0, NULL,
					(iLoop != 0) ? DSBPLAY_LOOPING : 0)))
	{
		return mrErrorPlay;
	}

	return mrNoError;
}

mrError32 mrSound::Stop(void)
{
	if(FAILED(m_lpkSound->Stop()))
	{
		return mrErrorStop;
	}
	
	return mrNoError;
}

