#include "mrDataTypes.h"
#include "mrRGBAImage.h"
#include "mrScreen.h"
#include "mrTexture.h"

#pragma once

/*texture rectangle class*/
class mrRectText
{
public:
	mrReal32 m_fLeft;
	mrReal32 m_fTop;
	mrReal32 m_fRight;
	mrReal32 m_fBottom;
};

/*template cell class*/
class mrCellID
{
public:
	mrUInt32 m_iX;
	mrUInt32 m_iY;
};

/*Mirus Template class*/
class mrTemplateSet
{
protected:
	/*Texture information*/
	mrTexture* m_pkTexture;
	mrUInt32 m_iTextureWidth;
	mrUInt32 m_iTextureHeight;

	/*Cell Information*/
	mrUInt32 m_iCellWidth;
	mrUInt32 m_iCellHeight;

public:
	mrTemplateSet(void);
	~mrTemplateSet(void);

	/*Template manipulation routines*/
	void Create(mrTexture* pkTexture, mrUInt32 iTextureWidth, mrUInt32 iTextureHegiht,
		mrUInt32 iCellWidth, mrUInt32 m_iCellHeight);
	void GetUV(mrCellID kPosition, mrRectText* pkUVRect);

	/*Texture maintenance routines*/
	void SetActiveTextures(void);

	mrUInt32 GetTextureWidth(void);
	mrUInt32 GetTextureHeight(void);
	mrUInt32 GetCellWidth(void);
	mrUInt32 GetCellHeight(void);
};

