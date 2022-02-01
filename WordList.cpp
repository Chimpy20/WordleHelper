#include "pch.h"
#include "WordList.h"
#include "Word.h"
#include "Analysis.h"

namespace wa
{

WordList::WordList() :
	m_wordListRaw( nullptr ),
	m_wordListRawSize( 0 ),
	m_analysis(*this)
{
}

WordList::~WordList()
{
	m_wordList.clear();

	if( m_wordListRaw != nullptr )
	{
		memory::Heap::Free( m_wordListRaw );
	}
}

UINT WordList::ReadWords( const WCHAR* wordListFileName )
{
#ifdef _DEBUG
	if( m_wordListRawSize != 0 )
	{
		io::OutputMessage( "Word list already has raw data, is it intended to read more?\n" );
	}
#endif

	HANDLE wordListFileHandle = CreateFileW( wordListFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr );
	if( wordListFileHandle == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	UINT numWordsRead = 0;

	LARGE_INTEGER fileSize;
	if( GetFileSizeEx( wordListFileHandle, &fileSize ) )
	{
		m_wordListRaw = (CHAR*)memory::Heap::Alloc( fileSize.QuadPart );
		DWORD bytesRead = 0;
		if( ReadFile( wordListFileHandle, m_wordListRaw, fileSize.LowPart, &bytesRead, nullptr ) )
		{
			ASSERT( bytesRead == fileSize.LowPart, "Wrong number of bytes read, read %u, expecting %u\n", bytesRead, fileSize.LowPart );
			if( bytesRead == fileSize.LowPart )
			{
				m_wordListRawSize = bytesRead;
				numWordsRead = ExtractWords();
			}
		}
	}

	CloseHandle( wordListFileHandle );

	return numWordsRead;
}

UINT WordList::ExtractWords()
{
	UINT numWords = 0;
	CHAR currentWord[ MaxWordBufferSize ];

	UINT currentWordListBufferPos = 0;
	UINT offset = 0;
	CHAR currentLetter = 0;

	while( currentWordListBufferPos < m_wordListRawSize )
	{
		currentLetter = m_wordListRaw[ currentWordListBufferPos + offset ];

		while( IsLetterAlpha( currentLetter ) &&
			( offset < MaxWordBufferSize ) &&
			( ( currentWordListBufferPos + offset ) < m_wordListRawSize ) )
		{
			currentWord[ offset ] = currentLetter;
			++offset;
			currentLetter = m_wordListRaw[ currentWordListBufferPos + offset ];
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
	const containers::List<Word>& sourceWordList = other.m_wordList;
	containers::List<Word>::const_iterator itor = sourceWordList.begin();
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
	containers::List<Word>::iterator itor = m_wordList.begin();
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

void WordList::OutputWords()
{
	containers::List<Word>::const_iterator itor = m_wordList.begin();
	while( itor != m_wordList.end() )
	{
		Word word = *itor;
		io::OutputMessage( "\t%s\n", word.GetAsString() );
		itor++;
	}
}

void WordList::Analyse()
{
	m_analysis.Analyse();
}

}