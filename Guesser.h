#pragma once

#include "Containers\List.h"
#include "RatedWord.h"

namespace wh
{

class WordList;

// @brief Generates guesses for a possible solution based on a list of words
// @detail The guesser examines each possible word in the overall "master" list
// and assigns each one a "rating" based on how many correct letters are in the
// word being compared. The resultant m_ratedWordList should be the same size
// as the master word list.
class Guesser
{
public:
	typedef containers::List<Word> WordListContainer;

	Guesser( const WordList& wordList );

	// @param masterWordList: The list of all N-letter words
	// @param analysis: The analysis object used to help weight some letters
	void						Guess( const WordList& masterWordList, const Analysis& analysis );
	float						RateWord( const Word& guessWord, const Word& testWord, const Analysis& analysis ) const;

	const containers::List<RatedWord>& GetRatedWordList() const
	{
		return m_ratedWordList;
	}

	void						Reset();

private:
	static const float			CorrectLetterScore;
	static const float			WrongPositionScore;
	static const float			MultipleLetterPenalty;
	static const float			IncorrectLetterBonus;

	const WordList&				m_wordList; // The (possibly filtered down) list of words, one of which is the solution
	containers::List<RatedWord>	m_ratedWordList; // Filled in with rated words, sorted by suitability
};

} // namespace wh
