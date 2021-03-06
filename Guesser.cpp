#include "pch.h"
#include "Guesser.h"
#include "WordList.h"
#include "FilterWord.h"

namespace wh
{

const float		Guesser::CorrectLetterScore = 0.50f;
const float		Guesser::WrongPositionScore = 0.25f;
const float		Guesser::MultipleLetterPenalty = 1.0f;
const float		Guesser::IncorrectLetterBonus = 0.28f;
const float		Guesser::IncorrectLetterBonusMultiplier = 4.0f;

Guesser::Guesser()
{
}

// Produces a list of words sorted by a "rating" or "score" based on how common the letters in the word are
// and how likely they are to elimiate other words
// @param filteredWordList: The list of words remaining that could be the solution
// @param masterWordList: The list of all N-letter words
// @param proportionTotalWordsRemaining: How many words are remaining after the unviable ones have been eliminated
const containers::List<RatedWord>& Guesser::Guess( const WordList& filteredWordList, const WordList& masterWordList, const float proportionTotalWordsRemaining )
{
	utils::StartTimer();

	const Analysis& analysis = filteredWordList.GetAnalysis();

	// Clear the resultant list is case this is not the first time of running
	m_ratedWordList.clear();

	const WordListContainer& wordListContainer = filteredWordList.GetWordList();
	const WordListContainer& masterWordListContainer = masterWordList.GetWordList();

	ASSERT( wordListContainer.size() > 0, "There are no words in the word list the guesser is using\n" );
	ASSERT( masterWordListContainer.size() >= 1, "Too few words in master list\n" );
	if( masterWordListContainer.size() == 0 )
		return m_ratedWordList;

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
				rating += RateWord( word, checkWord, analysis, proportionTotalWordsRemaining );
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

	// Sort the list of words
	m_ratedWordList.sort();

	utils::EndTimer( "Guess" );

	return m_ratedWordList;
}

// For a given word that has been a guess, take a word from the list of word provided to the guesser, and 
// award the word from the list a score based on how close tyhe guess is based on the states of the guessed letters
// @param guessWord the word that is the users guess
// @param testWord the word from the list of words
// @param analysis the analysis object
// @param proportionTotalWordsRemaining what fraction 0.0-1.0 of words are left from the original list of starting words
float Guesser::RateWord( const Word& guessWord, const Word& testWord, const Analysis& analysis, const float proportionTotalWordsRemaining ) const
{
	float rating = 0.0f;
	for( UINT letterIndex = 0; letterIndex < Word::WordLength; ++letterIndex )
	{
		bool found = false;
		const CHAR testLetter = testWord.GetLetterAtPosition( letterIndex );
		const CHAR currentLetter = guessWord.GetLetterAtPosition( letterIndex );
		if( testLetter == currentLetter )
		{
			// Correct letter
			rating += CorrectLetterScore;
			found = true;
		}
		else
		{
			// Not the correct letter - used in another part of the word?
			for( UINT checkLetter = 0; checkLetter < Word::WordLength; ++checkLetter )
			{
				if( checkLetter == letterIndex )
					continue;

				if( testLetter == currentLetter )
				{
					found = true;
					break;
				}
			}

			if( found )
				rating += WrongPositionScore;
		}

		if( !found )
		{
			// Not being found is also useful as it eliminates letters - award a score for eliminated common letters

			// Cap the factor used to multiply the incorrect letter bonus
			// Why do we multiply here? Typically, a player will reduce the number of viable words down to about a 
			// quarter in the first guess. We still want to award incorrect, but commonly used letters, on the 2nd guess.
			// This "boost" adds more weight for second guesses (or futher guesses if the player is really bad!)
			float incorrectLetterBonusMultiplier = proportionTotalWordsRemaining * IncorrectLetterBonusMultiplier;
			if( incorrectLetterBonusMultiplier > 1.0f )
				incorrectLetterBonusMultiplier = 1.0f;

			rating += ( IncorrectLetterBonus * analysis.GetWeightForLetterAtPosition( testLetter, letterIndex ) * incorrectLetterBonusMultiplier);
		}

		// If this letter already occurs in the test word, it's less useful, so lower the score
		for( UINT repeatLetterCheckIndex = 0; repeatLetterCheckIndex < letterIndex; ++repeatLetterCheckIndex )
		{
			if( guessWord.GetLetterAtPosition( repeatLetterCheckIndex ) == currentLetter )
			{
				rating -= MultipleLetterPenalty;
			}
		}
	}

	return rating;
}

// Clear the list of rated words
void Guesser::Reset()
{
	m_ratedWordList.clear();
}

}