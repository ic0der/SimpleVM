// Context.hpp

#if !defined(SVM_CONTEXT_H_) 
#define SVM_CONTEXT_H_

#include "Types.hpp"
#include "Bytecode.hpp"
#include "Stack.hpp"
#include "Heap.hpp"

namespace svm
{
	class Opcode;
	class Context;
	
	typedef std::function<void(Context&)> NativeFunction;

	class Context
	{
	private:
		bool loaded;
		bool running;
		BCOffset ip;
		struct
		{
			Byte ver_maj;
			Byte ver_min;
			BCOffset instructionoffset;
			std::unordered_map<Hash, BCOffset> exports;
			Bytecode bytecode;
		} chunk;
		std::stack<BCOffset> callstack;

	public:
		Stack stack;
		Heap heap;
		std::unordered_map<Hash, NativeFunction> nativeregistery;
		
	private:
		Byte Fetch();
		Value* GetOperand();
		void LoadChunk(const std::string &value, bool isFile = true);

	public:
		Context();
		~Context();

		void RegisterNative(NativeFunction function, const std::string &name);
		void LoadString(const std::string &program);
		void LoadFile(const std::string &filename);
		void Run();

		void Jump(BCOffset offset);
		void Call(BCOffset offset);
		void Return();
		void Halt();

		// Call an exported label/native function
		void operator()(const std::string &name, bool native = false);
		void operator()(Hash hash, bool native);
		// Set/Get heap
		Value& operator[](Length index);
	};
}

#endif
