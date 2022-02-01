#pragma once

#include "Containers.h"
#include "FilterWord.h"
#include "Analysis.h"

namespace wa
{

class Analysis;

class WordList
{
public:
	WordList();
	~WordList();

	UINT					ReadWords( const WCHAR* wordListFileName );

	void					Randomise();

	const containers::List<Word>& GetWordList() const
	{
		return m_wordList;
	}

	UINT					DuplicateFrom( const WordList& other );

	UINT					Filter( const FilterWord& filterWord );

	void					Analyse();

	void					OutputWords();

private:
	static const UINT		MaxWordBufferSize = 16;

	UINT					ExtractWords();
	bool					IsLetterAlpha( const CHAR letter );

	CHAR*					m_wordListRaw;
	UINT					m_wordListRawSize;

	containers::List<Word>	m_wordList;
	Analysis				m_analysis;
};

}