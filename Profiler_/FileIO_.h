#pragma once

#include <io.h>

bool	_file_exist( const char* szFileName )
{
	int iResult = 0;		

	iResult = _access( szFileName, 00);

	return ( iResult == 0 );	// 존재 하면 true
}

bool _file_save(	const char* szFileName, 
				const char* szBuffer,
				char* outSavedFileName  )
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

	if( 0 != result ) 
	{
		return false;
	}

	GetCurrentDirectory( MAX_PATH, outSavedFileName );
	strcat( outSavedFileName, "\\" );
	strcat( outSavedFileName, szFullFileName );

	fprintf_s( pStream, szBuffer );
	fclose( pStream );

	return true;
}
