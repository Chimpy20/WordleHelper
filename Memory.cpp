#include "pch.h"
#include "Memory.h"

namespace memory
{
	HANDLE Heap::sm_heap = INVALID_HANDLE_VALUE;

	bool Heap::Create()
	{
		sm_heap = HeapCreate( 0, 0, 0 );
		return( sm_heap != INVALID_HANDLE_VALUE );
	}

	void Heap::Destroy()
	{
		HeapDestroy( sm_heap );
		sm_heap = INVALID_HANDLE_VALUE;
	}

	void* Heap::Alloc( size_t size )
	{
		ASSERT( sm_heap != INVALID_HANDLE_VALUE, "Heap not created\n" );
		void* const newAllocation = HeapAlloc( sm_heap, 0, size );
		ASSERT( newAllocation != nullptr, "Unable to allocate heap memory of size %u", size );
		return newAllocation;
	}

	void Heap::Free( void* p )
	{
		ASSERT( sm_heap != INVALID_HANDLE_VALUE, "Heap not created\n" );
		HeapFree( sm_heap, 0, p );
	}
}

void* operator new( size_t size )
{
	return memory::Heap::Alloc( size );
}

void operator delete( void* p )
{
	memory::Heap::Free( p );
}

void operator delete( void* p, unsigned __int64 size )
{
	memory::Heap::Free( p );
}
