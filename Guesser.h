#pragma once

#include "Containers.h"
#include "RatedWord.h"

namespace wa
{

class WordList;

class Guesser
{
public:
	typedef containers::List<Word> WordListContainer;

	Guesser( const WordList& wordList );

	void						Guess( const WordList& masterWordList, const Analysis& analysis );

	const containers::List<RatedWord>& GetRatedWordList() const
	{
		return m_ratedWordList;
	}

private:
	const WordList&				m_wordList;
	containers::List<RatedWord>	m_ratedWordList;
};

} // namespace wa
