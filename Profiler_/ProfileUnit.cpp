#include ".\profileunit.h"

#include <windows.h>
#include <stdio.h>

//#include "Clock.h"
#include "util_.h"


CProfileUnit::CProfileUnit()
{
	Clear();
}

CProfileUnit::CProfileUnit( unsigned int uikey, char *szProfileUnitName )
{
	key = uikey;

	memset( m_lpszName, 0, sizeof(char) * MAX_PROFILESAMPLENAME );	

	if( szProfileUnitName )
	{
		if( strlen( szProfileUnitName ) < MAX_PROFILESAMPLENAME )
		{
			strcpy_s( m_lpszName, szProfileUnitName );
		}
		else
		{
			memcpy( m_lpszName, szProfileUnitName, sizeof(char) * MAX_PROFILESAMPLENAME );
		}
	}
	else
	{
		char	szTempProfileName[MAX_PROFILESAMPLENAME];

		memset( szTempProfileName, 0, sizeof(char) * MAX_PROFILESAMPLENAME );

		sprintf_s( szTempProfileName, MAX_PROFILESAMPLENAME, "%d", uikey );

		strcpy_s( m_lpszName, MAX_PROFILESAMPLENAME, szTempProfileName );
	}
	
}

CProfileUnit::~CProfileUnit()
{

}

void CProfileUnit::Clear( void)
{
	ZeroMemory( m_lpszName, MAX_PROFILESAMPLENAME);
	m_llSumTime = 0;
	m_llSumTime_GrandTotal = 0;

	m_fLatestPercentage = 0.0f;
	m_fMaxPercentage = 0.0f;
	m_fMinPercentage = 100000000.0f;
	m_fTotalPercentage = 0.0f;

	m_ulCountInFrame = 0;
	m_ulLatestCount = 0;

	m_fAve_ms = 0.0f;
	m_fAve_ms_GrandTotal = 0.0f;
	m_ulCountInFrame_GrandTotal = 0;

	m_ulAve_tick = 0;
	m_ulAve_tick_GrandTotal = 0;

	key = 0;
}

void CProfileUnit::Init( unsigned int uikey, char *lpszName)
{
	Clear();

	key = uikey;
	strncpy_s( m_lpszName, lpszName, MAX_PROFILESAMPLENAME - 1);
}

BOOL CProfileUnit::CheckName( char *lpszName)
{
	return ( 0 == strcmp( m_lpszName, lpszName));
}

void CProfileUnit::BeginUnit_( unsigned int uiKey, char *lpszName, LONGLONG llTick )
{
	Init( uiKey, lpszName );
	m_llStartTick = llTick;
}

void CProfileUnit::EndUnit_( LONGLONG llTick)
{
	m_llSumTime += ( llTick - m_llStartTick);
	++m_ulCountInFrame;
}

void CProfileUnit::GenerateResult( LONGLONG llTotalTime, LONGLONG llGrandTotalTime, LONGLONG llTickPerMS )
{
	if ( m_llSumTime <= 0)
	{	// 한번도 호출 안한경우 그대로 유지
		return;
	}

	// 시간 계산
	m_llSumTime_GrandTotal += m_llSumTime;

	// 연산 점유율 계산
	m_fLatestPercentage = 100.0f * ( float)m_llSumTime / ( float)llTotalTime;
	m_fMaxPercentage = max ( m_fLatestPercentage, m_fMaxPercentage);
	m_fMinPercentage = min ( m_fMinPercentage, m_fLatestPercentage);
	m_fTotalPercentage = 100.0f * ( float)m_llSumTime_GrandTotal / ( float)llGrandTotalTime;

	// 호출 회수
	m_ulLatestCount = m_ulCountInFrame;
	m_ulCountInFrame_GrandTotal += m_ulCountInFrame;

	// 호출 시간 평균
	if( llTickPerMS != 1 && m_ulCountInFrame != 0 && m_ulCountInFrame_GrandTotal != 0 )
	{
		m_fAve_ms = (float)(m_llSumTime / m_ulCountInFrame) / (float)llTickPerMS;
		m_ulAve_tick = m_llSumTime / m_ulCountInFrame;

		m_fAve_ms_GrandTotal = (float)(m_llSumTime_GrandTotal / m_ulCountInFrame_GrandTotal) / (float)llTickPerMS;
		m_ulAve_tick_GrandTotal = m_llSumTime_GrandTotal / m_ulCountInFrame_GrandTotal;
	}

	m_llSumTime = 0;
	m_ulCountInFrame = 0;
}

void CProfileUnit::GetResult_Percentage( float *pfLatestPercentage, 
								   float *pfMaxPercentage, 
								   float *pfMinPercentage, 
								   float *pfTotalPercentage )
{
	*pfLatestPercentage = m_fLatestPercentage;
	*pfMaxPercentage = m_fMaxPercentage;
	*pfMinPercentage = m_fMinPercentage;
	*pfTotalPercentage = m_fTotalPercentage;
}

void CProfileUnit::GetResult_StatisticAve( float *pfAve_ms, 
											 unsigned long *pulAve_tick, 
											 float *pfAve_ms_GrandTotal,
											 unsigned long *pulAve_tick_GrandTotal )
{
	*pfAve_ms = m_fAve_ms;
	*pulAve_tick = m_ulAve_tick;
	*pfAve_ms_GrandTotal = m_fAve_ms_GrandTotal;
	*pulAve_tick_GrandTotal = m_ulAve_tick_GrandTotal;
}

void CProfileUnit::GetResultText( char *lpszText)
{
	sprintf_s( lpszText, MAX_PROFILERESULT, "Cur : %3.2f%% (%d CallTime), Max : %3.2f%%, Min : %3.2f%%, Avg : %3.2f%%", m_fLatestPercentage, m_ulLatestCount, m_fMaxPercentage, m_fMinPercentage, m_fTotalPercentage);
}

void CProfileUnit::GetResultText_Average( char *lpszText )
{
	/*sprintf_s( lpszText, "Call Time: %d, Total Call Time: %d, ProcessSec_Ave: %10.8fms, ProcessSec_TotalAve: %10.8fms",
	m_ulLatestCount, m_fAve_ms, m_fAve_ms_GrandTotal );*/
	sprintf_s( lpszText, MAX_PROFILERESULT, "Call Time: %d, Total Call Time: %d, ProcessSec_Ave: %f ms, ProcessSec_TotalAve: %f ms",
		m_ulLatestCount, m_ulCountInFrame_GrandTotal, m_fAve_ms, m_fAve_ms_GrandTotal );
}

void CProfileUnit::GetResultText_Average_Total( char *lpszText )
{
	/*sprintf_s( lpszText, "Total Call Time: %d, ProcessSec_TotalAve: %10.8fms",
	m_fAve_ms, m_fAve_ms_GrandTotal );*/
	sprintf_s( lpszText, MAX_PROFILERESULT, "Total Call Time: %d, ProcessSec_TotalAve: %f ms", m_ulLatestCount, m_fAve_ms_GrandTotal );
}
