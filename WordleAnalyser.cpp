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
	FilterLetterState::Incorrect,
	FilterLetterState::Incorrect,
	FilterLetterState::WrongPosition,
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
	FilterLetterState::WrongPosition,
	FilterLetterState::Incorrect,
	FilterLetterState::Incorrect,
	FilterLetterState::WrongPosition,
	FilterLetterState::WrongPosition
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

	return wordsRead;
}

void WordleAnalyser::Run()
{
	WordList* const filteredWords = new WordList();
	filteredWords->DuplicateFrom(*m_wordList);

	filteredWords->Analyse();

	UINT uNumFilteredWords = filteredWords->Filter(FilterWord(testFilterLetters1, testFilterLetterStates1));
	io::OutputMessage("Filtered down to %u words\n", uNumFilteredWords);

	if (uNumFilteredWords <= 10)
	{
		io::OutputMessage("Filtered words are:\n");
		filteredWords->OutputWords();
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
