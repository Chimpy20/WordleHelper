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

void Guesser::Guess( const WordList& masterWordList, const Analysis& analysis )
{
	LARGE_INTEGER frequency, startTime, endTime;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &startTime );

	m_ratedWordList.clear();

	const WordListContainer& wordList = m_wordList.GetWordList();
	const WordListContainer& masterWordListContainer = masterWordList.GetWordList();

	const float masterWordsDivisor = static_cast<float>( masterWordListContainer.size() - 1 );

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
			if( word != masterWord )
			{
				rating += ratedWord.RateAgainst( masterWord, analysis );
			}
			++masterListItor;
		}

		rating /= masterWordsDivisor;
		ratedWord.SetRating( rating );

		m_ratedWordList.push_back( ratedWord );
		++itor;
	}

	m_ratedWordList.sort();

	QueryPerformanceCounter( &endTime );
	const float guessDuration = static_cast<float>( endTime.QuadPart - startTime.QuadPart ) * 1000.0f / static_cast<float>( frequency.QuadPart );
	io::OutputMessage( "Guess took %.4fms\n", guessDuration );
}

}