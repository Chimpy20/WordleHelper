#pragma once

#include "Word.h"

namespace wa
{

enum class FilterLetterState : unsigned char
{
	Incorrect = 0,
	WrongPosition,
	Correct,

	NumEntries
};

class FilterWord : public Word
{
public:
	FilterWord();

private:
	FilterLetterState	m_filterLetterStates[ WordLength ] = { FilterLetterState::Incorrect };

};

}
