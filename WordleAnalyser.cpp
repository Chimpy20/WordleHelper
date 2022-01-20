#include "pch.h"
#include "WordleAnalyser.h"

const WCHAR* const WordleAnalyser::WORD_LIST_FILENAME = L"wordlist.txt";

WordleAnalyser::WordleAnalyser()
{
}

WordleAnalyser::~WordleAnalyser()
{
}

bool WordleAnalyser::Initialise()
{
	HANDLE wordListFileHandle = CreateFileW( WORD_LIST_FILENAME, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if (wordListFileHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	CloseHandle( wordListFileHandle );

	return true;
}