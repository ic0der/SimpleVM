// Compiler.hpp

#if !defined(SVM_COMPILER_H_) 
#define SVM_COMPILER_H_

#include "../Includes.hpp"
#include "../Bytecode.hpp"
#include "../Types.hpp"
#include "TU.hpp"
#include "TokenList.hpp"
#include "TU.hpp"

namespace svm
{
	namespace Compiler
	{
		class Compiler
		{
		private:
			std::string m_name;
			std::string m_program;
			svm::Bytecode m_bc;
			TokenList m_tl;
			TranslationUnit m_tu;

		private:
			void Phase1(); /* Whitespace / newline fix and clearing comments */
			void Phase2(); /* Tokenizing and identification */
			void Phase3(); /* Analyzing */
			void Phase4(); /* Translation */
			void Phase5(); /* Compilation */
			void Initialize();
			std::string FixStringEscapeChars(std::string str);
			TokenType GetPhase2TokenType(std::string tokenvalue);

		public:
			Compiler(std::string filepath);
			Compiler(std::string name, std::string program);
			svm::Bytecode& DoCompile();

		};
	}
	
	std::string& Decompile(const Bytecode &bc);
}

#endif
