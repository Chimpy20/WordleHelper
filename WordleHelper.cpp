#include "pch.h"
#include "WordleHelper.h"
#include "FilterWord.h"

namespace wa
{

const WCHAR* const WordleHelper::WordListFilename = L"wordlist.txt";

WordleHelper::WordleHelper():
	m_masterWordList( nullptr ),
	m_filteredWords( nullptr )
{
}

WordleHelper::~WordleHelper()
{
}

UINT WordleHelper::Initialise()
{
	m_masterWordList = new WordList();
	const UINT wordsRead = m_masterWordList->ReadWords( WordListFilename, false );
	m_masterWordList->Randomise();

	m_filteredWords = new WordList();

	Reset();

	return wordsRead;
}

UINT WordleHelper::Filter( const FilterWord& filterWord )
{
	UINT numFilteredWords = 0;
	numFilteredWords = m_filteredWords->Filter( filterWord );

	CHAR filterMessage[ MessageMaxLength ];
	io::FormatString( filterMessage, MessageMaxLength, "Filtered down to %u words\n", numFilteredWords );
	m_messageLog.AddMessage( filterMessage, MessageMaxLength );

	return numFilteredWords;
}

void WordleHelper::Guess()
{
	const WordList* const wordListToUseForGuess = ( m_filteredWords->GetNumWords() > 128 ) ? m_masterWordList : m_filteredWords;
	if( wordListToUseForGuess->GetNumWords() > 0 )
	{
		CHAR infoMessage[ MessageMaxLength ];
		io::FormatString( infoMessage, MessageMaxLength, "There are %u words possible\n", wordListToUseForGuess->GetNumWords() );
		const containers::List<RatedWord>& ratedWordList = m_filteredWords->Guess( *wordListToUseForGuess );

		CHAR guessMessage[ MessageMaxLength ];
		io::FormatString( guessMessage, MessageMaxLength, "Best guesses are:\n\t" );
		containers::List<RatedWord>::const_iterator itor = ratedWordList.begin();
		UINT wordsToDisplay = 0;
		while( itor != ratedWordList.end() && ( wordsToDisplay < 3 ) )
		{
			const RatedWord& word = *itor;
			io::FormatStringAppend( guessMessage, MessageMaxLength, "%s ", word.GetAsString() );
			++itor;
			++wordsToDisplay;
		}
		io::FormatStringAppend( guessMessage, MessageMaxLength, "\n" );
		m_messageLog.AddMessage( guessMessage, MessageMaxLength );
	}
}

void WordleHelper::Reset()
{
	ASSERT( m_masterWordList != nullptr, "Word list doesn't exist\n" );

	// Create a word list that is a copy of the list of possible solutions
	ASSERT( m_filteredWords != nullptr, "Filtered word list doesn't exist\n" );
	const UINT numWords = m_filteredWords->DuplicateFrom( *m_masterWordList );

	io::OutputMessage( "There are %u words\n", numWords );
}

void WordleHelper::Shutdown()
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
