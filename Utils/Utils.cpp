#include "pch.h"
#include "Utils.h"

namespace utils
{

static unsigned int randSeed = 0;
unsigned int Rand()
{
	SYSTEMTIME time{};
	GetSystemTime( &time );
	return( ( randSeed++ * 1103515245u ) + 12345 + time.wMinute * 60 + time.wSecond );
}

LARGE_INTEGER g_startTime{ 0 };
void StartTimer()
{
	QueryPerformanceCounter( &g_startTime );
}

void EndTimer( const CHAR* tag )
{
	LARGE_INTEGER frequency, endTime;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &endTime );
	const float duration = static_cast<float>( endTime.QuadPart - g_startTime.QuadPart ) * 1000.0f / static_cast<float>( frequency.QuadPart );
	io::OutputMessage( "%s took %.4fms\n", tag, duration );
}


bool IsLetterAlpha( const CHAR letter )
{
	if( ( letter >= 'a' && letter <= 'z' ) ||
		( letter >= 'A' && letter <= 'Z' ) )
		return true;

	return false;
}

}