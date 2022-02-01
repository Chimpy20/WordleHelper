#pragma once

#include "WordList.h"

namespace wa
{

class WordleAnalyser
{
public:
	WordleAnalyser();
	~WordleAnalyser();

	UINT				Initialise();
	void				Run();
	void				Shutdown();

private:
	static const WCHAR* const WORD_LIST_FILENAME;

	WordList*			m_wordList;
};

} // namespace wa
