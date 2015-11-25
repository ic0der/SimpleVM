// Types.hpp

#if !defined(SVM_TYPES_H_)
#define SVM_TYPES_H_

#include "Includes.hpp"

namespace svm
{

	/* SVM Types */
	typedef std::uint8_t Byte;
	typedef std::int8_t Char;
	typedef std::uint8_t UChar;
	typedef std::int16_t Short;
	typedef std::uint16_t UShort;
	typedef std::int32_t Int;
	typedef std::uint32_t UInt;

	typedef std::size_t Length;
	typedef long long SLength;
	typedef std::uint32_t Hash;
	typedef std::uint32_t UID;
	typedef std::uint32_t BCOffset;
	typedef std::uint32_t BCLength;

	/* VM Types */
	typedef svm::Int Number;
	typedef std::uint8_t* String;
	
	union TypePool
	{
		Number n_;
		String s_;
	};

	/* VM Types */
	enum class SType : Byte
	{
		None,			/* No type */
		Null,			/* Null type. Empty */
		Number,		/* 4 bytes signed integer */
		String,			/* At least 4 bytes string */
	};

	class BasicClass
	{
	protected:
		UID m_uid;

	private:
		BasicClass();

	public:
		BasicClass(Length _typesize);
		
		friend bool operator==(BasicClass &lhv, BasicClass &rhv);
		friend bool operator!=(BasicClass &lhv, BasicClass &rhv);

	};
}

#endif
 
