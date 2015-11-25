// OpcodeInfo.hpp 

#if !defined(SVM_OPCODEINFO_H_)
#define SVM_OPCODEINFO_H_

#include "Includes.hpp"
#include "Types.hpp"
#include "Context.hpp"
#include "Value.hpp"

namespace svm
{
	class Opcode
	{
	public:
		enum class ID : Byte
		{
			NONE = 0,
			
			HRSRV,
			HALT,
			
			NATIVE,
			CALL,
			RET,

			PUSH,
			POP,

			STORE,
			LOAD,
			REL,

			TONUM,
			TOSTR,

			JNZ,
			JZ,
			JMP,

			ADD,
			SUB,
			MUL,
			DIV,
			MOD,
			INC,
			DEC,

			CLE,
			CLT,
			CGE,
			CGT,
			CEQ,
			CNE,

			__MAX
		};

		typedef std::function<bool(Context&, Opcode::ID, Value*)> OpcodeHandler;

	public:
		ID id;
		Byte idn;
		std::string name;
		bool hasoperand;
		OpcodeHandler handler;

	public:
		Opcode(ID id, std::string name, bool hasoperand, OpcodeHandler handler);
		friend bool operator==(const Opcode &lhv, const Opcode &rhv);
		friend bool operator!=(const Opcode &lhv, const Opcode &rhv);
	};

	class OpcodeInfo
	{
	private:
		static std::vector<std::unique_ptr<Opcode>> opcodes;

	public:
		static void Initialize();
		static UInt Count();
		static const Opcode& Get(Byte id);
		static const Opcode& Get(std::string name);
		static bool Exists(Byte id);
		static bool Exists(std::string name);
	};
}

#endif
