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

		return ( iResult == 0 );	// ���� �ϸ� true
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
	BOOL m_bFirstTime;	///< ��Ȯ���� ���� ù �������� �����Ѵ�.
	BOOL m_bProfiling;

	int m_iNumSample;

	array<CProfileUnit> m_arrProfileUnits;
	
	/// @name ��ü �ð�
	LONGLONG m_llTotalTimeStart;
	LONGLONG m_llTotalTimeEnd;
	LONGLONG m_llGrandTotalTime;

	LONGLONG m_llTickPerMS;

protected:
	void Reset( void);	///< ��� �ʱ�ȭ
	void Init( void);	///< measure �� ���� �Ŀ� �Ұ�
	
	void GenerateResultAll( void);	///< ��� ����

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
