#include "pch.h"
#include <mddbootstrap.h>
#include "WordleAnalyser.h"

int main()
{
	if( !memory::Heap::Create() )
	{
		return -1;
	}

    wa::WordleAnalyser wordleAnalyser;
    wordleAnalyser.Initialise();

	memory::Heap::Destroy();

    return 0;
}
