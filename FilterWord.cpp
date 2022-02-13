#include "pch.h"
#include "FilterWord.h"

namespace wa
{

FilterWord::FilterWord( const CHAR letters[ WordLength ], const FilterLetterState filteredLetterStates[ WordLength ] )
{
	for( UINT letterIndex = 0; letterIndex < WordLength; ++letterIndex )
	{
		m_letters[ letterIndex ] = MakeLower( letters[ letterIndex ] );
		m_filterLetterStates[ letterIndex ] = filteredLetterStates[ letterIndex ];
	}
}

FilterLetterState FilterWord::GetStateAtPos( const UINT letterPosition ) const
{
	ASSERT( letterPosition < WordLength, "Letter position of %u out of range\n", letterPosition );
	return m_filterLetterStates[ letterPosition ];
}


bool FilterWord::PotentialMatch( const Word& word ) const
{
	bool potentialMatch = true;
	for( UINT letterIndex = 0; letterIndex < WordLength; ++letterIndex )
	{
		const CHAR letter = m_letters[ letterIndex ];

		switch( m_filterLetterStates[ letterIndex ] )
		{
			case FilterLetterState::Incorrect:
			{
				// If an incorrect letter is in the test word at all, then it can't be a match
				for( UINT testWordLetterIndex = 0; testWordLetterIndex < WordLength; ++testWordLetterIndex )
				{
					const CHAR testLetter = word.GetLetterAtPosition( testWordLetterIndex );

					bool prevLetterFound = false;
					for( UINT prevLetterSearchIndex = 0; prevLetterSearchIndex < testWordLetterIndex; ++prevLetterSearchIndex )
					{
						if( m_letters[ prevLetterSearchIndex ] == testLetter )
						{
							// We've seen this letter before, so ignore this one, as the previous letter will either be wrong
							// causing the word to be discarded, or the earlier letter will have been correct, in which case
							// we can skip checking further
							prevLetterFound = true;
							break;
						}
					}

					if( prevLetterFound )
						break;

					if( testLetter == letter )
					{
						bool repeatLetterCorrect = false;

						// Work back from the end and ensure this letter doesn't appear again in the correct position
						for( UINT repeatLetterSearchIndex = WordLength - 1; repeatLetterSearchIndex > testWordLetterIndex; --repeatLetterSearchIndex )
						{
							const CHAR repeatLetter = m_letters[ repeatLetterSearchIndex ];
							if( testLetter == repeatLetter )
							{
								if( m_filterLetterStates[ letterIndex ] == FilterLetterState::Correct )
								{
									// We've found an incorrect letter but the same letter is oorrect later on. This letter is acceptable
									repeatLetterCorrect = true;
								}
							}
						}
						if( !repeatLetterCorrect )
						{
							potentialMatch = false;
							break;
						}
					}
				}
			}
			break;

		case FilterLetterState::WrongPosition:
			{
				// check to see if the letter is in another position, if it is, this could be a match
				bool isLetterInWordAtAnyPlace = false;
				for( UINT testWordLetterIndex = 0; testWordLetterIndex < WordLength; ++testWordLetterIndex )
				{
					if( testWordLetterIndex != letterIndex )
					{
						if( word.GetLetterAtPosition( testWordLetterIndex ) == letter )
						{
							isLetterInWordAtAnyPlace = true;
							break;
						}
					}
				}
				if( !isLetterInWordAtAnyPlace )
				{
					potentialMatch = false;
				}
			}
			break;

		case FilterLetterState::Correct:
			{
				// If a wrong letter is in a position we know is correct, then this word can't be right
				if( word.GetLetterAtPosition( letterIndex ) != letter )
				{
					potentialMatch = false;
				}
			}
			break;

		default:
			break;
		}

		if( !potentialMatch )
			break;
	}
	return potentialMatch;
}

}