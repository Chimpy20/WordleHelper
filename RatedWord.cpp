#include "pch.h"
#include "RatedWord.h"
#include "Analysis.h"

namespace wa
{

const float		RatedWord::CorrectLetterScore = 0.5f;
const float		RatedWord::WrongPositionScore = 0.25f;
const float		RatedWord::MultipleLetterPenalty = 0.1f;

RatedWord::RatedWord():
	m_rating( 1.0f )
{
}

RatedWord::RatedWord( const CHAR* letters ) :
	Word( letters ),
	m_rating( 1.0f )
{
}

void RatedWord::SetRating( const float rating )
{
	m_rating = rating;
}

float RatedWord::RateAgainst( const Word& testWord, const Analysis& analysis )
{
	float rating = 0.0f;
	for( UINT letterIndex = 0; letterIndex < WordLength; ++letterIndex )
	{
		bool found = false;
		const CHAR testLetter = testWord.GetLetterAtPosition( letterIndex );
		const CHAR currentLetter = m_letters[ letterIndex ];
		if( testLetter == currentLetter )
		{
			// Correct letter
			rating += CorrectLetterScore;
			found = true;
		}
		else
		{
			// Not the correct letter - used in another part of the word?
			for( UINT checkLetter = 0; checkLetter < WordLength; ++checkLetter )
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

		if( found )
		{
			// If this letter already occurs in the test word, it's less useful, so lower the score
			for( UINT repeatLetterCheckIndex = 0; repeatLetterCheckIndex < letterIndex; ++repeatLetterCheckIndex )
			{
				if( testWord.GetLetterAtPosition( repeatLetterCheckIndex ) == testLetter )
				{
					rating -= MultipleLetterPenalty;
				}
			}
		}
		else
		{
			// Not being found is also useful as it eliminates letters - award a score for eliminated common letters
			rating = WrongPositionScore * analysis.GetWeightForLetterAtPosition( testLetter, letterIndex );
		}
	}

	return rating;
}

}