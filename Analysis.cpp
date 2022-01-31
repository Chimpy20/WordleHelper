#include "pch.h"
#include "Analysis.h"
#include "WordList.h"

namespace wa
{

Analysis::Analysis( WordList& wordList ):
	m_wordList( wordList )
{
}

void Analysis::Analyse()
{
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
		Word word = *itor;
		for( UINT letterIndex = 0; letterIndex < Word::WordLength; ++letterIndex )
		{
			CHAR letter = word.GetLetterAtPosition( letterIndex ) - FirstLetterOffset;
			++m_overallLetterCountsPerPosition[ letter ][ letterIndex ];
		}
		itor++;
	}
}

}