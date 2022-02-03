#pragma once

#include "WordList.h"

namespace wa
{

class WordleAnalyser
{
public:
	WordleAnalyser();
	~WordleAnalyser();

	// Setup the analyser
	UINT				Initialise();

	
	void				Run();

	// Close down the analyser
	void				Shutdown();

private:
	static const WCHAR* const WORD_LIST_FILENAME;

	// The list of all possible solutions
	WordList*			m_wordList;
};

} // namespace wa
