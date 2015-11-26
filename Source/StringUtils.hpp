// StringUtils.hpp

#if !defined(SVM_STRINGUTILS_H_)
#define SVM_STRINGUTILS_H_

#include "Includes.hpp"
#include "Types.hpp"

namespace svm
{
	class StringUtils
	{
	public:
		static std::string& ToCString(String str);
		static String ToString(const std::string &str);
		static Length StringLen(String str);
		static String Duplicate(String str);
		static void Free(String *str);
	};
}

#endif
