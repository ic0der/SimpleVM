// Stack.cpp

#include "Stack.hpp"
#include "Logger.hpp"

namespace svm
{
	Stack::Stack():
		m_size(0), m_pos(0), m_list(nullptr), m_empty(true)
	{}

	Stack::~Stack()
	{
		try
		{
			delete[] m_list;
		}
		catch (...) {}
	}
	
	void Stack::Clear()
	{
		Length sz = Size();
		for (Length i = 0; i < sz; i++)
			Pop();
	}
	
	void Stack::Push(Value *value)
	{
		if (m_pos == m_size)
			Reserve(10);
		m_list[m_pos++] = value;
		m_empty = false;
	}
	
	Value* Stack::Pop(bool release)
	{
		if (m_empty)
			throw GenericError("Cannot pop empty stack");

		Value *v = m_list[--m_pos];
		
		if (m_pos == 0)
			m_empty = true;

		if (release)
		{
			delete v;
			v = nullptr;
		}

		return v;
	}
	
	Value* Stack::Top()
	{
		if (m_empty)
			return nullptr;
		return m_list[m_pos - 1];
	}

	Length Stack::Size()
	{
		return m_size;
	}

	void Stack::Reserve(Length size)
	{
		if (size == 0)
		{
			m_list = new Value*[size]();
			if (!m_list)
				throw GenericError("Allocation Failure");
			m_size = size;
		}
		else
		{
			Value **tmp = new Value*[m_size + size]();
			for (Length i = 0; i < m_size; i++)
				tmp[i] = m_list[i];
			delete[] m_list;
			m_list = tmp;
			m_size += size;
		}
	}

	void Stack::Reverse(Length n)
	{
		;
	}
}
