// Phase5.cpp

#include "../Compiler.hpp" 
#include "../../ByteBuffer.hpp"
#include "../../SVM.hpp"
#include "../../StringUtils.hpp"
#include "../../OpcodeInfo.hpp"

namespace svm
{
	namespace Compiler
	{
		void Compiler::Phase5()
		{
			BCLength ci = 0;
			ByteBuffer bf(m_tu.headerbytesize + m_tu.bytesize, true);

			// Magic
			Byte magic[] = "SVM.";
			bf.CopyFrom(magic, ci, 4);
			ci += 4;

			// Version
			bf.CopyFrom(&svm::version_major, ci++, 1);
			bf.CopyFrom(&svm::version_minor, ci++, 1);

			// Exports
			UInt nexports = m_tu.exports.size();
			bf.CopyFrom(&nexports, ci, sizeof(UInt));
			ci += sizeof(UInt);
			for (const auto &e : m_tu.exports)
			{
				bf.CopyFrom(&e.first, ci, sizeof(Hash));
				ci += sizeof(Hash);
				bf.CopyFrom(&e.second, ci, sizeof(BCOffset));
				ci += sizeof(BCOffset);
			}

			// Instructions
			for (const auto &i : m_tu.instructions)
			{
				Byte opcode = std::get<0>(i);
				bf[ci++] = opcode;
				if (std::get<1>(i)) // has operand
				{
					auto &operand = std::get<2>(i);
					switch (operand.type)
					{
						case SType::Number:
							bf[ci++] = (Byte)operand.type;
							bf.CopyFrom(&operand.pool.n_, ci, sizeof(Number));
							ci += sizeof(Number);
							break;
						case SType::String:
							{
								bf[ci++] = (Byte)operand.type;
								UInt len = StringUtils::StringLen(operand.pool.s_);
								bf.CopyFrom(&len, ci, sizeof len);
								ci += sizeof len;
								bf.CopyFrom(operand.pool.s_ + sizeof len, ci, len);
								ci += len;
							}
							break;
						default:
							break;
					}
				}
			}

			// Copy to bytecode
			m_bc.buffer = new Byte[ci]();
			m_bc.len = ci;
			std::memcpy(m_bc.buffer, bf.GetPtr(), ci);
		}
	}
}
 
