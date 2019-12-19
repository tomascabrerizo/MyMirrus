#include "mrScreen.h"

/*Singleton object*/
mrScreen* mrScreen::m_pKSingleton = NULL;

/*Default Contructor*/
mrScreen::mrScreen(void)
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_hWindow = NULL;
	m_iFPS = 0;

	assert(!m_pKSingleton);
	m_pKSingleton = this;
}

/*Assigment Constructor*/

mrScreen::mrScreen(HWND hWindow)
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_hWindow = hWindow;
	m_iFPS = 0;

	assert(!m_pKSingleton);
	m_pKSingleton = this;

	Init(hWindow);
}

mrScreen::~mrScreen(void)
{
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
	m_iFPS = 0;
	assert(m_pKSingleton);
	m_pKSingleton = NULL;
}

mrError32 mrScreen::Init(HWND hWindow)
{
	/*Create 3D Object*/
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	m_hWindow = hWindow;

	if (m_pD3D == NULL)
	{
		return mrErrorInitDirect3D;
	}

	return mrNoError;
}

mrError32 mrScreen::SetMode(mrUInt32 iFullScreen, mrUInt16 iWidth, mrUInt16 iHeight, mrUInt16 iDepth, bool bHardware)
{
	D3DPRESENT_PARAMETERS kPresentParams;
	mrUInt32 iDeviceType;
	/*Set type of device to create hardware or software*/
	if (!bHardware)
	{
		iDeviceType = D3DDEVTYPE_REF;
	}
	else
	{
		iDeviceType = D3DDEVTYPE_HAL;
	}

	/*reset present parametes and set swap effects*/
	ZeroMemory(&kPresentParams, sizeof(D3DPRESENT_PARAMETERS));
	kPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

	/*if Window mode*/
	if (!iFullScreen)
	{
		D3DDISPLAYMODE kCurrentMode;
		/*Get Current mode information*/
		if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &kCurrentMode)))
		{
			return mrErrorGetAdapterDisplayMode;
		}

		/*Set windowed mode and backbuffer format compatible with current format*/
		kPresentParams.Windowed = true;
		kPresentParams.BackBufferCount = 1;
		kPresentParams.BackBufferFormat = kCurrentMode.Format;

		/*Try to create device*/
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, (D3DDEVTYPE)iDeviceType, m_hWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &kPresentParams, &m_pD3DDevice)))
		{
			return mrErrorCreateDevice;
		}
	}
	else
	{
		/*Set fullscreen mode and full screen information*/
		kPresentParams.Windowed = false;
		kPresentParams.BackBufferCount = 1;
		kPresentParams.BackBufferWidth = iWidth;
		kPresentParams.BackBufferHeight = iHeight;

		kPresentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		kPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		/*If 16 bit, try to create the device using diferent 16 bit pixel format*/
		if (iDepth == 16)
		{
			kPresentParams.BackBufferFormat = D3DFMT_R5G6B5;
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				(D3DDEVTYPE)iDeviceType,
				m_hWindow,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&kPresentParams, &m_pD3DDevice)))
			{
				kPresentParams.BackBufferFormat = D3DFMT_X1R5G5B5;
				if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
					(D3DDEVTYPE)iDeviceType,
					m_hWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&kPresentParams, &m_pD3DDevice)))
				{
					kPresentParams.BackBufferFormat = D3DFMT_A1R5G5B5;
					if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
						(D3DDEVTYPE)iDeviceType,
						m_hWindow,
						D3DCREATE_SOFTWARE_VERTEXPROCESSING,
						&kPresentParams, &m_pD3DDevice)))
					{
						return mrErrorCreateDevice;
					}
				}
			}
		}
		else
		{
			kPresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				(D3DDEVTYPE)iDeviceType,
				m_hWindow,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&kPresentParams, &m_pD3DDevice)))
			{
				kPresentParams.BackBufferFormat = D3DFMT_X8R8G8B8;
				if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
					(D3DDEVTYPE)iDeviceType,
					m_hWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&kPresentParams, &m_pD3DDevice)))
				{
					return mrErrorCreateDevice;
				}
			}
		}
	}

	m_iFormat = kPresentParams.BackBufferFormat;

	/*Set render states*/
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	/*Set textures colors states*/
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	return mrNoError;
}

mrError32 mrScreen::Clear(mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue, mrUInt8 iAlpha)
{
	mrUInt32 iColor;
	iColor = D3DCOLOR_RGBA(iRed, iGreen, iBlue, iAlpha);

	/*Clear the screen to certain color*/
	if (FAILED(m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, iColor, 0, 0)))
	{
		return mrErrorClear;
	}

	return mrNoError;
}

mrError32 mrScreen::StartFrame(void)
{
	m_kTimer.Update();

	/*Start rendering*/
	if (FAILED(m_pD3DDevice->BeginScene()))
	{
		return mrErrorBeginScene;
	}

	return mrNoError;
}

mrError32 mrScreen::EndFrame(void)
{
	/*End Rendering*/
	if (FAILED(m_pD3DDevice->EndScene()))
	{
		return mrErrorEndScene;
	}

	/*Present Data to Screen*/
	if (FAILED(m_pD3DDevice->Present(NULL, NULL, NULL, NULL)))
	{
		return mrErrorPresent;
	}

	/*Calculate frame rate per second*/
	m_kTimer.Update();
	m_iFPS = (mrUInt32)(1 / m_kTimer.GetDelta());

	return mrNoError;
}

mrError32 mrScreen::DrawLine(mrReal32 fX1, mrReal32 fY1, mrReal32 fX2, mrReal32 fY2, mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue, mrUInt8 iAlpha)
{
	mrInt32 iColor;
	iColor = D3DCOLOR_RGBA(iRed, iGreen, iBlue, iAlpha);

	/*Create line vertices*/
	mrVertex pkVertices[] =
	{
		{fX1, fY1, 0, 1.0f, iColor, 0, 0},
		{fX2, fY2, 0, 1.0f, iColor, 0, 0},
	};

	/*Draw the line*/
	mrScreen::GetSingleton()->GetDevice()->SetFVF(D3DFVF_MIRUSVERTEX);
	if (FAILED(mrScreen::GetSingleton()->GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 2, pkVertices, sizeof(mrVertex))))
	{
		return mrErrorDrawPrimitive;
	}

	return mrNoError;
}

mrError32 mrScreen::DrawRectangle(mrReal32 fX1, mrReal32 fY1, mrReal32 fX2, mrReal32 fY2, mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue, mrUInt8 iAlpha)
{
	mrInt32 iColor;
	iColor = D3DCOLOR_RGBA(iRed, iGreen, iBlue, iAlpha);

	/*Create rectangle vertices*/
	mrVertex pkVertices[] =
	{
		{fX1, fY1, 0, 1.0f, iColor, 0, 0},
		{fX2, fY1, 0, 1.0f, iColor, 0, 0},
		{fX2, fY2, 0, 1.0f, iColor, 0, 0},
		{fX1, fY2, 0, 1.0f, iColor, 0, 0},
		{fX1, fY1, 0, 1.0f, iColor, 0, 0}
	};

	/*Draw the rectangle*/
	mrScreen::GetSingleton()->GetDevice()->SetFVF(D3DFVF_MIRUSVERTEX);
	if (FAILED(mrScreen::GetSingleton()->GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 4, pkVertices, sizeof(mrVertex))))
	{
		return mrErrorDrawPrimitive;
	}

	return mrNoError;
}

mrError32 mrScreen::DrawCircle(mrReal32 fCenterX, mrReal32 fCenterY, mrReal32 iRadius, mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue, mrUInt8 iAlpha, mrUInt32 iVertices)
{
	mrInt32 iColor;
	iColor = D3DCOLOR_RGBA(iRed, iGreen, iBlue, iAlpha);

	mrVertex* pkVertices;
	/*Allocate needed vertices*/
	pkVertices = new mrVertex[iVertices + 1];

	mrReal32 fAngle = 0;
	mrReal32 fComplete;

	mrUInt32 iVertex;

	/*Calculate each vertex position*/
	for (iVertex = 0; iVertex < iVertices; iVertex++)
	{
		/*precentage of circle already drawn*/
		fComplete = (mrReal32)iVertex / (mrReal32)iVertices;
		pkVertices[iVertex].m_fX = (mrReal32)((mrReal32)fCenterX + ((mrReal32)iRadius * cos(6.2831f * fComplete)));
		pkVertices[iVertex].m_fY = (mrReal32)((mrReal32)fCenterY + ((mrReal32)iRadius * sin(6.2831f * fComplete)));

		pkVertices[iVertex].m_fZ = 0;
		pkVertices[iVertex].m_fRHW = 1.0f;
		pkVertices[iVertex].m_iColor = iColor;
		pkVertices[iVertex].m_ftU = 0;
		pkVertices[iVertex].m_ftV = 0;
	}

	/*Close circle*/
	pkVertices[iVertex].m_fX = pkVertices[0].m_fX;
	pkVertices[iVertex].m_fY = pkVertices[0].m_fY;

	pkVertices[iVertex].m_fZ = 0;
	pkVertices[iVertex].m_fRHW = 1.0f;
	pkVertices[iVertex].m_iColor = iColor;
	pkVertices[iVertex].m_ftU = 0;
	pkVertices[iVertex].m_ftV = 0;

	/*Draw the circle*/
	mrScreen::GetSingleton()->GetDevice()->SetFVF(D3DFVF_MIRUSVERTEX);
	if (FAILED(mrScreen::GetSingleton()->GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, iVertices, pkVertices, sizeof(mrVertex))))
	{
		return mrErrorDrawPrimitive;
	}
	delete[] pkVertices;

	return mrNoError;
}

mrBool32 mrScreen::IsModeSupported(mrUInt16 iWidth, mrUInt16 iHeight, mrUInt16 iDepth)
{
	mrUInt32 iNumberOfModes;
	mrUInt32 iMode;
	D3DDISPLAYMODE kMode;

	/*Get numbers of availables modes*/
	iNumberOfModes = m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, (D3DFORMAT)m_iFormat);

	/*for each mode check if mode is equals*/
	for (iMode = 0; iMode < iNumberOfModes; iMode++)
	{
		/*Get mode information*/
		m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, (D3DFORMAT)m_iFormat, iMode, &kMode);
		
		/*Compare Dimensions*/
		if ((iWidth == kMode.Width)&&(iHeight == kMode.Height))
		{
			/*Compare bit depth*/
			if (iDepth == 16)
			{
				if ((kMode.Format == D3DFMT_R5G6B5)||
					(kMode.Format == D3DFMT_X1R5G5B5)||
					(kMode.Format == D3DFMT_A1R5G5B5))
				{
					return mrTrue;
				}
			}
			else
			{
				if ((kMode.Format == D3DFMT_A8R8G8B8) ||
					(kMode.Format == D3DFMT_X8R8G8B8))
				{
					return mrTrue;
				}
			}
		}
	}
	return mrFalse;
}

void mrScreen::ShowCursor(mrUInt32 iShowCursor)
{
	m_pD3DDevice->ShowCursor(iShowCursor);
}

LPDIRECT3DDEVICE9 mrScreen::GetDevice(void)
{
	return m_pD3DDevice;
}

mrUInt32 mrScreen::GetFPS(void)
{
	return m_iFPS;
}

mrUInt32 mrScreen::GetFormat(void)
{
	return m_iFormat;
}

mrUInt32 mrScreen::GetBitdepth(void)
{
	mrUInt32 iBitdepth;

	switch (m_iFormat)
	{
	case D3DFMT_R5G6B5:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
		iBitdepth = 16;
		break;
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
		iBitdepth = 32;
		break;
	}

	return iBitdepth;
}

mrScreen* mrScreen::GetSingleton(void)
{
	assert(m_pKSingleton);
	return m_pKSingleton;
}
