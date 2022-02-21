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

	// The number of results at which only words that could be
	// solutions are used
	static const UINT	MatchThreshold = 64;

	// The list of all known N-letter words
	WordList*			m_masterWordList;

	// The reduced list of words that could be solutions
	WordList*			m_filteredWords;

	// Performs a guess for possible solutions
	Guesser				m_guesser;

	// The message log
	MessageLog*			m_messageLog;
};

} // namespace wh
