#include "pch.h"
#include "WordList.h"

WordList::WordList():
	m_wordListRaw( nullptr )
{
	m_heap = HeapCreate( 0, 0, 0 );
}

WordList::~WordList()
{
	if( m_wordListRaw != nullptr )
	{
		HeapFree( m_heap, 0, m_wordListRaw );
	}

	HeapDestroy( m_heap );
}

UINT WordList::ReadWords( const WCHAR* wordListFileName )
{
	HANDLE wordListFileHandle = CreateFileW( wordListFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr );
	if( wordListFileHandle == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	UINT numWordsRead = 0;

	LARGE_INTEGER fileSize;
	if( GetFileSizeEx( wordListFileHandle, &fileSize ) )
	{
		m_wordListRaw = (CHAR*)HeapAlloc( m_heap, 0, fileSize.QuadPart );
		DWORD bytesRead = 0;
		if( ReadFile( wordListFileHandle, m_wordListRaw, fileSize.LowPart, &bytesRead, nullptr ) )
		{
			ASSERT( bytesRead == fileSize.LowPart, "Wrong number of bytes read, read %u, expecting %u\n", bytesRead, fileSize.LowPart );
			if( bytesRead == fileSize.LowPart )
			{
			}
		}
	}

	CloseHandle( wordListFileHandle );

	return numWordsRead;
}