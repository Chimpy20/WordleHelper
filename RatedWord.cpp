#include "pch.h"
#include "RatedWord.h"
#include "Analysis.h"

namespace wa
{

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
			rating += 0.50f;
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
				rating += 0.10f;
		}

		// If this letter already occurs in the test word, it's less useful, so lower the score
		if( found )
		{
			for( UINT repeatLetterCheckIndex = 0; repeatLetterCheckIndex < letterIndex; ++repeatLetterCheckIndex )
			{
				if( testWord.GetLetterAtPosition( repeatLetterCheckIndex ) == testLetter )
				{
					rating -= 0.10f;
				}
			}
		}
		else
		{
			// Not being found is also useful as it eliminates letters - award a score for eliminated common letters
			analysis.GetRankForLetterAtPosition( testLetter, letterIndex );
		}
	}

	return rating;
}

}