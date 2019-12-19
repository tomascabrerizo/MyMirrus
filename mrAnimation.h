#include "mrDataTypes.h"
#include "mrTemplateSet.h"
#include "mrScreen.h"
#include <d3d9.h>

#pragma once
class mrAnimation
{
protected:
	mrTemplateSet m_kTemplateSet;
	mrUInt32 m_iFrames;
	mrUInt32 m_iCurrentFrame;

	mrCellID* m_pkFramesID;

public:
	mrAnimation(void);
	~mrAnimation(void);

	/*Animation manipulation methods*/
	void Create(mrTemplateSet* pkTemplateSet, mrUInt32 iFrames, mrCellID* pkFramesID);
	void Update(void);
	mrError32 Render(RECT kDestRect, mrUInt32 iColor, mrReal32 fAngle);

	/* Animation maintenance methods */
	void SetCurrentFrame(mrUInt32 iFrame);
	mrUInt32 GetCurrentFrame(void);
};

