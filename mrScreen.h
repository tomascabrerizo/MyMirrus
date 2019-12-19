#include "mrDataTypes.h"
#include "mrTimer.h"

#include <windows.h>
#include <d3d9.h>
#include <math.h>
#include <assert.h>

#pragma once

//Mirrus custom vertex structure
class mrVertex
{
public:
	//Transformed prositions
	FLOAT m_fX, m_fY, m_fZ, m_fRHW;
	//Color
	DWORD m_iColor;
	//Texture Coords
	FLOAT m_ftU, m_ftV;
};

/* Mirus custom vertex type */
#define D3DFVF_MIRUSVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
class mrScreen
{
protected:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	mrUInt32 m_iFPS;
	HWND m_hWindow;
	
	mrTimer m_kTimer;
	mrUInt32 m_iFormat;

	//Singleton
	static mrScreen* m_pKSingleton;

public:

	/*Constructor and destructor*/
	mrScreen(void);
	mrScreen(HWND hWindow);

	~mrScreen(void);

	/*Screen manipulation rutines*/
	mrError32 Init(HWND hWindow);
	mrError32 SetMode(mrUInt32 iFullScreen, mrUInt16 iWidth, mrUInt16 iHeight,
		mrUInt16 iDepth, bool bHardware);

	/*Render Routines*/
	mrError32 Clear(mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue, mrUInt8 iAlpha);
	mrError32 StartFrame(void);
	mrError32 EndFrame(void);

	/*Draw routines*/

	mrError32 DrawLine(mrReal32 fX1, mrReal32 fY1,
		mrReal32 fX2, mrReal32 fY2,
		mrUInt8 iRed, mrUInt8 iGreen,
		mrUInt8 iBlue, mrUInt8 iAlpha);
	mrError32 DrawRectangle(mrReal32 fX1, mrReal32 fY1,
		mrReal32 fX2, mrReal32 fY2,
		mrUInt8 iRed, mrUInt8 iGreen,
		mrUInt8 iBlue, mrUInt8 iAlpha);
	mrError32 DrawCircle(mrReal32 fCenterX, mrReal32 fCenterY,
		mrReal32 iRadius, mrUInt8 iRed, mrUInt8 iGreen,
		mrUInt8 iBlue, mrUInt8 iAlpha, mrUInt32 iVertices);
	
	/* Miscellaneous routines */
	mrBool32 IsModeSupported(mrUInt16 iWidth, mrUInt16 iHeight, mrUInt16 iDepth);
	void ShowCursor(mrUInt32 iShowCursor);

	/* Miscellaneous methods */
	LPDIRECT3DDEVICE9 GetDevice(void);
	mrUInt32 GetFPS(void);
	mrUInt32 GetFormat(void);
	mrUInt32 GetBitdepth(void);


	/*Singleton*/
	static mrScreen* GetSingleton(void);
};

