#include "pch.h"
#include "WordleAnalyser.h"
#include "FilterWord.h"

namespace wa
{

const WCHAR* const WordleAnalyser::WORD_LIST_FILENAME = L"wordlist.txt";

const CHAR testFilterLetters1[ Word::WordLength ] =
{
	'r',
	'a',
	'i',
	's',
	'e'
};

const FilterLetterState testFilterLetterStates1[ Word::WordLength ] = 
{
	FilterLetterState::WrongPosition,
	FilterLetterState::Incorrect,
	FilterLetterState::Incorrect,
	FilterLetterState::Incorrect,
	FilterLetterState::Incorrect
};

const CHAR testFilterLetters2[ Word::WordLength ] =
{
	't',
	'o',
	'u',
	'g',
	'h'
};

const FilterLetterState testFilterLetterStates2[ Word::WordLength ] =
{
	FilterLetterState::Incorrect,
	FilterLetterState::Incorrect,
	FilterLetterState::Correct,
	FilterLetterState::WrongPosition,
	FilterLetterState::Incorrect
};

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
	UINT uNumFilteredWords = filteredWords->Filter( FilterWord( testFilterLetters1, testFilterLetterStates1 ) );
	io::OutputMessage( "Filtered down to %u words\n", uNumFilteredWords );
	uNumFilteredWords = filteredWords->Filter( FilterWord( testFilterLetters2, testFilterLetterStates2 ) );
	io::OutputMessage( "Filtered down to %u words\n", uNumFilteredWords );

	if( uNumFilteredWords <= 10 )
	{
		io::OutputMessage( "Filtered words are:\n" );
		filteredWords->OutputWords();
	}

	delete filteredWords;

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
