#pragma once

#include "Word.h"

namespace wh
{

class WordList;

class Analysis
{
public:
	static const UINT		NumLetters = 'z' - 'a' + 1;
	static const UINT		FirstLetterOffset = 'a';

	Analysis( WordList&		wordList );

	void					Analyse();

	float					RateWord( const Word& guessWord, const Word& testWord ) const;
	CHAR					GetLetterForRankAtPosition( const UINT rank, const UINT position ) const;
	float					GetWeightForLetterAtPosition( const CHAR letter, const UINT position ) const;

private:
	static const float		CorrectLetterScore;
	static const float		WrongPositionScore;
	static const float		MultipleLetterPenalty;
	static const float		IncorrectLetterBonus;

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
