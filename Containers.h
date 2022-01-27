#pragma once

#include "Debug.h"

namespace containers
{

template <class T>
class List;

class Iterator;

class Node
{
	friend class Iterator;
	template<class T> friend class List;

public:
	Node():
		m_next( this ),
		m_prev( this )
	{
	}
	~Node()
	{
		unlink();
	}

protected:
	Node*			m_next;
	Node*			m_prev;

	void push_back( Node* n )
	{
		n->m_next = this;
		n->m_prev = m_prev;
		m_prev->m_next = n;
		m_prev = n;
	}

	void unlink()
	{
		Node* next = m_next, * prev = m_prev;
		next->m_prev = prev;
		prev->m_next = next;
		m_next = this;
		m_prev = this;
	}
};

class Iterator
{
protected:
	Node*			m_node;

	Iterator( Node* node )
		: m_node( node )
	{}

public:
	Iterator& operator++()
	{
		m_node = m_node->m_next;
		return *this;
	}

	bool operator==( Iterator other ) const { return m_node == other.m_node; }
	bool operator!=( Iterator other ) const { return m_node != other.m_node; }

};

template <class T>
class List
{
private:
	class NodeT : public Node
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

	public:
		IteratorT( Node* node ) : Iterator( node ){}
		U& operator*() const
		{
			return node()->m_data;
		}
		U* operator->() const
		{
			return &node()->m_data;
		}
		operator IteratorT<U const>() const
		{
			return m_node;
		}
	};

	Node			m_head;

public:
	using iterator = IteratorT<T>;
	using const_iterator = IteratorT<T const>;

	List()
	{
	}

	~List()
	{
		clear();
	}

	bool empty() const
	{
		return m_head.m_next == &m_head;
	}

	iterator begin()
	{
		return m_head.m_next;
	}

	iterator end()
	{
		return &m_head;
	}

	void push_back( T t )
	{
		m_head.push_back( new NodeT( t ) );
	}

	void erase( const_iterator i )
	{
		delete i.node();
	}

	void clear()
	{
		while( !empty() )
			erase( begin() );
	}
};

}
