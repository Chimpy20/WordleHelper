#pragma once

namespace containers
{

template <class T>
class Vector;

template <typename T>
struct VectorBase
{
public:
	struct VectorImpl
	{
		T* m_start;
		T* m_finish;
		T* m_storageEnd;
		VectorImpl() : m_start( nullptr ),
			m_finish( nullptr ),
			m_storageEnd( nullptr )
		{
		}
	};

	VectorBase() : VectorImpl() {}
	VectorBase( size_t size ) : VectorImpl()
	{
		m_impl.m_start = new T[ size ];
		m_impl.m_finish = m_impl.m_start;
		m_impl.m_storageEnd = m_impl.m_start + size;
	}

	~VectorBase()
	{
		delete m_impl.m_start;
	}

public:
	VectorImpl	m_impl;
	/*00111
		00112       _Tp *
		00113       _M_allocate( size_t __n ) { return _M_impl.allocate( __n ); }
	00114
		00115       void
		00116       _M_deallocate( _Tp * __p, size_t __n )
		00117       { if( __p ) _M_impl.deallocate( __p, __n ); }
	00118     };*/
};

} // namespace containers