#include "pch.h"
#include "System\Memory.h"

namespace system
{

namespace memory
{

HANDLE Heap::sm_heap = INVALID_HANDLE_VALUE;
UINT64 Heap::sm_heapAllocationsTotal = 0;

bool Heap::Create()
{
	sm_heap = HeapCreate( 0, 0, 0 );
	sm_heapAllocationsTotal = 0;
	return( sm_heap != INVALID_HANDLE_VALUE );
}

void Heap::Destroy()
{
	ASSERT( sm_heapAllocationsTotal == 0, "There is still %u bytes allocated on the heap\n", sm_heapAllocationsTotal );
	HeapDestroy( sm_heap );
	sm_heap = INVALID_HANDLE_VALUE;
}

void* Heap::Alloc( size_t size )
{
	ASSERT( sm_heap != INVALID_HANDLE_VALUE, "Heap not created\n" );
	void* const newAllocation = HeapAlloc( sm_heap, 0, size );
	ASSERT( newAllocation != nullptr, "Unable to allocate heap memory of size %u", size );
	sm_heapAllocationsTotal += size;
	return newAllocation;
}

void Heap::Free( void* p )
{
	ASSERT( sm_heap != INVALID_HANDLE_VALUE, "Heap not created\n" );
	const size_t memoryFreed = HeapSize( sm_heap, 0, p );
	ASSERT( memoryFreed <= sm_heapAllocationsTotal, "Trying to free more memory than has been allocated\n" );
	sm_heapAllocationsTotal -= memoryFreed;
	HeapFree( sm_heap, 0, p );
}

} // namespace memory

} // namespace system


void* operator new( size_t size )
{
	return system::memory::Heap::Alloc( size );
}

void* operator new[]( size_t size )
{
	return system::memory::Heap::Alloc( size );
}

void operator delete( void* p )
{
	system::memory::Heap::Free( p );
}

void operator delete( void* p, unsigned __int64 size )
{
	system::memory::Heap::Free( p );
}
