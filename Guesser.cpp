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
	utils::StartTimer();


	 // Clear the resultant list is case this is not the first time of running
	m_ratedWordList.clear();

	const WordListContainer& wordListContainer = m_wordList.GetWordList();
	const WordListContainer& masterWordListContainer = masterWordList.GetWordList();

	ASSERT( wordListContainer.size() > 0, "There are no words in the word list the guesser is using\n" );
	ASSERT( masterWordListContainer.size() >= 1, "Too few words in master list\n" );
	const float masterWordsDivisor = static_cast<float>( masterWordListContainer.size() - 1 );

	// Examine each word which could be used to most effictively reduce down the number of possibilities
	// even if it might not be a solution
	containers::List<Word>::const_iterator masterListItor = masterWordListContainer.begin();
	while( masterListItor != masterWordListContainer.end() )
	{
		const Word& word = *masterListItor;

		// Create a rated word from the word from the master list
		RatedWord ratedWord( word.GetAsString() );
		float rating = 0.0f;

		// Examine each possible solution, and determine how well the rated word would fare in
		// identifying it
		WordListContainer::const_iterator listItor = wordListContainer.begin();
		while( listItor != wordListContainer.end() )
		{
			const Word& checkWord = *listItor;
			if( word != checkWord )
			{
				rating += ratedWord.RateAgainst( checkWord, analysis );
			}
			++listItor;
		}

		// Normalise the rating
		rating /= masterWordsDivisor;
		ratedWord.SetRating( rating );

		// Store the rated word
		m_ratedWordList.push_back( ratedWord );
		++masterListItor;
	}

	m_ratedWordList.sort();

	utils::EndTimer( "Guess");
}

}