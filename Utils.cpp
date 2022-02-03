#include "pch.h"
#include "Utils.h"

namespace utils
{

static unsigned int randSeed = 0;
unsigned int Rand()
{
	return( ( randSeed++ * 1103515245u ) + 12345 );
}

LARGE_INTEGER g_startTime = LARGE_INTEGER(0u);
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

}