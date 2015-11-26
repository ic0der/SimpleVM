// Phase2.cpp

#include "../../Utils.hpp"
#include "../Compiler.hpp" 
#include "../SyntaxHelper.hpp"
#include "../DirectiveHelper.hpp"
#include "../../Logger.hpp"
#include "../../OpcodeInfo.hpp"

namespace svm
{
	namespace Compiler
	{
		void Compiler::Phase2()
		{
			// Tokenize
			Length sidx = 0, slen = 0;
			Length line = 1, column = 0, scolumn = 0, sline = 0;
			bool iscapturing = false, instring = false;

			for (Length pi = 0; pi < m_program.length(); pi++)
			{
				char c = m_program[pi];

				if (c == '\n')
				{
					line++;
					column = 0;
				}
				else
					column++;

				if (!iscapturing && (isalnum(c) || c == '_' || c =='\'' || c == '.'))
				{
					iscapturing = true;
					sidx = pi;
					scolumn = column;
					sline = line;

					if (c == '\'')
					{
						instring = true;
						sidx++;
						scolumn++;
					}
					continue;
				}

				if (instring && iscapturing && c == '\'' && (pi - 1 > 0 && m_program[pi - 1] != '\\'))
				{
					iscapturing = false;
					instring = false;
					slen = pi - sidx;
					std::string fixedstring = FixStringEscapeChars(m_program.substr(sidx, slen));
					m_tl.AddLast(new Token(TokenType::String, fixedstring, sline, scolumn - 1));
					continue;
				}

				if (!instring && iscapturing && isspace(c))
				{
					iscapturing = false;
					slen = pi - sidx;
					std::string value = m_program.substr(sidx, slen);
					m_tl.AddLast(new Token(GetPhase2TokenType(value), value, sline, scolumn));

					switch (m_tl.tail->value->type)
					{
						case TokenType::LabelDef:
							value = SyntaxHelper::GetLabelDefName(value);
							m_tl.tail->value->value = value; /* Shit, too many values! */
							m_tl.labels.push_back(std::make_tuple(value, m_tl.tail, std::make_unique<std::vector<TokenListNode*>>())); // C++14
							//m_tl.labels.push_back(std::make_tuple(SyntaxHelper::GetLabelDefName(value), m_tl.tail, std::unique_ptr<std::vector<TokenListNode*>>(new std::vector<TokenListNode*>()))); // C++11
							break;
						default: break;
					}
				}
			}

			// Identify
			TokenListNode *cn = m_tl.head;
			while (cn != nullptr)
			{
				switch (cn->value->type)
				{
					case TokenType::NotAssigned:
						{
							bool found = false;
							for (auto &l : m_tl.labels)
								if (cn->value->value == std::get<0>(l))
								{
									found = true;
									cn->value->type = TokenType::LabelName;
									std::get<2>(l)->push_back(cn);
								}
							if (!found)
								cn->value->type = TokenType::Name;
						}
						break;
					case TokenType::D_Export:
						if (cn->next->value->type == TokenType::NotAssigned)
						{
							cn = cn->next;
							/* Check if it was added before */
							bool found = false;
							for (std::string &str : m_tl.exports)
								if (str == cn->value->value)
									found = true;
							if (!found)
								m_tl.exports.push_back(cn->value->value);

							/* Remove export directive */
							m_tl.Remove(cn->prev);

							/* Remove export name */
							if (cn->prev == nullptr)
							{
								cn = cn->next;
								m_tl.Remove(cn->prev);
								continue;
							}
							else
							{
								cn = cn->prev;
								m_tl.Remove(cn->next);
							}
						}
						break;
					default: break;
				}
				if (cn != nullptr)
					cn = cn->next;
			}
		}

		TokenType Compiler::GetPhase2TokenType(std::string tokenvalue)
		{
			/* Number */
			if (isdigit(tokenvalue[0]) || tokenvalue[0] == '-' || tokenvalue[0] == '+')
			{
				if (Util::Type::IsOfType<Int>(tokenvalue))
					return TokenType::Number;
				else
					throw GenericError("Invalid number " + tokenvalue);
			}
			/* Directive */
			else if (DirectiveHelper::IsDirectiveSign(tokenvalue[0]))
			{
				if (DirectiveHelper::IsDirective(tokenvalue.substr(1)))
					return DirectiveHelper::GetDirectiveType(tokenvalue.substr(1));
				else
					throw GenericError("Invalid directive " + tokenvalue);
			}
			/* Label Definition */
			else if (SyntaxHelper::IsLabelDef(tokenvalue))
				return TokenType::LabelDef;
			/* Check for opcode */
			else if (OpcodeInfo::Exists(tokenvalue))
				return TokenType::Opcode;

			return TokenType::NotAssigned;
		}

		std::string Compiler::FixStringEscapeChars(std::string str)
		{
			std::string newstr;
			for (Length i = 0; i < str.length(); i++)
			{
				if (str[i] == '\\' && SyntaxHelper::IsEscapeCharacter(str[i + 1]))
				{
					i++;
					switch (SyntaxHelper::GetEscapeCharType(str[i]))
					{
						case SyntaxHelper::EscapeCharType::Backslash:
							newstr += '\\';
							break;
						case SyntaxHelper::EscapeCharType::Backspace:
							newstr += '\b';
							break;
						case SyntaxHelper::EscapeCharType::Formfeed:
							newstr += '\f';
							break;
						case SyntaxHelper::EscapeCharType::Newline:
							newstr += '\n';
							break;
						case SyntaxHelper::EscapeCharType::Null:
							newstr += '\0';
							break;
						case SyntaxHelper::EscapeCharType::Return:
							newstr += '\r';
							break;
						case SyntaxHelper::EscapeCharType::SingleQoute:
							newstr += '\'';
							break;
						case SyntaxHelper::EscapeCharType::Tab:
							newstr += '\t';
							break;
						case SyntaxHelper::EscapeCharType::Vtab:
							newstr += '\v';
							break;
						default:
							throw GenericError("Some weird character in escape encoder has been found .-.");
					}
				}
				else
					newstr += str[i];
			}
			return newstr;
		}
	}
}
 
