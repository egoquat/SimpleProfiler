#pragma once

#include "Clock.h"

#pragma warning ( disable : 4305 4503 4786 4291 4312 4311 4267 4244 4355  )

#define _getRandFloat	(float)((rand() %10000000) * 0.000001f)

#ifdef _DEBUG
#define	DEBUGOUTPUT(lpText)															\
{																						\
	static char szBuffer[512];															\
	sprintf_s(szBuffer, 512, "[File: %s][Line: %d]\n[Note : %s]", __FILE__, __LINE__, lpText);	\
	OutputDebugString( szBuffer );												\
}																					
#else
#define DEBUGOUTPUT(lpText)
#endif


#ifdef _DEBUG
#define	DEBUGMSG(lpText)															\
{																						\
	static char szBuffer[512];															\
	sprintf_s(szBuffer, 512, "[File: %s][Line: %d]\n[Note : %s]", __FILE__, __LINE__, lpText);	\
	MessageBox(NULL, szBuffer, "ERROR", MB_ICONERROR);									\
}																					

#else
#define DEBUGMSG(lpText)
#endif




#define	OUTPUT_NOTICE_MSG(lpText)															\
{																						\
	MessageBox(NULL, lpText, "Notice", MB_OK );									\
}

#define	OUTPUT_ERROR_MSG(lpText)															\
{																						\
	MessageBox(NULL, lpText, "Error", MB_OK );									\
}

#define	OUTPUT_NOTICE(lpText)															\
{																						\
	static char szBuffer[1024];															\
	sprintf_s(szBuffer, 1024, "%s", lpText);	\
	OutputDebugString( szBuffer );												\
}	

template <typename T>
void	_doshuffle( int iMaxCnt, int iShuffleCnt, T *pData )
{
	int iRndNum = 0;
	T tmp;

	CClock	*pClock = m_pClock;

	for( int i = 0; i < iShuffleCnt; ++i )
	{
		for( int j = 0; j < iMaxCnt; ++j )
		{
			iRndNum			= pClock->GetRandNum( 0, iMaxCnt );
			tmp				= pData[j];
			pData[j]		= pData[iRndNum];
			pData[iRndNum]	= tmp;
		}
	}
}
