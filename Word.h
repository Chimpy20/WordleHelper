#pragma once

namespace wa
{

class Word
{
public:
	static const unsigned int WordLength = 5;
	Word();
	Word( const CHAR* letters );
	~Word();

	bool operator == ( const Word& other ) const
	{
		for( UINT letterIndex = 0; letterIndex < WordLength; ++letterIndex )
		{
			if( m_letters[ letterIndex ] != other.m_letters[ letterIndex ] )
				return false;
		}

		return true;
	}

	bool operator != ( const Word& other ) const
	{
		for( UINT letterIndex = 0; letterIndex < WordLength; ++letterIndex )
		{
			if( m_letters[ letterIndex ] != other.m_letters[ letterIndex ] )
				return true;
		}

		return false;
	}

	CHAR			GetLetterAtPosition( const UINT position ) const;
	const CHAR*		GetAsString() const
	{
		return m_letters;
	}

protected:
	static CHAR		MakeLower( CHAR letter );
	CHAR			m_letters[ WordLength + 1 ] = { '\0' };
};

} // namespace wa
