#include "pch.h"
#include "RatedWord.h"

namespace wa
{

RatedWord::RatedWord():
	m_rating( 0.0f )
{
}

RatedWord::RatedWord( CHAR letters[ WordLength ] ) :
	Word( letters ),
	m_rating( 0.0f )
{
}

}