// Context.cpp

#include "Context.hpp" 
#include "Utils.hpp"
#include "OpcodeInfo.hpp"
#include "OpcodeHandler.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"
#include "SVM.hpp"
#include "Compiler/Compiler.hpp"

namespace svm
{
	Context::Context():
		loaded(false), running(false), ip(0)
	{}

	Context::~Context()
	{}

	void Context::RegisterNative(NativeFunction function, const std::string &name)
	{
		nativeregistery.insert(std::make_pair(Util::HashFunc(name), function));
	}
	
	void Context::LoadString(const std::string &program)
	{
		LoadChunk(program, false);
	}
	
	void Context::LoadFile(const std::string &filename)
	{
		LoadChunk(filename);
	}
	
	void Context::Run()
	{
		if (!loaded)
			throw GenericError("No loaded program");

		running = true;
		//bool dbg = false;
		while (running)
		{
			//if (dbg) std::cout << "EXEC\n";
			Byte opc = Fetch();
			//if (dbg) std::cout << "FETCH " << OpcodeInfo::Get(opc).name << "(" << (int)opc << ")" << std::endl;
			Value *v = nullptr;
			if (OpcodeInfo::Get(opc).hasoperand)
			{
				//if (dbg) std::cout << "  HAS OPR ";
				v = GetOperand();
				//if (dbg) std::cout << *v << std::endl;
			}
			//if (dbg) std::cout << "CALL\n";
			OpcodeInfo::Get(opc).handler(*this, (Opcode::ID)opc, v);
			//if (dbg) std::cout << "REL\n";
			if (v != nullptr)
				delete v;
			//if (dbg) std::cout << "END\n";
		}
	}
	
	void Context::Jump(BCOffset offset)
	{
		if (loaded && running)
			ip = offset;
	}
	
	void Context::Call(BCOffset offset)
	{
		if (loaded)
		{
			callstack.push(ip);
			ip = offset;
		}
	}

	void Context::Return()
	{
		if (loaded)
		{
			if (callstack.size() > 0)
			{
				ip = callstack.top();
				callstack.pop();
			}
			else
				running = false;
		}
	}
	
	void Context::Halt()
	{
		if (loaded && running)
			running = false;
	}
	
	Value& Context::operator[](Length index)
	{
		return *heap.Get(index);
	}

	void Context::operator()(const std::string &name, bool native)
	{
		this->operator()(Util::HashFunc(name), native);
	}

	void Context::operator()(Hash hash, bool native)
	{
		// First look in export table
		if (!native && chunk.exports.find(hash) != chunk.exports.end())
		{
			callstack.push(ip);
			ip = chunk.exports[hash];
		}
		// Look up the native registery
		else if (native && nativeregistery.find(hash) != nativeregistery.end())
			nativeregistery[hash](*this);
	}
	
	Byte Context::Fetch()
	{
		return *chunk.bytecode[chunk.instructionoffset + ip++];
	}
	
	Value* Context::GetOperand()
	{
		// Get type
		SType type = (SType)Fetch();

		// Do stuff
		switch (type)
		{
			case SType::Number:
				{
					Number n;
					std::memcpy(&n, chunk.bytecode[chunk.instructionoffset + ip], sizeof n);
					ip += sizeof n;
					return new Value(type, {.n_ = n});
				}
				break;
			case SType::String:
				{
					UInt len;
					std::memcpy(&len, chunk.bytecode[chunk.instructionoffset + ip], sizeof len);
					ip += sizeof len;

					Byte *buffer = new Byte[len + sizeof len]();
					std::memcpy(buffer + sizeof len, chunk.bytecode[chunk.instructionoffset + ip], len);
					ip += len;

					std::memcpy(buffer, &len, sizeof len);
					return new Value(type, {.s_ = buffer});
				}
				break;
			default: return new Value(SType::None, {.n_ = 0});
		}
	}
	
	void Context::LoadChunk(const std::string &value, bool isFile)
	{
		loaded = false;
		running = false;
		ip = 0;

		// Load from file
		try
		{
			if (isFile)
				chunk.bytecode.ReadFromFile(value);
			else
			{
				Compiler::Compiler c("Program", value);
				Bytecode &bc = c.DoCompile();
				chunk.bytecode.buffer = bc.buffer;
				chunk.bytecode.len = bc.len;
			}
		}
		catch (svm::BaseError &error) { std::cout << error; }
		catch (std::string &error) { std::cout << "[ERROR]" << error << std::endl; }

		BCLength ci = 0;

		// Check magic
		char *magic = new char[4]();
		std::memcpy(magic, chunk.bytecode[0], 4);
		if (std::memcmp(magic, "SVM.", 4) != 0)
		{
			delete[] magic;
			throw GenericError("This is not a valid bytecode");
		}
		ci += 4;

		// Get version
		chunk.ver_maj = *chunk.bytecode[ci];
		chunk.ver_min = *chunk.bytecode[ci + 1];
		ci += 2;
		if (chunk.ver_maj != version_major && chunk.ver_min != version_minor)
			throw GenericError("Incompatible bytecode");

		// Get exports
		UInt nexports = 0;
		std::memcpy(&nexports, chunk.bytecode[ci], sizeof(UInt));
		ci += sizeof(UInt);

		for (UInt i = 0; i < nexports; i++)
		{
			Hash hash = 0;
			BCOffset offset = 0;

			std::memcpy(&hash, chunk.bytecode[ci], sizeof hash);
			ci += sizeof hash;
			std::memcpy(&offset, chunk.bytecode[ci], sizeof offset);
			ci += sizeof offset;

			chunk.exports.insert(std::make_pair(hash, offset));
		}

		chunk.instructionoffset = ci;
		loaded = true;
	}
}
