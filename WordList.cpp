#include "pch.h"
#include "WordList.h"
#include "Word.h"
#include "Analysis.h"

namespace wa
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

UINT WordList::ReadWords( const WCHAR* wordListFileName, bool append )
{
	CHAR* wordListRaw = nullptr;
	UINT wordListRawSize = 0;

	HANDLE wordListFileHandle = CreateFileW( wordListFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr );
	if( wordListFileHandle == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	UINT numWordsRead = 0;

	LARGE_INTEGER fileSize;
	if( GetFileSizeEx( wordListFileHandle, &fileSize ) )
	{
		wordListRaw = (CHAR*)memory::Heap::Alloc( fileSize.QuadPart );
		DWORD bytesRead = 0;
		if( ReadFile( wordListFileHandle, wordListRaw, fileSize.LowPart, &bytesRead, nullptr ) )
		{
			ASSERT( bytesRead == fileSize.LowPart, "Wrong number of bytes read, read %u, expecting %u\n", bytesRead, fileSize.LowPart );
			if( bytesRead == fileSize.LowPart )
			{
				// Clear the list before if we're not appending
				if( !append )
				{
					m_wordList.clear();
				}
				wordListRawSize = bytesRead;
				numWordsRead = ExtractWords( wordListRaw, wordListRawSize );
			}
		}
	}

	if( wordListRaw != nullptr )
	{
		memory::Heap::Free( wordListRaw );
	}

	CloseHandle( wordListFileHandle );

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

		while( IsLetterAlpha( currentLetter ) &&
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
	LARGE_INTEGER frequency, startTime, endTime;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &startTime );

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

	QueryPerformanceCounter( &endTime );
	const float randomiseDuration = static_cast<float>( endTime.QuadPart - startTime.QuadPart ) * 1000.0f / static_cast<float>( frequency.QuadPart );
	io::OutputMessage( "Randomising took %.4fms\n", randomiseDuration );
}

bool WordList::IsLetterAlpha( const CHAR letter )
{
	if( ( letter >= 'a' && letter <= 'z' ) ||
		( letter >= 'A' && letter <= 'Z' ) )
		return true;

	return false;
}

UINT WordList::DuplicateFrom( const WordList& other )
{
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

void WordList::OutputWords() const
{
	WordListContainer::const_iterator itor = m_wordList.begin();
	while( itor != m_wordList.end() )
	{
		Word word = *itor;
		io::OutputMessage( "\t%s\n", word.GetAsString() );
		itor++;
	}
}

void WordList::Guess( const WordList& masterWordList )
{
	m_analysis.Analyse();
	m_guesser.Guess( masterWordList, m_analysis );
}

}