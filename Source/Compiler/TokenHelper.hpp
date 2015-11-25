// TokenHelper.hpp

#if !defined(SVM_TOKENHELPER_H_) 
#define SVM_TOKENHELPER_H_

#include "TokenList.hpp"

namespace svm
{
	namespace Compiler
	{
		class TokenHelper
		{
		public:
			static std::string TokenTypeName(TokenType type);
			static bool IsOpcodeOrDirective(TokenType type);
			static bool IsOperand(TokenType type);
		};
	}
}

#endif
