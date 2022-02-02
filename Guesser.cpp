#include "pch.h"
#include "Guesser.h"
#include "WordList.h"
#include "FilterWord.h"

namespace wa
{

Guesser::Guesser( const WordList& wordList ):
	m_wordList( wordList )
{
}

Word Guesser::Guess( const WordList& masterWordList )
{
	LARGE_INTEGER frequency, startTime, endTime;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &startTime );

	m_ratedWordList.clear();

	Word bestGuess;

	const WordListContainer& wordList = m_wordList.GetWordList();
	const WordListContainer& masterWordListContainer = masterWordList.GetWordList();

	containers::List<Word>::const_iterator itor = wordList.begin();
	while( itor != wordList.end() )
	{
		const Word& word = *itor;
		RatedWord ratedWord( word.GetAsString() );
		float rating = 0.0f;

		// If this was the correct word, see how many of the other words would provide correct letters
		containers::List<Word>::const_iterator masterListItor = masterWordListContainer.begin();
		while( masterListItor != masterWordListContainer.end() )
		{
			const Word& masterWord = *masterListItor;
			rating += ratedWord.RateAgainst( masterWord );

			++masterListItor;
		}

		rating /= static_cast<float>( masterWordListContainer.size() );
		ratedWord.SetRating( rating );

		m_ratedWordList.push_back( ratedWord );
		++itor;
	}

	m_ratedWordList.sort();

	bestGuess = *m_ratedWordList.begin();

	QueryPerformanceCounter( &endTime );
	const float guessDuration = static_cast<float>( endTime.QuadPart - startTime.QuadPart ) * 1000.0f / static_cast<float>( frequency.QuadPart );
	io::OutputMessage( "Guess took %.4fms\n", guessDuration );

	return bestGuess;
}

}