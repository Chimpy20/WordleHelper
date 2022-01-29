#pragma once

#include "Containers.h"
#include "Word.h"

namespace wa
{

class WordList
{
public:
	WordList();
	~WordList();

	UINT					ReadWords( const WCHAR* wordListFileName );

	UINT					DuplicateFrom( const WordList& other );

private:
	static const UINT		MaxWordBufferSize = 16;

	UINT					ExtractWords();
	bool					IsLetterAlpha( const CHAR letter );

	CHAR*					m_wordListRaw;
	UINT					m_wordListRawSize;

	containers::List<Word>	m_wordList;
};

}