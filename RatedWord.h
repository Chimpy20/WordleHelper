#pragma once

#include "Word.h"

namespace wh
{

class Analysis;

class RatedWord : public Word
{
public:
	RatedWord();
	RatedWord( const CHAR* letters );

	bool operator< ( RatedWord& other )
	{
		return m_rating < other.m_rating;
	}

	bool operator> ( RatedWord& other )
	{
		return m_rating > other.m_rating;
	}

	float					GetRating() const
	{
		return m_rating;
	}
	void					SetRating( const float rating );

private:
	float					m_rating;
};

} // namespace wh
