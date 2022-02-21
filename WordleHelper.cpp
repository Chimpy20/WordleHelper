#include "pch.h"
#include "WordleHelper.h"
#include "FilterWord.h"
#include "Guesser.h"

namespace wh
{

WordleHelper::WordleHelper() :
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

	// Mix up the overall list of words because wordle uses consequative words each day from the list
	// which may indicate which word is the solution
	m_masterWordList->Randomise();

	m_filteredWords = new WordList();

	m_messageLog = new MessageLog();

	Reset();

	return wordsRead;
}

// Reduce the current list of possible words down based on the current guess word
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

// Provide some next guesses based on the current possible solutions
void WordleHelper::Guess()
{
	const WordList* const wordListToUseForGuess = m_filteredWords;
	ASSERT( wordListToUseForGuess != nullptr, "WordleHelper::Guess(): Couldn't get a list of words\n" );
	if( wordListToUseForGuess->GetNumWords() > 0 )
	{
		CHAR infoMessage[ MessageMaxLength ];
		io::FormatString( infoMessage, MessageMaxLength, "There are %u words possible\r\n", wordListToUseForGuess->GetNumWords() );

		Guesser guesser;

		// Do the guess
		m_filteredWords->Analyse();
		const float proportionWordsRemaining = static_cast<float>( m_filteredWords->GetNumWords() ) / static_cast<float>( m_masterWordList->GetNumWords() );

		const containers::List<RatedWord>& ratedWordList = guesser.Guess( *m_filteredWords, *wordListToUseForGuess, proportionWordsRemaining );

		// Output the results
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

// Reset things back ready for a new try
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
