#pragma once

#include "IO.h"

#ifdef _DEBUG

#define DEBUG_MESSAGE( message, ... ) \
OutputMessage( message, __VA_ARGS__ )

#define ASSERT(cond, msg, ...) \
do \
{ \
if( !( cond ) ) \
{ \
OutputMessage( "Assert failure: %s at %s %u\n",  #cond, __FILE__, __LINE__ ); \
OutputMessage( msg, __VA_ARGS__ ); \
DebugBreak(); \
} \
} while( 0 )

#else

#define DEBUG_MESSAGE( message, ...) \
do { (void)sizeof( message ); } while( 0 )

#define ASSERT(cond, msg, ...) \
do { (void)sizeof( cond ); } while( 0 )

#endif

