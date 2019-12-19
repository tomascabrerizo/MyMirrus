#include "mrDataTypes.h"
#include <windows.h>
#include <time.h>

#pragma once

class mrTimer
{
protected:
	//Hardware Timer variables
	LARGE_INTEGER m_iFrequency;
	LARGE_INTEGER m_iLastQuery;
	LARGE_INTEGER m_iDelta;

	//Time and date variables
	tm* m_pkTime;

public:
	mrTimer(void);
	~mrTimer(void);

	//Update the time viariables
	void Update(void);
	
	//Return Time information
	mrReal32 GetDelta(void);
	mrUInt32 GetSeconds(void);
	mrUInt32 GetMinutes(void);
	mrUInt32 GetHours(void);
	mrUInt32 GetDay(void);
	mrUInt32 GetMonth(void);
	mrUInt32 GetYear(void);
};

