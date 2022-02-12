#include "pch.h"
#include "WordleAnalyser.h"
#include "FilterWord.h"

namespace wa
{

const WCHAR* const WordleAnalyser::WORD_LIST_FILENAME = L"wordlist.txt";

WordleAnalyser::WordleAnalyser():
	m_masterWordList( nullptr ),
	m_filteredWords( nullptr )
{
}

WordleAnalyser::~WordleAnalyser()
{
}

UINT WordleAnalyser::Initialise()
{
	m_masterWordList = new WordList();
	const UINT wordsRead = m_masterWordList->ReadWords( WORD_LIST_FILENAME, false );
	m_masterWordList->Randomise();

	m_filteredWords = new WordList();

	Reset();

	return wordsRead;
}

UINT WordleAnalyser::FilterAndGuess( const FilterWord& filterWord )
{
	UINT numFilteredWords = 0;
	numFilteredWords = m_filteredWords->Filter( filterWord );
	io::OutputMessage( "Filtered down to %u words\n", numFilteredWords );

	const WordList* const wordListToUseForGuess = ( m_filteredWords->GetNumWords() > 24 ) ? m_masterWordList : m_filteredWords;
	if( wordListToUseForGuess->GetNumWords() > 0 )
	{
		const containers::List<RatedWord>& ratedWordList = m_filteredWords->Guess( *wordListToUseForGuess );

		io::OutputMessage( "Best guesses are:\n" );
		containers::List<RatedWord>::const_iterator itor = ratedWordList.begin();
		UINT wordsToDisplay = 0;
		while( itor != ratedWordList.end() && ( wordsToDisplay < 3 ) )
		{
			const RatedWord& word = *itor;
			io::OutputMessage( "\t%s (%.5f)\n", word.GetAsString(), word.GetRating() );
			++itor;
			++wordsToDisplay;
		}
	}

	return numFilteredWords;
}

void WordleAnalyser::Reset()
{
	ASSERT( m_masterWordList != nullptr, "Word list doesn't exist\n" );

	// Create a word list that is a copy of the list of possible solutions
	ASSERT( m_filteredWords != nullptr, "Filtered word list doesn't exist\n" );
	m_filteredWords->DuplicateFrom( *m_masterWordList );
}

void WordleAnalyser::Shutdown()
{
	if( m_filteredWords != nullptr )
	{
		delete m_filteredWords;
		m_filteredWords = nullptr;
	}

	if( m_masterWordList != nullptr )
	{
		delete m_masterWordList;
		m_masterWordList = nullptr;
	}
}

}
