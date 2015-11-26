/* Utils.hpp */

#if !defined(SVM_UTILS_H_)
#define SVM_UTILS_H_

#include "Logger.hpp"
#include "Types.hpp"

namespace svm
{
	class Util
	{
	public:

		class Type
		{
		public:
			template <typename T, typename std::enable_if<std::is_arithmetic<T>::value, T>::type...>
			static bool IsOfType(const std::string &str);

			template <typename T, typename std::enable_if<std::is_arithmetic<T>::value, T>::type...>
			static T FromStr(const std::string &str);

			template <typename T>
			static std::string& ToStr(const T &t);
		};

		class File
		{
		public:
			static std::string& ReadFile(std::string filePath);
			static void WriteFile(std::string filePath, const std::string &content);
			/*static bool Exists(std::string filePath); */
		};

		class HRT
		{
		private:
			std::chrono::time_point<std::chrono::steady_clock> m_start;

		public:
			HRT();
			double Reset();
			double Elapsed();
		};

		static Hash HashFunc(std::string name);
		static UID GenUID(Length seed = 0);
	};


	/* Templates implementation */
	template <typename T, typename std::enable_if<std::is_arithmetic<T>::value, T>::type...>
	bool Util::Type::IsOfType(const std::string &str)
	{
		T value;
		std::istringstream istr(str);
		istr >> value;
		return (!istr.fail() && istr.eof());
	}

	template <typename T, typename std::enable_if<std::is_arithmetic<T>::value, T>::type...>
	T Util::Type::FromStr(const std::string &str)
	{
		T value;
		std::istringstream istr(str);
		istr >> value;
		if (!istr.fail() && istr.eof())
			return value;
		else throw GenericError("Cannot convert " + str + " to given type");
	}

	template <typename T>
	std::string& Util::Type::ToStr(const T &t)
	{
		std::ostringstream ostr;
		ostr << t;
		std::string *str = new std::string(ostr.str());
		return *str;
	}
}

#endif
 
