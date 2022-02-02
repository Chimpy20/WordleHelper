#include "pch.h"
#include "RatedWord.h"

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

float RatedWord::RateAgainst( const Word& testWord )
{
	float rating = 0.0f;
	for( UINT letterIndex = 0; letterIndex < WordLength; ++letterIndex )
	{
		if( testWord.GetLetterAtPosition( letterIndex ) == m_letters[ letterIndex ] )
		{
			// Correct letter
			rating += 0.2f;
		}
		else
		{
			// Not the correct letter - used in another part of the word?
			bool found = false;
			for( UINT checkLetter = 0; checkLetter < WordLength; ++checkLetter )
			{
				if( checkLetter == letterIndex )
					continue;

				if( testWord.GetLetterAtPosition( checkLetter ) == m_letters[ letterIndex ] )
				{
					found = true;
					break;
				}
			}
			
			if( found )
				rating += 0.1f;
		}
	}

	return rating;
}

}