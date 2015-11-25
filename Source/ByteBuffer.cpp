// ByteBuffer.cpp

#include "ByteBuffer.hpp"

namespace svm
{
	ByteBuffer::ByteBuffer(const ByteBuffer &buffer)
	{
		ByteBuffer newbuffer(buffer.Size(), true);
		for (Length i = 0; i < newbuffer.Size(); i++)
			newbuffer[i] = buffer[i];
	}

	ByteBuffer::~ByteBuffer()
	{
		if (m_pByte) delete[] m_pByte;
	}

	void ByteBuffer::Allocate(Length size)
	{
		if (m_pByte)
			delete m_pByte;
		m_pByte = new Byte[size]();
		if (!m_pByte)
			throw "Allocation failure";
		m_length = size;
	}

	Length ByteBuffer::Size() const
	{
		return m_length;
	}

	std::vector<Byte> ByteBuffer::GetBytes() const
	{
		std::vector<Byte> b;
		for (Length i = 0; i < m_length; i++)
			b.push_back(m_pByte[i]);
		return b;
	}

	const Byte* ByteBuffer::GetPtr() const
	{
		return m_pByte;
	}

	void ByteBuffer::SwapBytes(Byte align)
	{
		Length bsz = Size();
		if (align > bsz) align = (Byte)bsz;
		for (Length i = 0; i < bsz;)
		{
			Byte fixedAlign = Byte(bsz - i) >= align ? align :
				Byte(bsz - i);
			for (Byte ai = 0; ai < fixedAlign / 2; ai++)
			{
				Length ifl = i + (fixedAlign - ai - 1);
				Byte tmp = m_pByte[i + ai];
				m_pByte[i + ai] = m_pByte[ifl];
				m_pByte[ifl] = tmp;
			}
			i += fixedAlign;
		}
	}

	Byte& ByteBuffer::operator[](const SLength index)
	{
		if (index < (SLength)m_length)
		{
			if (index >= 0)
				return m_pByte[index];
			else
				return m_pByte[m_length - -index];
		}
		return m_silly;
	}

	const Byte& ByteBuffer::operator[](const SLength index) const
	{
		if (index < (SLength)m_length)
		{
			if (index >= 0)
				return m_pByte[index];
			else
				return m_pByte[m_length - -index];
		}
		return m_silly;
	}

	std::ostream& operator<<(std::ostream &o, const ByteBuffer &buffer)
	{
		o << std::hex << "0x";
		for (Length i = 0; i < buffer.Size(); i++)
			o << (int)buffer[i];
		o << std::dec;
		return o;
	}

}
 
