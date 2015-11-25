// DirectiveHelper.hpp

#if !defined(SVM_DIRECTIVEHELPER_H_)
#define SVM_DIRECTIVEHELPER_H_

#include "../Includes.hpp"
#include "TokenList.hpp"

namespace svm
{
	namespace Compiler
	{
		class DirectiveHelper
		{
		private:
			static std::unordered_map<std::string, TokenType> directives;

		public:
			static char DirectiveSign;

		public:
			static void Initialize();
			static bool IsDirectiveSign(char c);
			static bool IsDirective(std::string value);
			static TokenType GetDirectiveType(std::string value);
		};
	}
}

#endif
