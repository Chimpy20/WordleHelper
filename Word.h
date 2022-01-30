#pragma once

namespace wa
{

class Word
{
public:
	static const unsigned int WordLength = 5;
	Word();
	Word( CHAR letters[ WordLength ] );
	~Word();

	CHAR			GetLetterAtPosition( const UINT position ) const;
	const CHAR*		GetAsString() const
	{
		return m_letters;
	}

protected:
	static CHAR		MakeLower( CHAR letter );
	CHAR			m_letters[ WordLength + 1 ] = { '\0' };
};

}
