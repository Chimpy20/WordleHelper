#include "pch.h"
#include "FilterWord.h"

namespace wa
{

FilterWord::FilterWord()
{
}

FilterWord::FilterWord( const CHAR letters[ WordLength ], const FilterLetterState filteredLetterStates[ WordLength ] )
{
	for( UINT letterIndex = 0; letterIndex < WordLength; ++letterIndex )
	{
		m_letters[ letterIndex ] = letters[ letterIndex ];
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
		CHAR letter = m_letters[ letterIndex ];
		// If an incorrect letter is in the test word at all, then it can't be a match
		switch( m_filterLetterStates[ letterIndex ] )
		{
			case FilterLetterState::Incorrect:
			{
				// Search the test word for that letter
				for( UINT testWordLetterIndex = 0; testWordLetterIndex < WordLength; ++testWordLetterIndex )
				{
					if( word.GetLetterAtPosition( testWordLetterIndex ) == letter )
					{
						potentialMatch = false;
						break;
					}
				}
			}
			break;

		case FilterLetterState::WrongPosition:
			{
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
				potentialMatch = isLetterInWordAtAnyPlace;
			}
			break;

		case FilterLetterState::Correct:
			{
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