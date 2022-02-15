#pragma once

namespace utils
{

unsigned int	Rand();
void			StartTimer();
void			EndTimer( const CHAR* tag );

// @return is the provided letter in the range 'a' to 'z', or 'A' to 'Z'?
bool			IsLetterAlpha( const CHAR letter );

}
