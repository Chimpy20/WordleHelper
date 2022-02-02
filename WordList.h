#pragma once

#include "Containers.h"
#include "FilterWord.h"
#include "Analysis.h"
#include "Guesser.h"

namespace wa
{

class Analysis;
class Guesser;


class WordList
{
public:
	typedef containers::List<Word> WordListContainer;
	WordList();
	~WordList();

	const WordListContainer& GetWordList() const
	{
		return m_wordList;
	}

	const Analysis&			GetAnalysis() const
	{
		return m_analysis;
	}

	UINT					ReadWords( const WCHAR* wordListFileName );

	void					Randomise();

	UINT					DuplicateFrom( const WordList& other );

	UINT					Filter( const FilterWord& filterWord );

	Word					Guess( const WordList& masterWordList );

	void					OutputWords() const;

private:
	static const UINT		MaxWordBufferSize = 16;

	UINT					ExtractWords();
	bool					IsLetterAlpha( const CHAR letter );

	CHAR*					m_wordListRaw;
	UINT					m_wordListRawSize;

	WordListContainer		m_wordList;
	Analysis				m_analysis;
	Guesser					m_guesser;
};

} // namespace wa