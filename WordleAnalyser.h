#pragma once

#include "WordList.h"

class WordleAnalyser
{
public:
	WordleAnalyser();
	~WordleAnalyser();

	bool Initialise();

private:
	static const WCHAR* const WORD_LIST_FILENAME;

	WordList m_wordList;
};
