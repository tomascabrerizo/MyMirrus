#include "mrMouse.h"
/*Default Constructor*/
mrMouse::mrMouse(void)
{
	m_lpkDiDevice = NULL;
	ZeroMemory(&m_akDeviceData, sizeof(DIDEVICEOBJECTDATA) * 2);
}

/*Default Destructor*/
mrMouse::~mrMouse(void)
{
	if(m_lpkDiDevice != NULL)
	{
		m_lpkDiDevice->Unacquire();
		m_lpkDiDevice->Release();
		m_lpkDiDevice = NULL;
	}
}

mrError32 mrMouse::Init(HWND hWindow)
{
	/*Create Mouse device*/
	if(FAILED(mrInputManager::GetSingleton()->GetInput()->CreateDevice(
					GUID_SysMouse, &m_lpkDiDevice, NULL)))
	{
		return mrErrorMouseCreateDevice;
	}

	/*Set mouse data format*/
	if(FAILED(m_lpkDiDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return mrErrorMouseSetDataFormat;
	}

	/*Set mouse cooperative level*/
	if(FAILED(m_lpkDiDevice->SetCooperativeLevel(
					hWindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
	{
		return mrErrorMouseSetCooperativeLevel;
	}

	/*Set Buffered input 8 events*/
	DIPROPDWORD kDiProp;

	kDiProp.diph.dwSize = sizeof(DIPROPDWORD);
	kDiProp.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	kDiProp.diph.dwObj = 0;
	kDiProp.diph.dwHow = DIPH_DEVICE;
	kDiProp.dwData = 8;
	
	if(FAILED(m_lpkDiDevice->SetProperty(
					DIPROP_BUFFERSIZE, &kDiProp.diph)))
	{
		return mrErrorMouseSetCooperativeLevel;
	}
	
	/*Acquire mose*/
	m_lpkDiDevice->Acquire();

	return mrNoError;
}

mrError32 mrMouse::Update(void)
{
	/*Update mouse status*/
	mrUInt32 iElement;
	ZeroMemory(m_akDeviceData, sizeof(DIDEVICEOBJECTDATA)*8);

	/*Update each element*/
	for(iElement = 0; iElement < 8; iElement++)
	{
		mrUInt32 dwElements = 1;
		/*Get device data*/
		HRESULT hRet = m_lpkDiDevice->GetDeviceData(
				sizeof(DIDEVICEOBJECTDATA),
				&m_akDeviceData[iElement],
				&dwElements, 0);

		if(FAILED(hRet) && (hRet == DIERR_INPUTLOST))
		{
			/*Try to acquire mouse and get device data*/
			m_lpkDiDevice->Acquire();
			if(FAILED(m_lpkDiDevice->GetDeviceData(
							sizeof(DIDEVICEOBJECTDATA),
							&m_akDeviceData[iElement],
							&dwElements, 0)))
			{
				return mrErrorMouseGetDeviceData;
			}
		}
		else
		{
			return mrErrorMouseGetDeviceData;
		}
	}

	return mrNoError;
}

mrBool32 mrMouse::IsButtonDown(mrUInt32 iButton)
{
	mrUInt32 iElements;
	mrUInt32 iMouseButton;

	switch(iButton)
	{
	case 0:
		iMouseButton = DIMOFS_BUTTON0;
		break;
	case 1:
		iMouseButton = DIMOFS_BUTTON1;
		break;
	case 2:
		iMouseButton = DIMOFS_BUTTON2;
		break;
	case 3:
		iMouseButton = DIMOFS_BUTTON3;
		break;
	default:
		iMouseButton = DIMOFS_BUTTON0;
		break;		
	}

	/*Check for all the states to see if button was pressed*/
	for(iElements = 0; iElements < 8; iElements++)
	{
		if((m_akDeviceData[iElements].dwOfs == iMouseButton) &&
			(m_akDeviceData[iElements].dwData & (1<<7)))
		{
			return mrTrue;
		}
	}

	return mrFalse;
}

mrBool32 mrMouse::IsButtonUp(mrUInt32 iButton)
{
	mrUInt32 iElements;
	mrUInt32 iMouseButton;

	switch(iButton)
	{
	case 0:
		iMouseButton = DIMOFS_BUTTON0;
		break;
	case 1:
		iMouseButton = DIMOFS_BUTTON1;
		break;
	case 2:
		iMouseButton = DIMOFS_BUTTON2;
		break;
	case 3:
		iMouseButton = DIMOFS_BUTTON3;
		break;
	default:
		iMouseButton = DIMOFS_BUTTON0;
		break;		
	}

	/*Check for all the states to see if button was pressed*/
	for(iElements = 0; iElements < 8; iElements++)
	{
		if((m_akDeviceData[iElements].dwOfs == iMouseButton) &&
			(m_akDeviceData[iElements].dwData & (1<<7)))
		{
			return mrFalse;
		}
	}

	return mrTrue;
}

mrUInt32 mrMouse::GetXAxis(void)
{
	/*Return mouse horizontal axis*/
	mrUInt32 iElements;
	mrUInt32 iMovement;

	iMovement = 0;

	/*Sum all the relative x movement of the mouse in the last event*/
	for(iElements = 0; iElements < 8; iElements++)
	{
		if(DIMOFS_X == m_akDeviceData[iElements].dwOfs)
		{
			iMovement += m_akDeviceData[iElements].dwData;
		}
	}

	return iMovement;
}

mrUInt32 mrMouse::GetYAxis(void)
{
	/*Return mouse horizontal axis*/
	mrUInt32 iElements;
	mrUInt32 iMovement;

	iMovement = 0;

	/*Sum all the relative y movement of the mouse in the last event*/
	for(iElements = 0; iElements < 8; iElements++)
	{
		if(DIMOFS_Y == m_akDeviceData[iElements].dwOfs)
		{
			iMovement += m_akDeviceData[iElements].dwData;
		}
	}

	return iMovement;
}

mrError32 mrMouse::Clear(void)
{
	/*Clears the device buffer buffer*/
	ZeroMemory(m_akDeviceData, sizeof(DIDEVICEOBJECTDATA)*8);

	mrUInt32 dwItems = INFINITE;
	if(FAILED(m_lpkDiDevice->GetDeviceData(
					sizeof(DIDEVICEOBJECTDATA),
					NULL, &dwItems, 0)))
	{
		return mrErrorMouseGetDeviceData;
	}

	return mrNoError;
}
