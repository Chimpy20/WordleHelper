#pragma once

#include "List.h"
#include "FilterWord.h"
#include "Analysis.h"
#include "Guesser.h"

namespace wa
{

class Analysis;
class Guesser;

// An object that maintains a list of words and associated functionality
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

	UINT					GetNumWords() const
	{
		return static_cast<UINT>( m_wordList.size() );
	}

	const Analysis&			GetAnalysis() const
	{
		return m_analysis;
	}

	const Guesser&			GetGuesser() const
	{
		return m_guesser;
	}

	// Reads words from a file on disk
	// @param append: set true to add words to the existing list, false to clear the list first
	// @return the number of words read from disk
	UINT					ReadWords( const WCHAR* wordListFileName, bool append );

	// Mix up the words in the list.
	// This is useful because the words appear in the order they are used in Wordle
	void					Randomise();

	// Copy words from another WordList object
	UINT					DuplicateFrom( const WordList& other );

	// Use a filter (guess) to reduce the number of possible solution words down
	// @return the number of reduced words remaining
	UINT					Filter( const FilterWord& filterWord );

	// Use the guesser object to produce a list of weighted words which may be solutions
	// @param masterWordList: the complete list of words - not the filtered down ones
	void					Guess( const WordList& masterWordList );

	// Output the words in the list
	void					OutputWords() const;

private:
	static const UINT		MaxWordBufferSize = 16;

	// Internal function to create the Word objects from a words in memory
	// @return number of words extracted
	UINT					ExtractWords( const CHAR* wordListRaw, UINT wordListRawSize );

	// @return is the provided letter in the range 'a' to 'z', or 'A' to 'Z'?
	bool					IsLetterAlpha( const CHAR letter );

	WordListContainer		m_wordList; // The container of the words
	Analysis				m_analysis; // Analysis object for these words
	Guesser					m_guesser; // Performs a guess for possible solutions
};

} // namespace wa