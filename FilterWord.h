#pragma once

#include "Word.h"

namespace wa
{

enum FilterLetterState
{
	Incorrect = 0,
	WrongPosition,
	Correct,

	NumEntries
};

// An object that contains N letters, and the configuration of the results of a Wordle guess
// of "Correct", "WrongPosition" or "Incorrect" for each letter
class FilterWord : public Word
{
public:
	//FilterWord();
	FilterWord( const CHAR letters[ WordLength ], const FilterLetterState filteredLetterStates[ WordLength ] );

	FilterLetterState	GetStateAtPos( const UINT letterPosition ) const;

	// Determines if a given word could be match based on this word being a guess,
	// using the letter states to elimate other words
	bool				PotentialMatch( const Word& word ) const;

private:
	FilterLetterState	m_filterLetterStates[ WordLength ] = { FilterLetterState::Incorrect }; // The state of each letter in a guess word i.e. WrongPosition
};

} // namespace wa
