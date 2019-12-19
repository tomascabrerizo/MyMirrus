#include "mrDataTypes.h"
#include "mrTexture.h"
#include "mrAnimation.h"
#include <d3d9.h>

#pragma once
/*Mirrus Animated Blittable Object (ABO) class*/
class mrABO
{
protected:
	mrAnimation* m_pkAnimations;
	mrUInt32 m_iCurrentAnimation;

	mrUInt32 m_iColor;

	mrUInt32 m_iXPosition;
	mrUInt32 m_iYPosition;

	mrUInt32 m_iWidth;
	mrUInt32 m_iHeight;

	mrReal32 m_fAngle;
	mrReal32 m_fRadius;

	/*Only if loading from files*/
	mrTexture* m_pkTexture;

public:
	mrABO(void);
	~mrABO(void);

	/*ABO manipulation methods*/
	void Create(mrUInt32 iAnimations, mrAnimation* pkAnimations);
	void Update(void);

	void SetAnimation(mrUInt32 iAnimation, mrAnimation* pkAnimation);
	void LoadFromFile(LPSTR lpszFilename);

	mrError32 Render(void);

	void Rotate(mrReal32 fAngle, mrUInt32 iAccumulate);
	mrBool32 Collide(mrABO& rkABO, mrUInt32 iUseSphere);
	mrBool32 ContainsPoint(mrInt32 iX, mrInt32 iY);

	/*ABO maintenance methods*/
	void SetCurrentAnimation(mrUInt32 iAnimation);
	mrUInt32 GetCurrentAnimation(void);

	void SetPosition(mrUInt32 iX, mrUInt32 iY);
	void SetSize(mrUInt32 iWidth, mrUInt32 iHeight);
	void SetColor(mrUInt8 iAlpha, mrUInt8 iRed, mrUInt8 iGreen, mrUInt8 iBlue);
	void SetRadius(mrReal32 fRadius);

	mrUInt32 GetXPosition(void);
	mrUInt32 GetYPosition(void);
	mrUInt32 GetHeight(void);
	mrUInt32 GetWidth(void);
	mrUInt32 GetColor(void);
	mrReal32 GetDirection(void);
	mrReal32 GetRadius(void);
};

