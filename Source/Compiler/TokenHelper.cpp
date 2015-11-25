// TokenHelper.cpp

#include "TokenHelper.hpp" 

namespace svm
{
	namespace Compiler
	{
		std::string TokenHelper::TokenTypeName(TokenType type)
		{
			switch(type)
			{
				case TokenType::None: return "None";
				case TokenType::NotAssigned: return "NotAssigned";
				case TokenType::String: return "String";
				case TokenType::Number: return "Number";
				case TokenType::Opcode: return "Opcode";
				case TokenType::LabelName: return "LabelName";
				case TokenType::LabelDef: return "LabelDef";
				case TokenType::Name: return "Name";
				case TokenType::D_None: return "D_None";
				case TokenType::D_Export: return "D_Export";
			}
		}

		bool TokenHelper::IsOpcodeOrDirective(TokenType type)
		{
			return type == TokenType::Opcode ||
					type == TokenType::D_Export;
		}

		bool TokenHelper::IsOperand(TokenType type)
		{
			return type == TokenType::String ||
					type == TokenType::Number ||
					type == TokenType::LabelName ||
					type == TokenType::Name;
		}
	}
}
