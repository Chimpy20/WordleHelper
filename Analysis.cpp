#include "pch.h"
#include "Analysis.h"
#include "WordList.h"

namespace wa
{

Analysis::Analysis( WordList& wordList ):
	m_wordList( wordList ),
	m_analysisRun( false )
{
}

void Analysis::Analyse()
{
	LARGE_INTEGER frequency, startTime, endTime;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &startTime );

	const UINT numEntries = NumLetters * Word::WordLength;
	UINT* entriesBase = &( m_overallLetterCountsPerPosition[ 0 ][ 0 ] );
	for( UINT entry = 0; entry < numEntries; ++entry )
	{
		entriesBase[ entry ] = 0;
	}

	const containers::List<Word>& wordList = m_wordList.GetWordList();
	containers::List<Word>::const_iterator itor = wordList.begin();
	while( itor != wordList.end() )
	{
		const Word& word = *itor;
		for( UINT letterIndex = 0; letterIndex < Word::WordLength; ++letterIndex )
		{
			CHAR letter = word.GetLetterAtPosition( letterIndex ) - FirstLetterOffset;
			++m_overallLetterCountsPerPosition[ letter ][ letterIndex ];
		}
		itor++;
	}

	for( UINT letterWordIndex = 0; letterWordIndex < Word::WordLength; ++letterWordIndex)
	{
		for( UINT letterIndex = 0; letterIndex < NumLetters; ++letterIndex )
		{
			m_letterRankingsPerPosition[ letterIndex ][ letterWordIndex ] = static_cast<CHAR>( FirstLetterOffset + letterIndex );
		}

		// Sort the letters
		for( UINT letterIndex = 0; letterIndex < NumLetters; ++letterIndex )
		{
			UINT highestCount = 0;
			UINT highestCountIndex = letterIndex;

			// Find the remaining letter with the highest count
			for( UINT letterIndexSort = letterIndex; letterIndexSort < NumLetters; ++letterIndexSort )
			{
				if( m_overallLetterCountsPerPosition[ letterIndexSort ][ letterWordIndex ] > highestCount )
				{
					highestCount = m_overallLetterCountsPerPosition[ letterIndexSort ][ letterWordIndex ];
					highestCountIndex = letterIndexSort;
				}
			}

			// Swap the first entry with the highest
			UINT swapCount = m_overallLetterCountsPerPosition[ highestCountIndex ][ letterWordIndex ];
			m_overallLetterCountsPerPosition[ highestCountIndex ][ letterWordIndex ] = m_overallLetterCountsPerPosition[ letterIndex ][ letterWordIndex ];
			m_overallLetterCountsPerPosition[ letterIndex ][ letterWordIndex ] = swapCount;

			CHAR swapLetter = m_letterRankingsPerPosition[ highestCountIndex ][ letterWordIndex ];
			m_letterRankingsPerPosition[ highestCountIndex ][ letterWordIndex ] = m_letterRankingsPerPosition[ letterIndex ][ letterWordIndex ];
			m_letterRankingsPerPosition[ letterIndex ][ letterWordIndex ] = swapLetter;
		}
	}

	m_analysisRun = true;

	QueryPerformanceCounter( &endTime );
	const float analyseDuration = static_cast<float>( endTime.QuadPart - startTime.QuadPart ) * 1000.0f / static_cast<float>( frequency.QuadPart );
	io::OutputMessage( "Run took %.4fms\n", analyseDuration );
}

CHAR Analysis::GetLetterRankAtPosition( const UINT rank, const UINT position )
{
	ASSERT( m_analysisRun, "Analysis hasn't been run yet\n" );
	ASSERT( position < Word::WordLength, "Letter position in word out of range\n" );
	ASSERT( rank < NumLetters, "Letter rank %u exceeds number of letters\n", rank );

	return m_letterRankingsPerPosition[ rank ][ position ];
}

}