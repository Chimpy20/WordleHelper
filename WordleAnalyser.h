#pragma once

#include "WordList.h"

namespace wa
{

class WordleAnalyser
{
public:
	static const UINT	NumLetters = 'a' - 'A';
	WordleAnalyser();
	~WordleAnalyser();

	UINT				Initialise();
	void				Shutdown();
	void				GenerateStats();

private:
	static const WCHAR* const WORD_LIST_FILENAME;

	WordList*			m_wordList;
	UINT				m_overallLetterCountsPerPosition[ NumLetters ][ Word::WordLength ];
};

}
