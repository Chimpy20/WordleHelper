#include "pch.h"
#include "WordleHelper.h"
#include "FilterWord.h"

namespace wh
{

WordleHelper::WordleHelper():
	m_masterWordList( nullptr ),
	m_filteredWords( nullptr ),
	m_messageLog( nullptr )
{
}

WordleHelper::~WordleHelper()
{
}

UINT WordleHelper::Initialise()
{
	m_masterWordList = new WordList();
	const UINT wordsRead = m_masterWordList->ReadWords( IDR_PRIMARYWORDLIST, false );
	m_masterWordList->Randomise();

	m_filteredWords = new WordList();

	m_messageLog = new MessageLog();

	Reset();

	return wordsRead;
}


UINT WordleHelper::Filter( const FilterWord& filterWord )
{
	CHAR filterMessage[ MessageMaxLength ];

	io::FormatString( filterMessage, MessageMaxLength, "Trying \"%s\"\r\n", filterWord.GetAsString() );
	m_messageLog->AddMessage( filterMessage, MessageMaxLength );

	UINT numFilteredWords = 0;
	numFilteredWords = m_filteredWords->Filter( filterWord );

	if( numFilteredWords > 1 )
	{
		io::FormatString( filterMessage, MessageMaxLength, "Filtered down to %u words\r\n", numFilteredWords );
	}
	else if( numFilteredWords == 1 )
	{
		io::FormatString( filterMessage, MessageMaxLength, "There's only one word remaining - you've probably won.\r\n" );
	}
	else
	{
		io::FormatString( filterMessage, MessageMaxLength, "This leaves zero words - did you make a mistake in entering the correct letters?\r\n" );
	}
	m_messageLog->AddMessage( filterMessage, MessageMaxLength );

	return numFilteredWords;
}

void WordleHelper::Guess()
{
	const WordList* const wordListToUseForGuess = ( m_filteredWords->GetNumWords() > 128 ) ? m_masterWordList : m_filteredWords;
	if( wordListToUseForGuess->GetNumWords() > 0 )
	{
		CHAR infoMessage[ MessageMaxLength ];
		io::FormatString( infoMessage, MessageMaxLength, "There are %u words possible\r\n", wordListToUseForGuess->GetNumWords() );
		const containers::List<RatedWord>& ratedWordList = m_filteredWords->Guess( *wordListToUseForGuess );

		CHAR guessMessage[ MessageMaxLength ];
		io::FormatString( guessMessage, MessageMaxLength, "Best guesses are:\r\n" );
		containers::List<RatedWord>::const_iterator itor = ratedWordList.begin();
		UINT wordsToDisplay = 0;
		while( itor != ratedWordList.end() && ( wordsToDisplay < 5 ) )
		{
			const RatedWord& word = *itor;
			io::FormatStringAppend( guessMessage, MessageMaxLength, "%s ", word.GetAsString() );
			++itor;
			++wordsToDisplay;
		}
		io::FormatStringAppend( guessMessage, MessageMaxLength, "\r\n" );
		m_messageLog->AddMessage( guessMessage, MessageMaxLength );
	}
}

void WordleHelper::Reset()
{
	m_messageLog->Reset();

	ASSERT( m_masterWordList != nullptr, "Word list doesn't exist\n" );

	// Create a word list that is a copy of the list of possible solutions
	ASSERT( m_filteredWords != nullptr, "Filtered word list doesn't exist\n" );
	const UINT numWords = m_filteredWords->DuplicateFrom( *m_masterWordList );

	CHAR infoMessage[ MessageMaxLength ];
	io::FormatString( infoMessage, MessageMaxLength, "%u known words\r\n", numWords );
	m_messageLog->AddMessage( infoMessage, MessageMaxLength );
}

void WordleHelper::Shutdown()
{
	if( m_messageLog != nullptr )
	{
		delete m_messageLog;
		m_messageLog = nullptr;
	}

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
