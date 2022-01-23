#include "pch.h"
#include "WordleAnalyser.h"

namespace wa
{

const WCHAR* const WordleAnalyser::WORD_LIST_FILENAME = L"wordlist.txt";

WordleAnalyser::WordleAnalyser()
{
}

WordleAnalyser::~WordleAnalyser()
{
}

bool WordleAnalyser::Initialise()
{
	m_wordList = new WordList();
	const UINT wordsRead = m_wordList->ReadWords( WORD_LIST_FILENAME );

	io::OutputMessage( "Read %u words\n", wordsRead );

	delete m_wordList;

	DEBUG_MESSAGE( "Test %u %s\n", 15, "Melons" );

	return true;
}

}
