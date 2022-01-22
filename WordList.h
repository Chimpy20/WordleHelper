#pragma once

class WordList
{
public:
	WordList();
	~WordList();

	UINT		ReadWords( const WCHAR* wordListFileName );

private:
	static const UINT MaxWordBufferSize = 16;
	UINT		ExtractWords();
	bool		IsLetterAlpha( const CHAR letter );

	HANDLE		m_heap;
	CHAR*		m_wordListRaw;
	UINT		m_wordListRawSize;
};