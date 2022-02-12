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
				potentialMatch = isLetterInWordAtAnyPlace;
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