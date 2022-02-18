#pragma once

#include "List.h"

namespace wh
{

class MessageLog
{
public:
	static const UINT		MaxLogMessageSize = 512;
	MessageLog() {}
	~MessageLog() {}
	void					Reset();
	bool					AddMessage( const CHAR* const messageText, const UINT numChars );
	UINT					GetCombinedMessages( CHAR* log, const UINT logMaxSize ) const;

private:
	class Message
	{
	public:
		Message()
		{
			m_message[ 0 ] = '\0';
		}
		Message( const CHAR* const messageText )
		{
			_strncpy( m_message, messageText, MaxLogMessageSize );
		}

		CHAR				m_message[ MaxLogMessageSize ];
	};

	typedef containers::List<Message> MessageListContainer;

	MessageListContainer	m_messageList;
};

} // namespace wh
