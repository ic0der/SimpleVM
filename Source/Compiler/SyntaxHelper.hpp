// SyntaxHelper.hpp

#if !defined(SVM_SYNTAXHELPER_H_) 
#define SVM_SYNTAXHELPER_H_

#include "../Includes.hpp"

namespace svm
{
	namespace Compiler
	{
		class SyntaxHelper
		{
		public:
			enum class EscapeCharType
			{
				None,
				SingleQoute,
				Backslash,
				Newline,
				Return,
				Tab,
				Backspace,
				Formfeed,
				Vtab,
				Null
			};

		private:
			static std::unordered_map<char, EscapeCharType> escapechars;

		public:
			static constexpr char StringLiteral = '\'';
			static constexpr char CommentChar = ';';

		public:
			static void Initialize();
			static bool IsCommentChar(char c);
			static bool IsStringLiteral(char c);
			static bool IsEscapeCharacter(char c);
			static SyntaxHelper::EscapeCharType GetEscapeCharType(char c);
			static bool IsLabelDef(std::string value);
			static std::string GetLabelDefName(std::string value);
		};
	}
}

#endif
