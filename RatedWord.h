#pragma once

#include "Word.h"

namespace wa
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

	float					RateAgainst( const Word& testWord, const Analysis& analysis );

private:
	static const float		CorrectLetterScore;
	static const float		WrongPositionScore;
	static const float		MultipleLetterPenalty;
	static const float		IncorrectLetterBonus;

	float					m_rating;
};

} // namespace wa
