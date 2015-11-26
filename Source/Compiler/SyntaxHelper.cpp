// SyntaxHelper.cpp

#include "SyntaxHelper.hpp" 

namespace svm
{
	namespace Compiler
	{
		std::unordered_map<char, SyntaxHelper::EscapeCharType> SyntaxHelper::escapechars;

		void SyntaxHelper::Initialize()
		{
			escapechars.insert({'\0', EscapeCharType::None});
			escapechars.insert({'\r', EscapeCharType::Return});
			escapechars.insert({'\'', EscapeCharType::SingleQoute});
			escapechars.insert({'\\', EscapeCharType::Backslash});
			escapechars.insert({'n', EscapeCharType::Newline});
			escapechars.insert({'t', EscapeCharType::Tab});
			escapechars.insert({'b', EscapeCharType::Backspace});
			escapechars.insert({'v', EscapeCharType::Vtab});
			escapechars.insert({'f', EscapeCharType::Formfeed});
			escapechars.insert({'0', EscapeCharType::Null});
		}
		
		bool SyntaxHelper::IsCommentChar(char c)
		{
			return c == CommentChar;
		}
		
		bool SyntaxHelper::IsStringLiteral(char c)
		{
			return c == StringLiteral;
		}
		
		bool SyntaxHelper::IsEscapeCharacter(char c)
		{
			return escapechars.find(c) != escapechars.end();
		}

		SyntaxHelper::EscapeCharType SyntaxHelper::GetEscapeCharType(char c)
		{
			return escapechars[c];
		}

		bool SyntaxHelper::IsLabelDef(std::string value)
		{
			return value[value.length() - 1] == ':';
		}

		std::string SyntaxHelper::GetLabelDefName(std::string value)
		{
			std::string str;
			if (IsLabelDef(value))
				str = value.substr(0, value.length() - 1);
			return str;
		}
	}
}
