#include "pch.h"
#include "MessageLog.h"

namespace wa
{

void MessageLog::Reset()
{
}

bool MessageLog::AddMessage( const CHAR* const message, const UINT numChars )
{
	io::OutputMessage( message );
	return true;
}

UINT MessageLog::GetMessageLog( CHAR* const log, const UINT logMaxSize )
{
	return 0;
}

} // namespace wa