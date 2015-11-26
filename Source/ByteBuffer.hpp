// ByteBuffer.hpp

#if !defined(SVM_BYTEBUFFER_H_)
#define SVM_BYTEBUFFER_H_

#include "Includes.hpp"
#include "Types.hpp"

namespace svm
{

	/* A byte buffer that represents an array of bytes */
	class ByteBuffer
	{
	private:
		Byte *m_pByte; /* Byte array */
		Length m_length; /* Byte array length */
		Byte m_silly;

	private:
		ByteBuffer();
		void Allocate(Length size);

	public:
		template <typename T, typename std::enable_if< std::is_arithmetic<T>::value, T >::type... >
		ByteBuffer(T v, bool isSize = false);

		ByteBuffer(const ByteBuffer &buffer);

		~ByteBuffer();

		/* Returns size of array */
		Length Size() const;

		/* Returns bytes as a byte vector */
		std::vector<Byte> GetBytes() const;

		/* Returns the pointer to byte buffer */
		const Byte* GetPtr() const;

		/* Copy value from a numeric type */
		template <typename T, typename std::enable_if< std::is_arithmetic<T>::value, T >::type... >
		void CopyFrom(T v);

		/* Copy bytes from a type */
		template <typename T>
		void CopyFrom(T *v, Length start, Length count);

		/* Copy bytes into a numeric type and return it */
		template <typename T, typename std::enable_if< std::is_arithmetic<T>::value, T >::type... >
		T GetAs() const;

		void SwapBytes(Byte align);

		/* Negative indexing is allowed (To items from the end of the array) */
		Byte& operator[](const SLength index);
		const Byte& operator[](const SLength index) const;

		friend std::ostream& operator<<(std::ostream &o, const ByteBuffer &buffer);
	};

	/* Template implementations */
	template <typename T, typename std::enable_if< std::is_arithmetic<T>::value, T >::type... >
	ByteBuffer::ByteBuffer(T v, bool isSize) :
		m_pByte(nullptr), m_length(0), m_silly(0)
	{
		if (isSize) Allocate(v);
		else
		{
			Allocate(sizeof(T));
			CopyFrom(v);
		}
	}

	template <typename T, typename std::enable_if< std::is_arithmetic<T>::value, T >::type... >
	void ByteBuffer::CopyFrom(T v)
	{
		if (m_length < sizeof(T))
			throw "Not enough space for ByteBuffer";
		std::memcpy(m_pByte, &v, sizeof(T));
	}

	template <typename T>
	void ByteBuffer::CopyFrom(T *v, Length start, Length count)
	{
		if (m_length < start + count)
			throw "Not enough space for ByteBuffer";
		std::memcpy(m_pByte + start, v, count);
	}

	template <typename T, typename std::enable_if< std::is_arithmetic<T>::value, T >::type... >
	T ByteBuffer::GetAs() const
	{
		T v;
		if (sizeof(T) > m_length)
			throw "Type size is bigger than buffer";
		std::memcpy(&v, m_pByte, sizeof(T));
		return v;
	}
}

#endif
 
