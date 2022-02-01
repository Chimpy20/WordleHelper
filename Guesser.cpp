#include "pch.h"
#include "Guesser.h"
#include "WordList.h"

namespace wa
{

Guesser::Guesser( const WordList& wordList ):
	m_wordList( wordList )
{
}

Word Guesser::Guess()
{
	m_ratedWordList.clear();

	Word bestGuess;

	const containers::List<Word>& wordList = m_wordList.GetWordList();
	containers::List<Word>::const_iterator itor = wordList.begin();
	while( itor != wordList.end() )
	{
		const Word& word = *itor;

		// If this was the correct word, see how many of the other words would provide correct letters

		RatedWord ratedWord;
		m_ratedWordList.push_back( ratedWord );
		++itor;
	}

	return bestGuess;
}

}