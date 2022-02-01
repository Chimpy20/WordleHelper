#pragma once

#include "RatedWord.h"

namespace wa
{

class WordList;

class Guesser
{
public:
	Guesser( const WordList& wordList );

	Word						Guess();

private:
	const WordList&				m_wordList;
	containers::List<RatedWord>	m_ratedWordList;
};

} // namespace wa
