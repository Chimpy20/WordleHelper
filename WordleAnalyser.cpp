#include "pch.h"
#include "WordleAnalyser.h"
#include "FilterWord.h"

namespace wa
{

const WCHAR* const WordleAnalyser::WORD_LIST_FILENAME = L"wordlist.txt";

const CHAR testFilterLetters1[ Word::WordLength ] =
{
	's',
	'u',
	'n',
	'e',
	't'
};

const FilterLetterState testFilterLetterStates1[ Word::WordLength ] = 
{
	FilterLetterState::WrongPosition,
	FilterLetterState::Incorrect,
	FilterLetterState::Incorrect,
	FilterLetterState::Incorrect,
	FilterLetterState::Correct
};

const CHAR testFilterLetters2[ Word::WordLength ] =
{
	'c',
	'o',
	'a',
	's',
	't'
};

const FilterLetterState testFilterLetterStates2[ Word::WordLength ] =
{
	FilterLetterState::Incorrect,
	FilterLetterState::Correct,
	FilterLetterState::Incorrect,
	FilterLetterState::Correct,
	FilterLetterState::Correct
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
	const UINT wordsRead = m_wordList->ReadWords( WORD_LIST_FILENAME, false );
	m_wordList->Randomise();

	return wordsRead;
}

void WordleAnalyser::Run()
{
	ASSERT( m_wordList != nullptr, "Word list doesn't exist\n" );

	// Create a word list that is a copy of the list of possible solutions
	WordList* const filteredWords = new WordList();
	filteredWords->DuplicateFrom( *m_wordList );

	filteredWords->Guess( *m_wordList );

	const Guesser& guesser = filteredWords->GetGuesser();

	{
		const containers::List<RatedWord>& ratedWordList = guesser.GetRatedWordList();
		containers::List<RatedWord>::const_iterator itor = ratedWordList.begin();
		UINT wordsToDisplay = 0;
		while( itor != ratedWordList.end() && ( wordsToDisplay < 5 ) )
		{
			const RatedWord& word = *itor;
			io::OutputMessage( "\t%s (%.3f)\n", word.GetAsString(), word.GetRating() );
			++itor;
			++wordsToDisplay;
		}
	}

	UINT uNumFilteredWords = filteredWords->Filter(FilterWord(testFilterLetters1, testFilterLetterStates1));
	io::OutputMessage("Filtered down to %u words\n", uNumFilteredWords);

	if (uNumFilteredWords <= 10)
	{
		io::OutputMessage("Filtered words are:\n");
		filteredWords->OutputWords();
	}

	filteredWords->Guess( *m_wordList );

	{
		const containers::List<RatedWord>& ratedWordList = guesser.GetRatedWordList();
		containers::List<RatedWord>::const_iterator itor = ratedWordList.begin();
		UINT wordsToDisplay = 0;
		while( itor != ratedWordList.end() && ( wordsToDisplay < 5 ) )
		{
			const RatedWord& word = *itor;
			io::OutputMessage( "\t%s (%.3f)\n", word.GetAsString(), word.GetRating() );
			++itor;
			++wordsToDisplay;
		}
	}

	uNumFilteredWords = filteredWords->Filter(FilterWord(testFilterLetters2, testFilterLetterStates2));
	io::OutputMessage("Filtered down to %u words\n", uNumFilteredWords);

	if (uNumFilteredWords <= 10)
	{
		io::OutputMessage("Filtered words are:\n");
		filteredWords->OutputWords();
	}

	delete filteredWords;
}

void WordleAnalyser::Shutdown()
{
	delete m_wordList;
}

}
