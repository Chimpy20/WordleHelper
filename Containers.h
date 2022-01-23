#pragma once

#include "Debug.h"

namespace containers
{

template <class T>
class Element
{
public:
	Element()
	{
		m_next = nullptr;
	}
	~Element() {}
	T				m_data;
	Element*		m_next;
};

template <class T>
class List
{
private:
	Element<T>*		m_first;
public:
	List()
	{
		m_first = nullptr;
	}
	~List()
	{
		while( m_first != nullptr )
		{
			Element<T>* nextElement = m_first->m_next;
			delete m_first;
			m_first = nextElement;
		}
	}

	void Push( T item );
};

template <class T>
void List<T>::Push( T item )
{
	Element<T>* newElement = new Element<T>;
	ASSERT( newElement != nullptr, "Unable to create element for list\n" );
	newElement->m_data = item;
	newElement->m_next = m_first;
	m_first = newElement;
}

}
