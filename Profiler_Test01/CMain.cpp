#pragma once

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "ProfileProcess.h"

int main()
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	CProfileProcess	MainProcess;
	int	iContinue = 1;

	while( iContinue == 1 )
	{
		int iDataCount = 0;
		int iSearchCount = 0;
		int iRepetition = 0;
		printf_s( "BinarySearch, LinearSearch Profiling.\nHow many data to array for search? (recommand:100~10000)" );
		scanf_s( "%d", &iDataCount );
		printf_s( "\n" );

		printf_s( "How much search for profiling?" );
		scanf_s( "%d", &iSearchCount );
		printf_s( "\n" );

		printf_s( "How much repetition ? (recommand: 1~10)" );
		scanf_s( "%d", &iRepetition );
		printf_s( "\n" );

		MainProcess._process_input( iDataCount, iSearchCount, iRepetition );

		MainProcess._construction();

		MainProcess._process_all();
		MainProcess._process_output();

		MainProcess._destruction();

		printf_s( "Continue??? positive:1 nagative:0 " );
		scanf_s( "%d", &iContinue );
	}

	return 0;
}
