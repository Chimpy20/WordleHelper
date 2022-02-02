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

	Word						Guess( const WordList& masterWordList );

	containers::List<RatedWord> GetRatedWordList() const
	{
		return m_ratedWordList;
	}

private:
	const WordList&				m_wordList;
	containers::List<RatedWord>	m_ratedWordList;
};

} // namespace wa
