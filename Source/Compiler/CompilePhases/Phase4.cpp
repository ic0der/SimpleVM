// Phase4.cpp

#include "../Compiler.hpp" 
#include "../../OpcodeInfo.hpp"
#include "../../Utils.hpp"
#include "../../StringUtils.hpp"

namespace svm
{
	namespace Compiler
	{
		void Compiler::Phase4()
		{
			/* First get the label positions */
			TokenListNode *cn = m_tl.head;
			while (cn != nullptr)
			{
				if (cn->value->type == TokenType::Opcode)
				{
					m_tu.bytesize++;
					if (OpcodeInfo::Get(cn->value->value).hasoperand)
					{
						// 1 byte for type
						m_tu.bytesize++;
						cn = cn->next;
						switch (cn->value->type)
						{
							case TokenType::Name:
								// hash it
								m_tu.bytesize += sizeof(UInt);
								cn->value->value = Util::Type::ToStr(Util::HashFunc(cn->value->value));
								cn->value->type = TokenType::Number;
								break;
							case TokenType::String:
								m_tu.bytesize += sizeof(UInt); // for length
								m_tu.bytesize += cn->value->value.length();
								break;
							case TokenType::LabelName:
								// its just an address
							case TokenType::Number:
								m_tu.bytesize += sizeof(UInt);
								break;
							default: break;
						}
					}
				}
				else if (cn->value->type == TokenType::LabelDef)
					m_tu.labels[cn->value->value] = m_tu.bytesize;

				cn = cn->next;
			}

			/* Now replace label names with their positions */
			for (auto &l : m_tl.labels)
			{
				std::string labelname = std::get<0>(l);
				for (TokenListNode *tn : *std::get<2>(l))
				{
					tn->value->value = Util::Type::ToStr(m_tu.labels[labelname]);
					tn->value->type = TokenType::Number;
				}
				// Remove the label definition
				m_tl.Remove(std::get<1>(l));
			}

			/* Now get the instructions easily :) */
			cn = m_tl.head;
			while (cn != nullptr)
			{
				if (OpcodeInfo::Get(cn->value->value).hasoperand)
				{
					TypePool pool;
					SType type = SType::None;
					switch (cn->next->value->type)
					{
						case TokenType::Number:
							pool.n_ = Util::Type::FromStr<Number>(cn->next->value->value);
							type = SType::Number;
							break;
						case TokenType::String:
							pool.s_ = StringUtils::ToString(cn->next->value->value);
							type = SType::String;
							break;
						default:
							break;
					}
					m_tu.instructions.push_back(std::make_tuple(OpcodeInfo::Get(cn->value->value).idn, true, TranslationUnit::Operand(type, pool)));
					cn = cn->next;
				}
				else
					m_tu.instructions.push_back(std::make_tuple(OpcodeInfo::Get(cn->value->value).idn, false, TranslationUnit::Operand()));
				cn = cn->next;
			}

			/* Hash exports. I do this here because in Phase3 duplicate exports and labels will be checked so I cannot check a hash with label */
			for (std::string &e : m_tl.exports)
				m_tu.exports.push_back(std::make_pair(Util::HashFunc(e), m_tu.labels[e]));

			/* Calculate headerbytesize */
			m_tu.headerbytesize += 4; // Magic: SVM.
			m_tu.headerbytesize += sizeof(Byte) * 2; // Version: major minor
			m_tu.headerbytesize += sizeof(UInt); // Number of exports
			m_tu.headerbytesize += m_tu.exports.size() * (sizeof(Hash) + sizeof(BCOffset));
		}
	}
}
 
