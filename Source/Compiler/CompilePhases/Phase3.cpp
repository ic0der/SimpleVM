// Phase3.cpp

#include "../Compiler.hpp"
#include "../../Logger.hpp"
#include "../TokenHelper.hpp"
#include "../../OpcodeInfo.hpp"

namespace svm
{
	namespace Compiler
	{
		void Compiler::Phase3()
		{
			TokenListNode *cn = m_tl.head;
			while (cn != nullptr)
			{
				if (TokenHelper::IsOpcodeOrDirective(cn->value->type))
				{
					switch (cn->value->type)
					{
						case TokenType::Opcode:
							if (OpcodeInfo::Get(cn->value->value).hasoperand)
							{
								if (cn->next != nullptr && !TokenHelper::IsOperand(cn->next->value->type))
									throw TokenError(cn->next->value, m_name, "Expected valid operand");

								if (cn->next != nullptr)
									cn = cn->next;
							}
							break;
						default:
							throw TokenError(cn->value, m_name, "Invalid token type");
					}
				}
				else if (cn->value->type == TokenType::LabelDef) ; // just ignore it
				else
					throw TokenError(cn->value, m_name, "Invalid token type");

				if (cn != nullptr)
					cn = cn->next;
			}

			/* Check for invalid exports */
			for (std::string &exported : m_tl.exports)
			{
				bool found = false;
				for (const auto &l : m_tl.labels)
					if (std::get<0>(l) == exported)
						found = true;
				if (!found)
					throw GenericError("Undefined label " + exported);
			}

			/* Check for duplicate exports */
			int d_count = 0;
			for (std::string &e : m_tl.exports)
			{
				for (std::string &e2 : m_tl.exports)
					if (e2 == e && ++d_count > 1)
						throw GenericError("Duplicate export " + e2);
				d_count = 0;
			}

			/* Check for duplicate labels */
			d_count = 0;
			for (const auto &l : m_tl.labels)
			{
				for (const auto &l2 : m_tl.labels)
					if (std::get<0>(l) == std::get<0>(l2) && ++d_count > 1)
						throw TokenError(std::get<1>(l2)->value, m_name, "Duplicate label " + std::get<0>(l));
				d_count = 0;
			}
		}
	}
}
