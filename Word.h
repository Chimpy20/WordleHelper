#pragma once

namespace wa
{

class Word
{
public:
	static const unsigned int WordLength = 5;
	Word();
	Word( CHAR* letters );
	~Word();

private:
	static CHAR		MakeLower( CHAR letter );
	CHAR			m_letters[ WordLength + 1 ] = { '\0' };
};

}