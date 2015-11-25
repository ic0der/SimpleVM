// Heap.cpp

#include "Heap.hpp"
#include "Logger.hpp"

namespace svm
{
	Heap::Heap():
		m_size(0), m_list(nullptr), m_empty(true)
	{}

	Heap::~Heap()
	{
		try
		{
			delete[] m_list;
		}
		catch (...) {}
	}
	
	void Heap::Clear()
	{
		for (Length i = 0; i < m_size; i++)
		{
			delete m_list[i];
			m_list[i] = nullptr;
		}
	}
	
	void Heap::Set(Length index, Value *v)
	{
		if (index >= m_size)
			Reserve(index - m_size + 1);
		m_list[index] = v;
	}
	
	Value* Heap::Get(Length index)
	{
		if (index >= m_size)
			Reserve(index - m_size + 1);
		return m_list[index];
	}
	
	Length Heap::Size()
	{
		return m_size;
	}
	
	void Heap::Reserve(Length n)
	{
		if (m_size == 0)
		{
			m_list = new Value*[n]();
			if (!m_list)
				throw GenericError("Allocation Failure");
			m_size = n;
		}
		else
		{
			Value **tmp = new Value*[m_size + n]();
			for (Length i = 0; i < m_size; i++)
				tmp[i] = m_list[i];
			delete[] m_list;
			m_list = tmp;
			m_size += n;
		}
	}
	
	void Heap::Release(Length index)
	{
		if (index >= m_size)
			return;
		delete m_list[index];
		m_list[index] = nullptr;
	}
}
