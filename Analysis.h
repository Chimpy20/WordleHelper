#pragma once

#include "Word.h"

namespace wh
{

class WordList;

// Object that examines each word and does a statistical analysis to do things
// like identify the most common letters in each letter space
class Analysis
{
public:
	static const UINT		NumLetters = 'z' - 'a' + 1;
	static const UINT		FirstLetterOffset = 'a';

	Analysis( WordList&	wordList );

	void					Analyse();

	CHAR					GetLetterForRankAtPosition( const UINT rank, const UINT position ) const;
	float					GetWeightForLetterAtPosition( const CHAR letter, const UINT position ) const;

private:
	WordList&				m_wordList;

	struct LetterRankInfo
	{
		CHAR				m_letter = '\0';
		UINT				m_count = 0;
		float				m_weight = 0.0f;

		void				Reset()
		{
			m_letter = '\0';
			m_count = 0;
			m_weight = 0.0f;
		}
	};
	LetterRankInfo			m_letterRanksPerPosition[ NumLetters ][ Word::WordLength ];
	bool					m_analysisRun;
};

}
