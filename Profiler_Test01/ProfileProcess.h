#pragma once

#include <iostream>
#include <io.h>
#include <math.h>
#include "..\profiler_\Clock.h"
#include "..\Profiler_\array_.h"
#include "..\Profiler_\Profiler.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Profiler_\\Debug\\Profiler_.lib")
#else
#pragma comment(lib, "..\\Profiler_\\Release\\Profiler_.lib")
#endif 

#define DEFAULT_REPETITION_CNT		2
#define DEFAULT_DATA_CNT			10000
#define DEFAULT_SEARCH_CNT			1000

#define EPSILON	0.00000001f

enum	METHOD {	METHOD_ARRAY_START_ = -1,
					METHOD_ARRAY_BINARYSEARCH,
					METHOD_ARRAY_BINARYKEYSEARCH,
					METHOD_ARRAY_LINEARSEARCH,
					METHOD_CNT };

#define _BINARYSEARCH		TEXT("BINARY_SEARCH")
#define _BINARYKEYSEARCH	TEXT("BINARY_KEY_SEARCH")
#define _LINEARSEARCH		TEXT("LINEAR_SEARCH")


class Item
{
public:
	unsigned int key;
public:
	float fX;
	float fY;
	float fZ;

public:
	BOOL operator==(const Item &rhs)
	{
		return key == rhs.key;
	}

	BOOL operator<=(const Item &rhs)
	{
		return key <= rhs.key;
	}

	BOOL operator >=(const Item &rhs)
	{
		return key >= rhs.key;
	}

	BOOL operator <(const Item &rhs)
	{
		return key < rhs.key;
	}

	BOOL operator >(const Item &rhs)
	{
		return key > rhs.key;
	}


	BOOL operator==(const Item &rhs) const
	{
		return key == rhs.key;
	}

	BOOL operator<=(const Item &rhs) const
	{
		return key <= rhs.key;
	}

	BOOL operator >=(const Item &rhs) const
	{
		return key >= rhs.key;
	}

	BOOL operator <(const Item &rhs) const
	{
		return key < rhs.key;
	}

	BOOL operator >(const Item &rhs) const
	{
		return key > rhs.key;
	}

public:
	Item()
	{
		key = 0;
	};

	Item( unsigned int uikey, float fX_, float fY_, float fZ_ )
	{
		key = uikey;
		fX = fX_;
		fY = fY_;
		fZ = fZ_;
	};
};

class CProfileProcess
{
public:
	int				m_iDataCnt;
	int				m_iProcessCnt;
	int				m_iRepetition;

	CClock			*m_pClock;
	CProfiler		*m_pProfiler;

	array<Item>		m_arrItems;
	array<int>		m_arrSearchIdxs;

	FILE			*m_Stream;

private:
	int		m_iFoundIdx;

public:
	void _binarysearch();
	void _binarykeysearch();
	void _linearsearch();

	void _process_binarysearch();
	void _process_binarykeysearch();
	void _process_linearsearch();

	void _process_input( int iDataCount, int iSearchCount, int iRepetition );
	void _process_output();

	bool _output_file(	const char* szFileName, 
		const char* szBuffer,
		char* savedFileName  );

	void _process_all();

	void _construct_testData( int iDataCnt, int iSearchCnt );
	void _destruct_testData( int iDataCnt );

	void _construction();
	void _destruction();

	void _init();
	void _release();
public:
	CProfileProcess(void);
	~CProfileProcess(void);
};

