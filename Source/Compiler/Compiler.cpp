// Compiler.cpp

#include "Compiler.hpp"
#include "../Utils.hpp"
#include "SyntaxHelper.hpp"
#include "DirectiveHelper.hpp"

namespace svm
{
	namespace Compiler
	{
		Compiler::Compiler(std::string filepath):
			m_name(filepath), m_program(Util::File::ReadFile(filepath))
		{
			Initialize();
		}

		Compiler::Compiler(std::string name, std::string program):
			m_name(name), m_program(program)
		{
			Initialize();
		}

		svm::Bytecode& Compiler::DoCompile()
		{
			Phase1();
			Phase2();
			Phase3();
			Phase4();
			Phase5();
			return m_bc;
		}

		void Compiler::Initialize()
		{
			SyntaxHelper::Initialize();
			DirectiveHelper::Initialize();
		}
	}
}
