#include "mrSoundPlayer.h"

mrSoundPlayer* mrSoundPlayer::m_pkSingleton;

/*Default constuctor*/
mrSoundPlayer::mrSoundPlayer(void)
{
	m_lpkDSound = NULL;

	assert(!m_pkSingleton);
	m_pkSingleton = this;
}

/*Default Destructor*/
mrSoundPlayer::~mrSoundPlayer(void)
{
	if(m_lpkDSound != NULL)
	{
		m_lpkDSound->Release();
		m_lpkDSound = NULL;
	}

	assert(m_pkSingleton);
	m_pkSingleton = NULL;
}

mrError32 mrSoundPlayer::Init(HWND hWindow)
{
	/*Creates direct sound object*/
	DirectSoundCreate8(NULL, &m_lpkDSound, NULL);

	if(m_lpkDSound == NULL)
	{
		return mrErrorCreateSoundDevice;
	}

	/*Set Direct sound cooperative level*/
	if(FAILED(m_lpkDSound->SetCooperativeLevel(hWindow, DSSCL_NORMAL)))
	{
		return mrErrorSetCoperativeLevel;
	}

	return mrNoError;
}

LPDIRECTSOUND8 mrSoundPlayer::GetSound(void)
{
	return m_lpkDSound;
}

mrSoundPlayer* mrSoundPlayer::GetSingleton(void)
{
	return m_pkSingleton;
}

