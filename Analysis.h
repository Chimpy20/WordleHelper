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

	CHAR				GetMostCommonLetterAtPosition( const UINT position );

private:
	WordList&			m_wordList;
	UINT				m_overallLetterCountsPerPosition[ NumLetters ][ Word::WordLength ] = {'\0'};
	CHAR				m_letterRankingsPerPosition[ NumLetters ][ Word::WordLength ] = { 0 };
};

}
