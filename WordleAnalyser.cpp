#include "pch.h"
#include "WordleAnalyser.h"

namespace wa
{

const WCHAR* const WordleAnalyser::WORD_LIST_FILENAME = L"wordlist.txt";

WordleAnalyser::WordleAnalyser():
	m_wordList( nullptr )
{
}

WordleAnalyser::~WordleAnalyser()
{
}

UINT WordleAnalyser::Initialise()
{
	m_wordList = new WordList();
	const UINT wordsRead = m_wordList->ReadWords( WORD_LIST_FILENAME );

	WordList* const filteredWords = new WordList();
	filteredWords->DuplicateFrom( *m_wordList );


	delete filteredWords;

	DEBUG_MESSAGE( "Test %u %s\n", 15, "Melons" );

	return wordsRead;
}

void WordleAnalyser::Shutdown()
{
	delete m_wordList;
}

void WordleAnalyser::GenerateStats()
{
	for( UINT letter = 0; letter < NumLetters; ++letter )
	{
		for( UINT wordLetterIndex = 0; wordLetterIndex < Word::WordLength; ++wordLetterIndex )
		{
			m_overallLetterCountsPerPosition[ letter ][ wordLetterIndex ] = 0;
		}
	}

}

}
