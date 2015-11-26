// DirectiveHelper.cpp

#include "DirectiveHelper.hpp"

namespace svm
{
	namespace Compiler
	{
		std::unordered_map<std::string, TokenType> DirectiveHelper::directives;
		char DirectiveHelper::DirectiveSign = '.';

		void DirectiveHelper::Initialize()
		{
			directives.insert(std::make_pair("none", TokenType::D_None));
			directives.insert(std::make_pair("export", TokenType::D_Export));
		}

		bool DirectiveHelper::IsDirectiveSign(char c)
		{
			return c == DirectiveSign;
		}

		bool DirectiveHelper::IsDirective(std::string value)
		{
			if (directives.find(value) != directives.end())
				return true;
			return false;
		}

		TokenType DirectiveHelper::GetDirectiveType(std::string value)
		{
			return directives[value];
		}
	}
}
