#pragma once

#include <io.h>
#include <stdio.h>
#include "util_.h"
#include "array_.h"
#include "ProfileUnit.h"
#include "Singleton_.h"

class CProfilerFileParser
{
public:
	bool	_file_exist( const char* szFileName )
	{
		int iResult = 0;		

		iResult = _access( szFileName, 00);

		return ( iResult == 0 );	// 존재 하면 true
	}

	bool _file_save(	const char* szFileName, 
		const char* szBuffer,
		char* savedFileName  )
	{
		FILE			*pStream;

		char	szFullFileName[MAX_PATH];
		int		iUnit = 1;

		sprintf_s( szFullFileName, MAX_PATH, "%s.txt", szFileName );
		while( _file_exist( szFullFileName ) )
		{
			sprintf_s( szFullFileName, MAX_PATH, "%s_%d.txt", szFileName, iUnit );
			++iUnit;
		}

		int result = fopen_s( &pStream, szFullFileName, "w+t" );

		if( result != 0 )
		{
			return false;
		}

		strcpy_s( savedFileName, MAX_PATH, szFullFileName );

		fprintf( pStream, szBuffer );
		fclose( pStream );

		return true;
	}
};

class CProfiler : public CSingleton_<CProfiler>  
{
public:
	CProfiler();
	virtual ~CProfiler();
	void Clear( void);

private:
	CClock	*m_pClock;

protected:
	BOOL m_bFirstTime;	///< 정확도를 위해 첫 프레임은 제외한다.
	BOOL m_bProfiling;

	int m_iNumSample;

	array<CProfileUnit> m_arrProfileUnits;
	
	/// @name 전체 시간
	LONGLONG m_llTotalTimeStart;
	LONGLONG m_llTotalTimeEnd;
	LONGLONG m_llGrandTotalTime;

	LONGLONG m_llTickPerMS;

protected:
	void Reset( void);	///< 결과 초기화
	void Init( void);	///< measure 가 끝난 후에 할것
	
	void GenerateResultAll( void);	///< 결과 생성

public:
	void TotalBegin();
	void TotalEnd();

	void BeginUnit( unsigned int uikey, char *lpszName);
	void EndUnit( unsigned int iKey );

	int GetNumSample( void)	{ return ( m_iNumSample); }
	void GetResult_Percentage( unsigned int uiKey, float *pfLatestPercentage, float *pfMaxPercentage, float *pfMinPercentage, float *pfTotalPercentage);
	void GetResult_StatisticAve( unsigned int uiKey, 
									float *pfAve_ms, 
									unsigned long *pulAve_tick, 
									float *pfAve_ms_GrandTotal,
									unsigned long *pulAve_tick_GrandTotal ); 
	void GetResultText( unsigned int uiKey, char *lpszText );
};
