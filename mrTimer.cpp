#define _CRT_SECURE_NO_WARNINGS
#include "mrTimer.h"

mrTimer::mrTimer(void)
{
	QueryPerformanceFrequency(&m_iFrequency);
	Update();
}

mrTimer::~mrTimer(void)
{
	m_iFrequency.QuadPart = 0;
	m_iLastQuery.QuadPart = 0;
}

void mrTimer::Update(void)
{
	LARGE_INTEGER kTempTimer;
	time_t iTempTimeDate;

	//Get current information and calculate the diference
	QueryPerformanceCounter(&kTempTimer);
	m_iDelta.QuadPart = kTempTimer.QuadPart - m_iLastQuery.QuadPart;

	//Save Current time information
	m_iLastQuery.QuadPart = kTempTimer.QuadPart;

	//Get current time and date
	time(&iTempTimeDate);
	m_pkTime = localtime(&iTempTimeDate);
}

mrReal32 mrTimer::GetDelta(void)
{
	/* Convert to float and calculate delta in seconds */
	return (mrReal32)(m_iDelta.QuadPart) / 
		(mrReal32)(m_iFrequency.QuadPart);
}

mrUInt32 mrTimer::GetSeconds(void)
{
	return m_pkTime->tm_sec;
}

mrUInt32 mrTimer::GetMinutes(void)
{
	return m_pkTime->tm_min;
}

mrUInt32 mrTimer::GetHours(void)
{
	return m_pkTime->tm_hour;
}

mrUInt32 mrTimer::GetDay(void)
{
	return m_pkTime->tm_mday;
}

mrUInt32 mrTimer::GetMonth(void)
{
	return m_pkTime->tm_mon;
}

mrUInt32 mrTimer::GetYear(void)
{
	return m_pkTime->tm_year;
}
