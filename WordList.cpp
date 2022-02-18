#include "pch.h"
#include "System.h"
#include "WordList.h"
#include "Word.h"
#include "Analysis.h"

namespace wh
{

WordList::WordList() :
	m_analysis(*this),
	m_guesser(*this)
{
}

WordList::~WordList()
{
	m_wordList.clear();
}

UINT WordList::ReadWords( const INT wordListResourceID, bool append )
{
	UINT numWordsRead = 0;
	UINT fileSize = 0;
	const CHAR* textFileBytes = system::OpenTextFile( wordListResourceID, fileSize );
	CHAR* wordListRaw = (CHAR*)memory::Heap::Alloc( fileSize );
	
	// Clear the list before if we're not appending
	if( !append )
	{
		m_wordList.clear();
	}

	numWordsRead = ExtractWords( textFileBytes, fileSize );

	if( wordListRaw != nullptr )
	{
		memory::Heap::Free( wordListRaw );
	}

	system::CloseTextFile( wordListResourceID );

	return numWordsRead;
}

UINT WordList::ExtractWords( const CHAR* wordListRaw, UINT wordListRawSize )
{
	UINT numWords = 0;
	CHAR currentWord[ MaxWordBufferSize ];

	UINT currentWordListBufferPos = 0;
	UINT offset = 0;
	CHAR currentLetter = 0;

	while( currentWordListBufferPos < wordListRawSize )
	{
		currentLetter = wordListRaw[ currentWordListBufferPos + offset ];

		while( utils::IsLetterAlpha( currentLetter ) &&
			( offset < MaxWordBufferSize ) &&
			( ( currentWordListBufferPos + offset ) < wordListRawSize ) )
		{
			currentWord[ offset ] = currentLetter;
			++offset;
			currentLetter = wordListRaw[ currentWordListBufferPos + offset ];
		};

		if( offset == Word::WordLength )
		{
			// Create word and add it to the word list
			Word word( currentWord );
			m_wordList.push_back( word );
			++numWords;
		}

		currentWordListBufferPos += offset + 1;
		offset = 0;
	};

	return numWords;
}

void WordList::Randomise()
{
	utils::StartTimer();

	const UINT numWords = static_cast<UINT>( m_wordList.size() );
	UINT randomWordIndex = 0;
	WordListContainer::iterator itor = m_wordList.begin();
	while( itor != m_wordList.end() )
	{
		Word& word = *itor;

		randomWordIndex = utils::Rand() % numWords;
		UINT wordIndex = 0;
		WordListContainer::iterator swapItor = m_wordList.begin();
		while( swapItor != m_wordList.end() && ( wordIndex < randomWordIndex ) )
		{
			++swapItor;
			++wordIndex;
		}
		Word& swapWord = *swapItor;
		Word tmp;
		tmp = swapWord;
		swapWord = word;
		word = tmp;

		++itor;
	}

	utils::EndTimer( "Randomise" );
}

UINT WordList::DuplicateFrom( const WordList& other )
{
	m_wordList.clear();
	const WordListContainer& sourceWordList = other.m_wordList;
	WordListContainer::const_iterator itor = sourceWordList.begin();
	while( itor != sourceWordList.end() )
	{
		const Word word = *itor;
		m_wordList.push_back( word );
		itor++;
	}

	return static_cast<UINT>( m_wordList.size() );
}

UINT WordList::Filter( const FilterWord& filterWord )
{
	WordListContainer::iterator itor = m_wordList.begin();
	while( itor != m_wordList.end() )
	{
		Word word = *itor;
#ifdef _DEBUG
		word.Validate();
#endif
		if( !filterWord.PotentialMatch( word ) )
		{
			itor = m_wordList.erase( itor );
		}
		else
		{
			itor++;
		}
	}

	return static_cast<UINT>( m_wordList.size() );
}

#ifdef _DEBUG
void WordList::OutputWords() const
{
	io::OutputMessage( "Words in list are:\n" );
	WordListContainer::const_iterator itor = m_wordList.begin();
	while( itor != m_wordList.end() )
	{
		Word word = *itor;
		io::OutputMessage( "\t%s\n", word.GetAsString() );
		itor++;
	}
}
#endif // _DEBUG

const containers::List<RatedWord>& WordList::Guess( const WordList& masterWordList )
{
	m_guesser.Reset();
	if( m_wordList.size() > 0 )
	{
		m_analysis.Analyse();
		m_guesser.Guess( masterWordList, m_analysis );
	}
	return m_guesser.GetRatedWordList();
}

}