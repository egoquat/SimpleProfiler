#pragma once

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "ProfileProcess.h"

int main(void)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	CProfileProcess	MainProcess;
	int	iContinue = 1;

	while( iContinue == 1 )
	{
		int iDataCount = 0;
		int iSearchCount = 0;
		int iRepetition = 0;

		printf_s( "BinarySearch, LinearSearch Profiling.\nData Count?(10-100000)" );
		scanf_s( "%d", &iDataCount );
		printf_s( "\n" );

		printf_s( "Search Count?(1-10000)" );
		scanf_s( "%d", &iSearchCount );
		printf_s( "\n" );

		printf_s( "How much repetition?(1-10)" );
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
}
