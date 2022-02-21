#pragma once

namespace system
{
namespace memory
{

class Heap
{
public:
	static bool		Create();
	static void		Destroy();

	static void* Alloc( size_t size );
	static void		Free( void* p );

private:
	static HANDLE	sm_heap;
	static UINT64	sm_heapAllocationsTotal;
#ifdef _DEBUG
	static UINT64	sm_maxHeap;
#endif
};

} // namespace memory
} // namespace system

void* operator new( size_t size );
void* operator new[]( size_t size );
void operator delete( void* p );
void operator delete( void* p, unsigned __int64 size );
