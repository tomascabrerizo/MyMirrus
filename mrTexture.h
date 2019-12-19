#include "mrDataTypes.h"
#include "mrRGBAImage.h"
#include "mrScreen.h"
#include <d3d9.h>

#pragma once
class mrTexture
{
protected:
	/*Direct 3D Texture*/
	LPDIRECT3DTEXTURE9 m_pkD3DTexture;
	mrRGBAImage* m_pkRawImage;

	mrUInt32 m_iID;
	mrBool32 m_iHasAlpha;

	/*Maintenance members*/
	static mrUInt32 m_iActiveTexture;
	static mrUInt32 m_iCurrentID;

public:

	mrTexture(void);
	~mrTexture(void);

	/*Textures manipulation routines*/
	mrError32 Create(mrRGBAImage* pkRawImage);
	mrError32 Update(void);

	void SetRawImage(mrRGBAImage* pkRawImage);
	mrRGBAImage* GetRawImage(void);

	/*Texture maintenance methods*/
	void SetActiveTexture(void);
	mrUInt32 GetID(void);

	static mrUInt32 GetActiveTexture(void);
};

