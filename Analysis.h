#pragma once

#include "Word.h"

namespace wa
{

class WordList;

class Analysis
{
public:
	static const UINT	NumLetters = 'z' - 'a' + 1;
	static const UINT	FirstLetterOffset = 'a';

	explicit Analysis( WordList& wordList );

	void				Analyse();

private:
	WordList&			m_wordList;
	UINT				m_overallLetterCountsPerPosition[ NumLetters ][ Word::WordLength ] = {'\0'};
};

}
