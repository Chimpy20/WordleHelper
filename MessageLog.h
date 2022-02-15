#pragma once

namespace wa
{

class MessageLog
{
public:
	MessageLog() {}
	~MessageLog() {}
	void				Reset();
	bool				AddMessage( const CHAR* const message, const UINT numChars );
	UINT				GetMessageLog( CHAR* const log, const UINT logMaxSize );
};

} // namespace wa
