#include "pch.h"
#include "WordleAnalyser.h"

int main()
{
	if( !memory::Heap::Create() )
	{
		return -1;
	}

	LARGE_INTEGER frequency, startTime, endTime;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &startTime );

	wa::WordleAnalyser wordleAnalyser;
	const UINT wordsRead = wordleAnalyser.Initialise();
	wordleAnalyser.GenerateStats();

	QueryPerformanceCounter( &endTime );
	const float readDuration = static_cast<float>( endTime.QuadPart - startTime.QuadPart ) * 1000.0f / static_cast<float>( frequency.QuadPart );

	io::OutputMessage( "Read %u words in %.3fms\n", wordsRead, readDuration );

	wordleAnalyser.Shutdown();
	memory::Heap::Destroy();

    return 0;
}
