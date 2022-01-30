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
	FilterWord( const CHAR letters[ WordLength ], const FilterLetterState filteredLetterStates[ WordLength  ] );

	FilterLetterState	GetStateAtPos( const UINT letterPosition ) const;
	bool				PotentialMatch( const Word& word ) const;

private:
	FilterLetterState	m_filterLetterStates[ WordLength ] = { FilterLetterState::Incorrect };
};

}
