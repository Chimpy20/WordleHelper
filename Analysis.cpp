#include "pch.h"
#include "Analysis.h"
#include "WordList.h"

namespace wh
{

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

}