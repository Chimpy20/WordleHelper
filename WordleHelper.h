#pragma once

#include "WordList.h"
#include "MessageLog.h"

namespace wh
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
	MessageLog*			GetMessageLog() const
	{
		return m_messageLog;
	}

	// Close down the analyser
	void				Shutdown();

private:
	static const UINT	MessageMaxLength = 128;

	// The list of all possible solutions
	WordList*			m_masterWordList;
	WordList*			m_filteredWords;
	MessageLog*			m_messageLog;
};

} // namespace wh
