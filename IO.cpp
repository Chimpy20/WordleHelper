#include "pch.h"
#include "IO.h"
#include "printf.h"

namespace io
{

static const UINT MaxDebugMessageSize = 1024;

void OutputMessage( const CHAR* const message, ... )
{
	CHAR messageBuffer[ MaxDebugMessageSize ];
	va_list args;
	va_start( args, message );
	vsnprintf_( messageBuffer, MaxDebugMessageSize, message, args );
	va_end( args );

	OutputDebugStringA( messageBuffer );
}

}
