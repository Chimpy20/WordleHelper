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
		m_letters[ letterIndex ] = letters[ letterIndex ];
	}

	m_letters[ WordLength ] = '\0';
}

Word::~Word()
{
}

}
