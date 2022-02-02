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

	const Guesser&			GetGuesser() const
	{
		return m_guesser;
	}

	UINT					ReadWords( const WCHAR* wordListFileName, bool append );

	void					Randomise();

	UINT					DuplicateFrom( const WordList& other );

	UINT					Filter( const FilterWord& filterWord );

	void					Guess( const WordList& masterWordList );

	void					OutputWords() const;

private:
	static const UINT		MaxWordBufferSize = 16;

	UINT					ExtractWords( const CHAR* wordListRaw, UINT wordListRawSize );
	bool					IsLetterAlpha( const CHAR letter );

	WordListContainer		m_wordList;
	Analysis				m_analysis;
	Guesser					m_guesser;
};

} // namespace wa