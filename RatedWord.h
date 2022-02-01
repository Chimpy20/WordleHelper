#pragma once

#include "Word.h"

namespace wa
{

class RatedWord : public Word
{
public:
	RatedWord();
	RatedWord( CHAR letters[ WordLength ] );

private:
	float					m_rating;
};

} // namespace wa
