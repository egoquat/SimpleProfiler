#include ".\profileprocess.h"

#include <math.h>

char	*_szFileName	= "Profile_op";

CProfileProcess::CProfileProcess(void)
{
	_init();
}

CProfileProcess::~CProfileProcess(void)
{
	_release();
}


void CProfileProcess::_binarysearch()
{
	bool	bPlus = false;

	for( int i = 0; i < m_iProcessCnt; ++i )
	{
		int iCurIndex	= m_arrSearchIdxs[i];
		Item &item_		= m_arrItems[iCurIndex];
		m_iFoundIdx		= m_arrItems.binary_search( item_ );
		_DEBUG_BREAK_IF(item_.key != m_iFoundIdx)
	}
}


void CProfileProcess::_binarykeysearch()
{
	bool	bPlus = false;

	for( int i = 0; i < m_iProcessCnt; ++i )
	{
		int iCurIndex	= m_arrSearchIdxs[i];
		Item &item_		= m_arrItems[iCurIndex];
		Item &item__	= m_arrItems.binary_search_bykey_returnvalue( item_.key );
		_DEBUG_BREAK_IF( item_.key != item__.key );
	}
}


void CProfileProcess::_linearsearch()
{
	bool	bPlus = false;

	for( int i = 0; i < m_iProcessCnt; ++i )
	{
		int iCurIndex = m_arrSearchIdxs[i];
		Item &item_ = m_arrItems[iCurIndex];
		m_iFoundIdx = m_arrItems.linear_search( item_ );
		_DEBUG_BREAK_IF( m_iFoundIdx != item_.key )
	}
}

void CProfileProcess::_process_binarysearch()
{
	m_arrItems.sort();
	for( int i = 0; i < m_iRepetition; ++i )
	{
		m_pProfiler->BeginUnit( (unsigned int)METHOD_ARRAY_BINARYSEARCH , _BINARYSEARCH );
		_binarysearch();
		m_pProfiler->EndUnit( (unsigned int)METHOD_ARRAY_BINARYSEARCH );
	}
}

void CProfileProcess::_process_binarykeysearch()
{
	m_arrItems.sortbykey();
	for( int i = 0; i < m_iRepetition; ++i )
	{
		m_pProfiler->BeginUnit( (unsigned int)METHOD_ARRAY_BINARYKEYSEARCH , _BINARYKEYSEARCH );
		_binarykeysearch();
		m_pProfiler->EndUnit( (unsigned int)METHOD_ARRAY_BINARYKEYSEARCH );
	}
}

void CProfileProcess::_process_linearsearch()
{
	for( int i = 0; i < m_iRepetition; ++i )
	{
		m_pProfiler->BeginUnit( (unsigned int)METHOD_ARRAY_LINEARSEARCH, _LINEARSEARCH );
		_linearsearch();
		m_pProfiler->EndUnit( (unsigned int)METHOD_ARRAY_LINEARSEARCH );
	}
}

void CProfileProcess::_process_all()
{
	m_pProfiler->TotalBegin();
	_process_binarysearch();
	_process_binarykeysearch();
	_process_linearsearch();
	m_pProfiler->TotalEnd();
}

void CProfileProcess::_process_input( int iDataCount, int iProcessCount, int iRepetition )
{
	m_iDataCnt = iDataCount;
	m_iProcessCnt = iProcessCount;
	m_iRepetition = iRepetition;
}

#include "..\Profiler_\FileIO_.h"

void CProfileProcess::_process_output()
{
	char	szBuffer[2048];
	int		iCur = 0;

	float	fAve_ms_bs, fAve_ms_bks, fAve_ms_ls, fAve_ms_GrandTotal_bs, fAve_ms_GrandTotal_bks, fAve_ms_GrandTotal_ls;
	unsigned long ulAve_tick_bs, ulAve_tick_bks, ulAve_tick_ls, ulAve_tick_GrandTotal_bs, ulAve_tick_GrandTotal_bks, ulAve_tick_GrandTotal_ls;

	m_pProfiler->GetResult_StatisticAve( METHOD_ARRAY_BINARYSEARCH, &fAve_ms_bs, 
														&ulAve_tick_bs, 
														&fAve_ms_GrandTotal_bs, 
														&ulAve_tick_GrandTotal_bs );

	m_pProfiler->GetResult_StatisticAve( METHOD_ARRAY_BINARYKEYSEARCH, &fAve_ms_bks, 
														&ulAve_tick_bks, 
														&fAve_ms_GrandTotal_bks, 
														&ulAve_tick_GrandTotal_bks );

	m_pProfiler->GetResult_StatisticAve( METHOD_ARRAY_LINEARSEARCH, &fAve_ms_ls, 
														&ulAve_tick_ls, 
														&fAve_ms_GrandTotal_ls, 
														&ulAve_tick_GrandTotal_ls );
	float	ratioBS				= (float)ulAve_tick_GrandTotal_ls / (float)ulAve_tick_GrandTotal_bs;
	float	ratioBKS			= (float)ulAve_tick_GrandTotal_ls / (float)ulAve_tick_GrandTotal_bks;
	float	ratioLS				= (float)ulAve_tick_GrandTotal_ls / (float)ulAve_tick_GrandTotal_ls;

	iCur += sprintf( szBuffer + iCur,  "Profile Binary / Linear Search - \n" );

	iCur += sprintf( szBuffer + iCur,  "Data count	: %d \n", m_iDataCnt );

	iCur += sprintf( szBuffer + iCur,  "Search count: %d \n", m_iProcessCnt );

	iCur += sprintf( szBuffer + iCur,  "Repetition	: %d \n", m_iRepetition );

	iCur += sprintf( szBuffer + iCur,  "\n" );

	iCur += sprintf( szBuffer + iCur,  "Total Search call Per Each : %d \n", m_iProcessCnt * m_iRepetition );

	iCur += sprintf( szBuffer + iCur,  "\n" );
	iCur += sprintf( szBuffer + iCur,  "1. Binary Search	: Ave_Sec(%f), Ave_Sec(%f)\n", 
					fAve_ms_bs * 0.001f, fAve_ms_GrandTotal_bs * 0.001f );
	
	iCur += sprintf( szBuffer + iCur,  "\n" );
	iCur += sprintf( szBuffer + iCur,  "2. Binary KeySearch : Ave_Sec(%f), Ave_Sec(%f)\n", 
					fAve_ms_bks * 0.001f, fAve_ms_GrandTotal_bks * 0.001f );

	iCur += sprintf( szBuffer + iCur,  "\n" );
	iCur += sprintf( szBuffer + iCur,  "3. Linear Search	: Ave_Sec(%f), Ave_Sec(%f)\n", 
					fAve_ms_ls * 0.001f, fAve_ms_GrandTotal_ls * 0.001f );

	iCur += sprintf( szBuffer + iCur,  "\n" );
	iCur += sprintf( szBuffer + iCur,  " binary : %3.2f Times Faster than linear.\n", ratioBS );

	iCur += sprintf( szBuffer + iCur,  "\n" );
	iCur += sprintf( szBuffer + iCur,  " binary key : %3.2f Times Faster than linear.\n", ratioBKS );

	iCur += sprintf( szBuffer + iCur,  "\n" );
	iCur += sprintf( szBuffer + iCur,  " linear : %3.2f Times Faster than linear.\n", ratioLS );

	printf_s( szBuffer );

	char	szErr[MAX_PATH + 24];
	char	savedFileName[MAX_PATH];
	
	memset(szErr, NULL, MAX_PATH + 24);
	memset(savedFileName, NULL, MAX_PATH);

	if( _file_save( _szFileName, szBuffer, savedFileName ) )
	{
		sprintf_s( szErr, MAX_PATH + 24, "Saved.\n(%s)", savedFileName );
		iCur += sprintf( szBuffer + iCur,  "\nResult saved:\n%s\n", savedFileName);
	}
	else
	{
		sprintf_s( szErr, MAX_PATH + 24, "Error save file.\n(%s)", savedFileName );
	}

	MessageBox( NULL, szBuffer, "Result", MB_OK );
}

void CProfileProcess::_init()
{
	m_pProfiler = NULL;
	m_Stream = NULL;

	m_iDataCnt = DEFAULT_DATA_CNT;
	m_iProcessCnt = DEFAULT_SEARCH_CNT;

	m_iRepetition = DEFAULT_REPETITION_CNT;

	m_pProfiler = m_pProfiler->GetThis_();
	m_iFoundIdx = 0;
}

void CProfileProcess::_release()
{
	_destruct_testData( m_iDataCnt );
}

void CProfileProcess::_construct_testData(int iDataCnt, int iSearchCnt)
{
	m_arrItems.reallocate( iDataCnt );

	for( int i = 0; i < iDataCnt; ++i )
	{
		Item item( i, _getRandFloat, _getRandFloat, _getRandFloat );

		m_arrItems.insert( item, i );
	}

	m_arrSearchIdxs.reallocate( iSearchCnt );

	bool overlapped = true;
	int idxData = 0;
	for(int i = 0; i < iSearchCnt; ++i)
	{
		overlapped = true;
		while(overlapped)
		{
			idxData = rand() % iDataCnt;
			overlapped = (m_arrSearchIdxs.binary_search(idxData)!=-1);
		}
		m_arrSearchIdxs.insert(idxData, i);
	}

	RandomMixArray_( m_arrSearchIdxs, 2 );
}

void CProfileProcess::_destruct_testData( int iDataCnt )
{
	m_arrItems.clear();
	m_arrSearchIdxs.clear();
}

void CProfileProcess::_construction()
{
	_construct_testData( m_iDataCnt, m_iProcessCnt );

	m_pProfiler = m_pProfiler->GetThis_();

	m_pClock = new CClock();
}

void CProfileProcess::_destruction()
{
	_destruct_testData( m_iDataCnt );

	delete m_pClock;

	m_pProfiler->DestroyThis();

	int i = 0;
}

