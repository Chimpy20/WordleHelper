#include "pch.h"
#include "Guesser.h"
#include "WordList.h"
#include "FilterWord.h"

namespace wh
{

Guesser::Guesser( const WordList& wordList ) :
	m_wordList( wordList )
{
}

// Produces a list of words sorted by a "rating" or "score" based on how common the letters in the word are
// and how likely they are to elimiate other words
// @param masterWordList: The list of all N-letter words, one of which will be the solution
// @param analysis: The analysis object used to help weight some letters
void Guesser::Guess( const WordList& masterWordList, const Analysis& analysis )
{
	utils::StartTimer();

	// Clear the resultant list is case this is not the first time of running
	m_ratedWordList.clear();

	const WordListContainer& wordListContainer = m_wordList.GetWordList();
	const WordListContainer& masterWordListContainer = masterWordList.GetWordList();

	ASSERT( wordListContainer.size() > 0, "There are no words in the word list the guesser is using\n" );
	ASSERT( masterWordListContainer.size() >= 1, "Too few words in master list\n" );
	if( masterWordListContainer.size() == 0 )
		return;

	const float masterWordsDivisor = static_cast<float>( masterWordListContainer.size() );

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
				rating += analysis.RateWord( word, checkWord );
			}
			++listItor;
		}

		/*if( rating < 0.0f )
			rating = 0.0f;*/

		// Normalise the rating
		rating /= masterWordsDivisor;
		ratedWord.SetRating( rating );

		// Store the rated word
		m_ratedWordList.push_back( ratedWord );
		++masterListItor;
	}

	// Sort the list of words
	m_ratedWordList.sort();

	utils::EndTimer( "Guess" );
}

void Guesser::Reset()
{
	m_ratedWordList.clear();
}

}