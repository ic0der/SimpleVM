// StringUtils.cpp

#include "StringUtils.hpp"

namespace svm
{
	std::string& StringUtils::ToCString(String str)
	{
		Length len = *(UInt*)str;
		char *ptr = (char*)str + sizeof(UInt);
		return *(new std::string(ptr, len));
	}

	String StringUtils::ToString(const std::string &str)
	{
		Length len = str.length();
		Byte *ptr = new Byte[len + sizeof(UInt)]();
		std::memcpy(ptr, &len, sizeof(UInt)); /* Copy the size */
		std::memcpy(ptr + sizeof(UInt), str.c_str(), len); /* Copy the string */
		return (String)ptr;
	}

	Length StringUtils::StringLen(String str)
	{
		return (Length)*(UInt*)str;
	}

	String StringUtils::Duplicate(String str)
	{
		Length len = *(UInt*)str;
		Byte* s = new Byte[len + sizeof len]();
		std::memcpy(s, str, len + sizeof len);
		return s;
	}

	void StringUtils::Free(String *str)
	{
		delete[] *str;
		*str = nullptr;
	}
}
