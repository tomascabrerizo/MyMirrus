#include "mrDataTypes.h"
#include "mrRGBAImage.h"
#include "mrScreen.h"
#include <d3d9.h>

#pragma once
class mrSurface
{
protected:
	/*Direct3D surface*/
	LPDIRECT3DSURFACE9 m_pkD3DSurface;
	mrRGBAImage* m_pkRawImage;

public:
	/*Constuctor and Destructor*/
	mrSurface(void);
	~mrSurface(void);

	/*Surface manipulation rutines*/
	mrError32 Create(mrRGBAImage* pkRawImage);
	mrError32 Update(void);
	mrError32 Render(POINT* pkDestPoint, RECT* pkSourceRect = NULL);

	/*Surface maintenance methods*/
	void SetRawImage(mrRGBAImage* pkRawImage);
	mrRGBAImage* GetRawImage(void);
};

