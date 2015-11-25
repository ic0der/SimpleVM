// OpcodeInfo.cpp

#include "OpcodeInfo.hpp" 
#include "OpcodeHandler.hpp"

namespace svm
{
	std::vector<std::unique_ptr<Opcode>> OpcodeInfo::opcodes;

	Opcode::Opcode(ID id, std::string name, bool hasoperand, OpcodeHandler handler):
		id(id), idn((Byte)id), name(name), hasoperand(hasoperand), handler(handler)
	{}

	bool operator==(const Opcode &lhv, const Opcode &rhv)
	{
		return lhv.id == rhv.id;
	}

	bool operator!=(const Opcode &lhv, const Opcode &rhv)
	{
		return !(lhv == rhv);
	}


	void OpcodeInfo::Initialize()
	{
		opcodes.reserve((Byte)Opcode::ID::__MAX);

		opcodes.emplace_back(new Opcode(Opcode::ID::NONE, 		"none", 	false, 	nullptr));

		opcodes.emplace_back(new Opcode(Opcode::ID::HRSRV, 		"hrsrv", 	true, 	OpcodeHandler::F_System()));
		opcodes.emplace_back(new Opcode(Opcode::ID::HALT, 		"halt", 	false, 	OpcodeHandler::F_System()));
		
		opcodes.emplace_back(new Opcode(Opcode::ID::NATIVE, 	"native", 	true, 	OpcodeHandler::F_Call()));
		opcodes.emplace_back(new Opcode(Opcode::ID::CALL, 		"call", 	true, 	OpcodeHandler::F_Call()));
		opcodes.emplace_back(new Opcode(Opcode::ID::RET, 		"ret", 		false, 	OpcodeHandler::F_Call()));

		opcodes.emplace_back(new Opcode(Opcode::ID::PUSH, 		"push", 	true, 	OpcodeHandler::F_Stack()));
		opcodes.emplace_back(new Opcode(Opcode::ID::POP, 		"pop", 		false, 	OpcodeHandler::F_Stack()));

		opcodes.emplace_back(new Opcode(Opcode::ID::STORE, 		"store", 	true, 	OpcodeHandler::F_Heap()));
		opcodes.emplace_back(new Opcode(Opcode::ID::LOAD, 		"load", 	true, 	OpcodeHandler::F_Heap()));
		opcodes.emplace_back(new Opcode(Opcode::ID::LOAD, 		"rel",	 	true, 	OpcodeHandler::F_Heap()));
		
		opcodes.emplace_back(new Opcode(Opcode::ID::TONUM, 		"tonum", 	false, 	OpcodeHandler::F_Util()));
		opcodes.emplace_back(new Opcode(Opcode::ID::TOSTR, 		"tostr", 	false, 	OpcodeHandler::F_Util()));

		opcodes.emplace_back(new Opcode(Opcode::ID::JNZ, 		"jnz", 		true, 	OpcodeHandler::F_Jump()));
		opcodes.emplace_back(new Opcode(Opcode::ID::JZ, 		"jz", 		true, 	OpcodeHandler::F_Jump()));
		opcodes.emplace_back(new Opcode(Opcode::ID::JMP, 		"jmp", 		true, 	OpcodeHandler::F_Jump()));

		opcodes.emplace_back(new Opcode(Opcode::ID::ADD, 		"add", 		false, 	OpcodeHandler::F_Math()));
		opcodes.emplace_back(new Opcode(Opcode::ID::SUB, 		"sub", 		false, 	OpcodeHandler::F_Math()));
		opcodes.emplace_back(new Opcode(Opcode::ID::MUL, 		"mul", 		false, 	OpcodeHandler::F_Math()));
		opcodes.emplace_back(new Opcode(Opcode::ID::DIV, 		"div", 		false, 	OpcodeHandler::F_Math()));
		opcodes.emplace_back(new Opcode(Opcode::ID::MOD, 		"mod", 		false, 	OpcodeHandler::F_Math()));
		opcodes.emplace_back(new Opcode(Opcode::ID::INC, 		"inc", 		false, 	OpcodeHandler::F_Math()));
		opcodes.emplace_back(new Opcode(Opcode::ID::DEC, 		"dec", 		false, 	OpcodeHandler::F_Math()));

		opcodes.emplace_back(new Opcode(Opcode::ID::CLE, 		"cle", 		false, 	OpcodeHandler::F_Compare()));
		opcodes.emplace_back(new Opcode(Opcode::ID::CLT, 		"clt", 		false, 	OpcodeHandler::F_Compare()));
		opcodes.emplace_back(new Opcode(Opcode::ID::CGE, 		"cge", 		false, 	OpcodeHandler::F_Compare()));
		opcodes.emplace_back(new Opcode(Opcode::ID::CGT, 		"cgt", 		false, 	OpcodeHandler::F_Compare()));
		opcodes.emplace_back(new Opcode(Opcode::ID::CEQ, 		"ceq", 		false, 	OpcodeHandler::F_Compare()));
		opcodes.emplace_back(new Opcode(Opcode::ID::CNE, 		"cne", 		false, 	OpcodeHandler::F_Compare()));
	}
	
	UInt OpcodeInfo::Count()
	{
		return opcodes.size();
	}
	
	const Opcode& OpcodeInfo::Get(Byte id)
	{
		return *opcodes[id];
	}
	
	const Opcode& OpcodeInfo::Get(std::string name)
	{
		for (auto &o : opcodes)
			if (o->name == name)
				return *o;
		return *opcodes[0];
	}
	
	bool OpcodeInfo::Exists(Byte id)
	{
		return id < (Byte)Opcode::ID::__MAX;
	}
	
	bool OpcodeInfo::Exists(std::string name)
	{
		for (auto &o : opcodes)
			if (o->name == name)
				return true;
		return false;
	}	
}
