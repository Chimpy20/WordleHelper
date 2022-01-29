#pragma once

#include "Debug.h"

namespace containers
{

template <class T>
class List;

//class Iterator;
//class ConstIterator;

class NodeBase
{
public:
	NodeBase() :
		m_next( this ),
		m_prev( this )
	{
	}
	~NodeBase()
	{
		unlink();
	}

	NodeBase*		m_next;
	NodeBase*		m_prev;

	void push_back( NodeBase* newNode )
	{
		newNode->m_next = this;
		newNode->m_prev = m_prev;
		m_prev->m_next = newNode;
		m_prev = newNode;
	}

	void unlink()
	{
		NodeBase *next = m_next, *prev = m_prev;
		next->m_prev = prev;
		prev->m_next = next;
		m_next = this;
		m_prev = this;
	}
};

template<class T>
class Node : public NodeBase
{
public:
	Node() : m_data() {}
	Node( T t ) : m_data( t ) {}

	T				m_data;
};

template<typename T>
class Iterator
{
public:
	NodeBase*		m_node;

	Iterator() : m_node() {}
	Iterator( NodeBase* node )
		: m_node( node ) {}

	const T& operator*() const
	{
		return static_cast<const Node<T>*>( m_node )->m_data;
	}

	const T* operator->() const
	{
		return &static_cast<const Node<T>*>( m_node )->m_data;
	}

	Iterator& operator++()
	{
		m_node = m_node->m_next;
		return *this;
	}

	Iterator operator++( int )
	{
		Iterator tmp = *this;
		m_node = m_node->m_next;
		return tmp;
	}

	bool operator==( Iterator other ) const { return m_node == other.m_node; }
	bool operator!=( Iterator other ) const { return m_node != other.m_node; }

};

template<typename T>
class ConstIterator
{
public:
	const NodeBase* m_node;

	ConstIterator(): m_node() {}
	ConstIterator( const NodeBase* node ) : m_node( node ) {}
	ConstIterator( const Iterator<T>& node ) : m_node( node.m_node ) {}

	const T& operator*() const
	{
		return static_cast<const Node<T>*>( m_node )->m_data;
	}
	
	const T* operator->() const
	{
		return &static_cast<const Node<T>*>( m_node )->m_data;
	}

	ConstIterator& operator++()
	{
		m_node = m_node->m_next;
		return *this;
	}

	ConstIterator operator++( int )
	{
		ConstIterator tmp = *this;
		m_node = m_node->m_next;
		return tmp;
	}

	bool operator==( ConstIterator other ) const { return m_node == other.m_node; }
	bool operator!=( ConstIterator other ) const { return m_node != other.m_node; }

};

template <class T>
class List
{
private:
	/*class NodeT : public Node
	{
		friend class List<T>;
		T			m_value;
		NodeT( T t ) : m_value( t ){}
	};

	template<class U>
	class IteratorT : public Iterator
	{
	private:
		friend class List<T>;

		NodeT* node() const
		{
			return static_cast<NodeT*>( m_node );
		}

		NodeT* node()
		{
			return static_cast<NodeT*>( m_node );
		}

	public:
		IteratorT( Node* node ) : Iterator( node ){}
		U& operator*()
		{
			return node()->m_value;
		}
		U& operator*() const
		{
			return node()->m_value;
		}
		U* operator->()
		{
			return &node()->m_value;
		}
		U* operator->() const
		{
			return &node()->m_value;
		}
		operator IteratorT<U const>()
		{
			return m_node;
		}
		operator IteratorT<U const>() const
		{
			return m_node;
		}
	};*/

	Node<T>			m_head;
	size_t			m_size;

public:
	typedef Iterator<T> iterator;
	typedef ConstIterator<T> const_iterator;

	List():
		m_size( 0 )
	{
	}

	~List()
	{
		clear();
	}

	inline bool empty() const
	{
		return m_head.m_next == &m_head;
	}

	inline iterator begin()
	{
		return m_head.m_next;
	}

	inline const_iterator begin() const
	{
		return m_head.m_next;
	}

	inline iterator end()
	{
		return &m_head;
	}

	inline const_iterator end() const
	{
		return &m_head;
	}

	inline void push_back( T t )
	{
		m_head.push_back( new Node<T>( t ) );
		++m_size;
	}

	inline void erase( iterator i )
	{
		delete i.m_node;
		ASSERT( m_size > 0, "Trying to erase from a non-empty list\n" );
		--m_size;
	}

	inline void clear()
	{
		while( !empty() )
			erase( begin() );

		ASSERT( m_size == 0, "Finished erase but size is still %u.\n", m_size );
		m_size = 0;
	}

	inline size_t size() const
	{
		return m_size;
	}
};

}
