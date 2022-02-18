#include "pch.h"
#include "MessageLog.h"

namespace wh
{

void MessageLog::Reset()
{
	m_messageList.clear();
}

bool MessageLog::AddMessage( const CHAR* const messageText, const UINT numChars )
{
	Message message( messageText );
	m_messageList.push_back( message );
	io::OutputMessage( messageText );
	return true;
}

UINT MessageLog::GetCombinedMessages( CHAR* log, const UINT logMaxSize ) const
{
	MessageListContainer::const_iterator itor = m_messageList.begin();
	INT offset = 0;
	INT charsRemaining = logMaxSize;
	while( itor != m_messageList.end() )
	{
		const Message& message = *itor;
		_strncpy( log + offset, message.m_message, charsRemaining );

		const UINT messageSize = strnlen_( message.m_message, MaxLogMessageSize );
		offset += messageSize;
		charsRemaining -= messageSize;
		if( charsRemaining <= 0 )
			break;

		itor++;
	}

	return offset;
}

} // namespace wh