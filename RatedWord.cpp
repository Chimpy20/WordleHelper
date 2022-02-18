#include "pch.h"
#include "RatedWord.h"
#include "Analysis.h"

namespace wh
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

}