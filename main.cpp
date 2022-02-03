#include "pch.h"
#include "WordleAnalyser.h"
#include "System.h"

int main()
{
	if( !memory::Heap::Create() )
	{
		return false;
	}

	LARGE_INTEGER frequency, startTime, endTime;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &startTime );
	wa::WordleAnalyser wordleAnalyser;
	const UINT wordsRead = wordleAnalyser.Initialise();
	QueryPerformanceCounter( &endTime );
	const float readDuration = static_cast<float>( endTime.QuadPart - startTime.QuadPart ) * 1000.0f / static_cast<float>( frequency.QuadPart );
	io::OutputMessage( "Initialisation with %u words took %.4fms\n", wordsRead, readDuration );

	wordleAnalyser.Run();

	wordleAnalyser.Shutdown();

	memory::Heap::Destroy();

    return 0;
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow )
{
	if( system::Initialise( instance, cmdLine, cmdShow ) )
	{
		system::Run();
		system::Shutdown();
	}

	return 0;
}
