#pragma once

class WordleAnalyser
{
public:
	WordleAnalyser();
	~WordleAnalyser();

	bool Initialise();

	static const WCHAR* const WORD_LIST_FILENAME;
};
