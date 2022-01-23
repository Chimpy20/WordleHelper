#include "pch.h"
#include "Word.h"

namespace wa
{

Word::Word()
{
	m_letters[ 0 ] = '\0';
}

Word::Word( CHAR* letters )
{
	for( UINT letterIndex = 0; letterIndex < WordLength; ++letterIndex )
	{
		CHAR letter = letters[ letterIndex ];
		letter = MakeLower( letter );
		m_letters[ letterIndex ] = letter;
	}

	m_letters[ WordLength ] = '\0';
}

Word::~Word()
{
}

CHAR Word::MakeLower( CHAR letter )
{
	if( letter >= 'A' && letter <= 'Z' )
	{
		letter += ( 'a' - 'A' );
	}

	return letter;
}

}
