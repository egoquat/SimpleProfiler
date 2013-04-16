#include ".\profiler.h"

#include <windows.h>
#include <assert.h>
//#include "Clock.h"

CProfiler::CProfiler()
{
	Clear();
	m_pClock = new CClock();
}

CProfiler::~CProfiler()
{
	delete m_pClock;
	Clear();
	for( unsigned int i = 0; i < m_arrProfileUnits.size(); ++i)
	{
		m_arrProfileUnits[i].Clear();
	}
	m_arrProfileUnits.clear();
}

void CProfiler::Clear( void)
{
	//m_bFirstTime = TRUE;
	m_bFirstTime = FALSE;
	m_iNumSample = 0;
	m_llGrandTotalTime = 0;
	m_bProfiling = FALSE;
}

void CProfiler::Init( void)
{
	//if ( m_pClock )
	{
		static CClock s_Clock;
		s_Clock.AutoMeasure( 3000);

		m_llTickPerMS = s_Clock.GetCustomTickPerMillisecond();
	}
	//Reset();
}

void CProfiler::TotalBegin( )
{
	Init();
	Reset();
	m_bProfiling = TRUE;
}

void CProfiler::TotalEnd()
{
	m_llTotalTimeEnd = m_pClock->GetCustomTick();
	m_bProfiling = FALSE;

	GenerateResultAll();
}

void CProfiler::Reset( void)
{
	Clear();
	for( unsigned int i = 0; i < m_arrProfileUnits.size(); ++i)
	{
		m_arrProfileUnits[i].Clear();
	}
	m_llTotalTimeStart = m_pClock->GetCustomTick();
}

void CProfiler::BeginUnit( unsigned int uikey, char *lpszName )
{
	if ( m_bFirstTime)
	{
		return;
	}

	LONGLONG llTick = m_pClock->GetCustomTick();

	CProfileUnit ProfileNewUnit( uikey, lpszName );
	//m_arrProfileUnits.insert( ProfileNewUnit );
	m_arrProfileUnits.push_back( ProfileNewUnit );

	int iIdx = m_arrProfileUnits.binary_search_bykey_returnindex( uikey );

	if( -1 ==  iIdx )
	{
		

		return;
	}

	CProfileUnit &ProfileUnit = m_arrProfileUnits[iIdx];
	ProfileUnit.BeginUnit_( uikey, lpszName, llTick );
}

void CProfiler::EndUnit( unsigned int uikey )
{
	if ( m_bFirstTime)
	{
		return;
	}

	LONGLONG llTick = m_pClock->GetCustomTick();
	
	int uiIdx = m_arrProfileUnits.binary_search_bykey_returnindex( uikey );

	CProfileUnit &ProfileUnit = m_arrProfileUnits[uiIdx];

	ProfileUnit.EndUnit_( llTick );
}

void CProfiler::GenerateResultAll( void )
{
	LONGLONG llTotalTime = m_llTotalTimeEnd - m_llTotalTimeStart;
	if ( !m_bFirstTime)
	{
		m_llGrandTotalTime += llTotalTime;

		for( unsigned int iCur = 0; iCur < m_arrProfileUnits.size(); ++iCur )
		{
			m_arrProfileUnits[iCur].GenerateResult( llTotalTime, m_llGrandTotalTime, m_llTickPerMS );
		}
	}
}

void CProfiler::GetResult_Percentage( unsigned int uiKey, 
									float *pfLatestPercentage, 
									float *pfMaxPercentage, 
									float *pfMinPercentage, 
									float *pfTotalPercentage)
{
	CProfileUnit &ProfileUnit = m_arrProfileUnits.binary_search_bykey_returnvalue( uiKey );
	ProfileUnit.GetResult_Percentage( pfLatestPercentage, pfMaxPercentage, pfMinPercentage, pfTotalPercentage);
}

void CProfiler::GetResult_StatisticAve( unsigned int uiKey, 
										float *pfAve_ms, 
										unsigned long *pulAve_tick, 
										float *pfAve_ms_GrandTotal,
										unsigned long *pulAve_tick_GrandTotal )
{
	CProfileUnit &ProfileUnit = m_arrProfileUnits.binary_search_bykey_returnvalue( uiKey );
	ProfileUnit.GetResult_StatisticAve( pfAve_ms, pulAve_tick, pfAve_ms_GrandTotal, pulAve_tick_GrandTotal);
}

void CProfiler::GetResultText( unsigned int uiKey, char *lpszText )
{
	CProfileUnit ProfileUnit = m_arrProfileUnits.binary_search_bykey_returnvalue( uiKey );
	//Profile.GetResult_StatisticAve( pfAve_ms, pulAve_tick, pfAve_ms_GrandTotal, pulAve_tick_GrandTotal);
}
