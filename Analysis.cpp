#include "pch.h"
#include "Analysis.h"
#include "WordList.h"

namespace wh
{
const float		Analysis::CorrectLetterScore = 0.50f;
const float		Analysis::WrongPositionScore = 0.25f;
const float		Analysis::MultipleLetterPenalty = 1.0f;
const float		Analysis::IncorrectLetterBonus = 0.25f;

Analysis::Analysis( WordList& wordList ):
	m_wordList( wordList ),
	m_analysisRun( false )
{
}

void Analysis::Analyse()
{
	utils::StartTimer();


	for( UINT letterWordIndex = 0; letterWordIndex < Word::WordLength; ++letterWordIndex )
	{
		for( UINT letterIndex = 0; letterIndex < NumLetters; ++letterIndex )
		{
			m_letterRanksPerPosition[ letterIndex ][ letterWordIndex ].Reset();
			m_letterRanksPerPosition[ letterIndex ][ letterWordIndex ].m_letter = static_cast<CHAR>( FirstLetterOffset + letterIndex );
		}
	}
	
	const containers::List<Word>& wordList = m_wordList.GetWordList();
	containers::List<Word>::const_iterator itor = wordList.begin();
	while( itor != wordList.end() )
	{
		const Word& word = *itor;
		for( UINT letterIndex = 0; letterIndex < Word::WordLength; ++letterIndex )
		{
			CHAR letter = word.GetLetterAtPosition( letterIndex ) - FirstLetterOffset;
			++m_letterRanksPerPosition[ letter ][ letterIndex ].m_count;
		}
		itor++;
	}

	for( UINT letterWordIndex = 0; letterWordIndex < Word::WordLength; ++letterWordIndex)
	{
		// Sort the letters
		UINT highestCount = 0;
		for( UINT letterIndex = 0; letterIndex < NumLetters; ++letterIndex )
		{
			UINT highestCountIndex = letterIndex;

			// Find the remaining letter with the highest count
			for( UINT letterIndexSort = letterIndex; letterIndexSort < NumLetters; ++letterIndexSort )
			{
				if( m_letterRanksPerPosition[ letterIndexSort ][ letterWordIndex ].m_count > highestCount )
				{
					highestCount = m_letterRanksPerPosition[ letterIndexSort ][ letterWordIndex ].m_count;
					highestCountIndex = letterIndexSort;
				}
			}

			// Swap the first entry with the highest
			LetterRankInfo swap = m_letterRanksPerPosition[ highestCountIndex ][ letterWordIndex ];
			m_letterRanksPerPosition[ highestCountIndex ][ letterWordIndex ] = m_letterRanksPerPosition[ letterIndex ][ letterWordIndex ];
			m_letterRanksPerPosition[ letterIndex ][ letterWordIndex ] = swap;
		}

		ASSERT( highestCount > 0, "There is no highest count\n" );
		for( UINT letterIndex = 0; letterIndex < NumLetters; ++letterIndex )
		{
			m_letterRanksPerPosition[ letterIndex ][ letterWordIndex ].m_weight = static_cast<float>( m_letterRanksPerPosition[ letterIndex ][ letterWordIndex ].m_count ) / static_cast<float>( highestCount );
		}
	}

	utils::EndTimer( "Analysis" );

	m_analysisRun = true;
}

CHAR Analysis::GetLetterForRankAtPosition( const UINT rank, const UINT position ) const
{
	ASSERT( m_analysisRun, "Analysis hasn't been run yet\n" );
	ASSERT( position < Word::WordLength, "Letter position in word out of range\n" );
	ASSERT( rank < NumLetters, "Letter rank %u exceeds number of letters\n", rank );

	return m_letterRanksPerPosition[ rank ][ position ].m_letter;
}

float Analysis::GetWeightForLetterAtPosition( const CHAR letter, const UINT position ) const
{
	ASSERT( m_analysisRun, "Analysis hasn't been run yet\n" );
	ASSERT( position < Word::WordLength, "Letter position in word out of range\n" );

	// Find the letter in the array
	float weight = 0.0f;
	for( UINT letterIndex = 0; letterIndex < NumLetters; ++letterIndex )
	{
		if( m_letterRanksPerPosition[ letterIndex ][ position ].m_letter == letter )
		{
			weight = m_letterRanksPerPosition[ letterIndex ][ position ].m_weight;
			break;
		}
	}

	return weight;
}

float Analysis::RateWord( const Word& guessWord, const Word& testWord ) const
{
	float rating = 0.0f;
	for( UINT letterIndex = 0; letterIndex < Word::WordLength; ++letterIndex )
	{
		bool found = false;
		const CHAR testLetter = testWord.GetLetterAtPosition( letterIndex );
		const CHAR currentLetter = guessWord.GetLetterAtPosition( letterIndex );
		if( testLetter == currentLetter )
		{
			// Correct letter
			rating += CorrectLetterScore;
			found = true;
		}
		else
		{
			// Not the correct letter - used in another part of the word?
			for( UINT checkLetter = 0; checkLetter < Word::WordLength; ++checkLetter )
			{
				if( checkLetter == letterIndex )
					continue;

				if( testLetter == currentLetter )
				{
					found = true;
					break;
				}
			}

			if( found )
				rating += WrongPositionScore;
		}

		if( !found )
		{
			// Not being found is also useful as it eliminates letters - award a score for eliminated common letters
			rating += IncorrectLetterBonus * GetWeightForLetterAtPosition( testLetter, letterIndex );
		}

		// If this letter already occurs in the test word, it's less useful, so lower the score
		for( UINT repeatLetterCheckIndex = 0; repeatLetterCheckIndex < letterIndex; ++repeatLetterCheckIndex )
		{
			if( guessWord.GetLetterAtPosition( repeatLetterCheckIndex ) == currentLetter )
			{
				rating -= MultipleLetterPenalty;
			}
		}
	}

	return rating;
}

}