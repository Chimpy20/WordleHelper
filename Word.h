#pragma once

namespace wh
{

// An object describing an N letter word
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
	// Get the letter at a given position in the word
	inline CHAR			GetLetterAtPosition( const UINT position ) const
	{
		ASSERT( position < WordLength, "Letter position out of range\n" );
		return m_letters[ position ];
	}

	// Access the word as a c-style string
	const CHAR*			GetAsString() const
	{
		return m_letters;
	}

#ifdef _DEBUG
	bool				Validate() const;
#endif

protected:
	// Internal function to make the letters all lower case
	static CHAR			MakeLower( CHAR letter );

	CHAR				m_letters[ WordLength + 1 ] = { '\0' }; // The letters in the word as a c-style array
};

} // namespace wh
