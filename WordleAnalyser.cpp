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
	m_wordList.ReadWords( WORD_LIST_FILENAME );

	DEBUG_MESSAGE( "Test %u %s\n", 15, "Melons" );

	return true;
}