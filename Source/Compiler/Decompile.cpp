// Decompile.cpp

#include "Compiler.hpp"
#include "../Utils.hpp"
#include "../OpcodeInfo.hpp"
#include "../StringUtils.hpp"

namespace svm
{
	std::string& Decompile(const Bytecode &bc)
	{
		std::stringstream ss;

		std::vector<std::tuple<std::string, Hash, BCOffset>> exports;

		Length ci = 0;

		// Check magic
		char *magic = new char[4]();
		std::memcpy(magic, bc.buffer, 4);
		if (std::strcmp(magic, "SVM.") != 0)
			return *new std::string();
		ci += 4;

		// Get version
		ss << "; SVM Version " << (int)bc.buffer[ci] << "." << (int)bc.buffer[ci + 1] << std::endl << std::endl;
		ci += 2;

		// Get exports
		UInt nexports = 0;
		std::memcpy(&nexports, bc.buffer + ci, sizeof(UInt));
		ci += sizeof(UInt);
		for (UInt i = 0; i < nexports; i++)
		{
			Hash hash = 0;
			BCOffset offset = 0;

			std::memcpy(&hash, bc.buffer + ci, sizeof hash);
			ci += sizeof hash;
			std::memcpy(&offset, bc.buffer + ci, sizeof offset);
			ci += sizeof offset;

			std::string exportname = "EL_"; exportname += Util::Type::ToStr(offset);
			exports.push_back(std::make_tuple(exportname, hash, offset));
			ss << ".export " << exportname << " ; Hash 0x" << std::hex << hash << std::dec << std::endl;
		}
		ss << std::endl;

		// Instructions
		Length base = ci;
		for (Length i = base; i < bc.len;)
		{
			Opcode opcode = OpcodeInfo::Get((Byte)bc.buffer[i++]);
			ss << "L_" << i - base - 1 << ": " << opcode.name;

			if (opcode.name == "jnz" || opcode.name == "jz" || opcode.name == "jmp" || opcode.name == "call")
			{
				if ((SType)bc.buffer[i++] == SType::Number)
				{
					Number n;
					std::memcpy(&n, bc.buffer + i, sizeof n);
					i += sizeof n;
					n -= base;
					ss << " L_" << (int)n;
				}
			}
			else if (opcode.hasoperand)
			{
				SType optype = (SType)bc.buffer[i++];
				switch (optype)
				{
					case SType::Number:
						{
							Number n;
							std::memcpy(&n, bc.buffer + i, sizeof n);
							i += sizeof n;
							ss << " " << (int)n;
						}
						break;
					case SType::String:
						{
							UInt len;
							std::memcpy(&len, bc.buffer + i, sizeof len);
							i += sizeof len;

							Byte *buffer = new Byte[len + sizeof len]();
							std::memcpy(buffer + sizeof len, bc.buffer + i, len);
							i += len;

							std::memcpy(buffer, &len, sizeof len);
							ss << " \'" << StringUtils::ToCString((String)buffer) << "\'";
						}
						break;
					default: break;
				}
			}
			ss << std::endl;
		}

		return *new std::string(ss.str());
	}
}
