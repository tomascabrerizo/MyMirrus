#include "mrDataTypes.h"
#include <windows.h>
#include <fstream>
#include <d3d9.h>

#pragma once
class mrRGBAImage
{
protected:
	mrUInt32 m_iWidth;
	mrUInt32 m_iHeight;

	/*Image buffer*/
	mrUInt32* m_piImageBUffer;

public:
	mrRGBAImage(void);
	~mrRGBAImage(void);

	/*Operators*/
	mrRGBAImage& operator = (mrRGBAImage& rkImage);

	/*Load image from windows bitmap*/
	mrError32 LoadFromBitmap(LPSTR lpszFilename);
	mrError32 LoadFromTarga(LPSTR lpszFilename);

	/*Image manipulator*/
	void SetColorKey(mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue);

	void SetWidth(mrUInt32 iWidth);
	void SetHegiht(mrUInt32 iHeight);
	void SetColor(mrUInt32 iX, mrUInt32 iY, mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue, mrUInt8 iAlpha);
	void SetImageBuffer(mrUInt32* pImage);

	mrUInt32 GetWidth(void);
	mrUInt32 GetHeight(void);
	mrUInt32 GetColor(mrUInt32 iX, mrUInt32 iY);
	mrUInt32* GetImageBuffer(void);

};

