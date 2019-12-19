#include "mrInputManager.h"

mrInputManager* mrInputManager::m_pkSingleton = NULL;

mrInputManager::mrInputManager(void)
{
	m_lpkDInput = NULL;
	
	assert(!m_pkSingleton);
	m_pkSingleton = this;
}

mrInputManager::~mrInputManager(void)
{
	if (m_lpkDInput != NULL)
	{
		m_lpkDInput->Release();
		m_lpkDInput = NULL;
	}
	assert(m_pkSingleton);
	m_pkSingleton = NULL;
}

mrError32 mrInputManager::Init(HINSTANCE hInstance)
{
	/*Create Direct Input Object*/
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpkDInput, NULL)))
	{
		return mrErrorDInputCreate;
	}

	return mrNoError;
}

LPDIRECTINPUT8 mrInputManager::GetInput(void)
{
	return m_lpkDInput;
}

mrInputManager* mrInputManager::GetSingleton(void)
{
	assert(m_pkSingleton);
	return m_pkSingleton;
}
