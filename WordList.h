#pragma once

class WordList
{
public:
	WordList();
	~WordList();

	UINT ReadWords( const WCHAR* wordListFileName );

private:
	HANDLE		m_heap;
	CHAR*		m_wordListRaw;
};