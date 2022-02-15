#pragma once

#include "WordList.h"
#include "MessageLog.h"

namespace wa
{

class WordleHelper
{
public:
	WordleHelper();
	~WordleHelper();

	// Setup the analyser
	UINT				Initialise();

	void				Guess();
	UINT				Filter( const FilterWord& filterWord );
	void				Reset();
	void				Run();

	// Close down the analyser
	void				Shutdown();

private:
	static const WCHAR* const WordListFilename;
	static const UINT	MessageMaxLength = 128;

	// The list of all possible solutions
	WordList*			m_masterWordList;
	WordList*			m_filteredWords;
	MessageLog			m_messageLog;
};

} // namespace wa
