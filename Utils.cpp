#include "pch.h"
#include "Utils.h"

namespace utils
{

static unsigned int randSeed = 0;
unsigned int Rand()
{
	return( ( randSeed++ * 1103515245u ) + 12345 );
}

}