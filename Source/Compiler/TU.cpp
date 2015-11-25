// TU.cpp

#include "TU.hpp" 

namespace svm
{
	namespace Compiler
	{
		TranslationUnit::Operand::Operand()
		{}
		
		TranslationUnit::Operand::Operand(SType type, TypePool p):
			type(type), pool(p)
		{}

		TranslationUnit::TranslationUnit():
			bytesize(0), headerbytesize(0)
		{}
	}
}
