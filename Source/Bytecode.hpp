// Bytecode.hpp

#if !defined(SVM_BYTECODE_H_)
#define SVM_BYTECODE_H_

#include "Includes.hpp"
#include "Types.hpp"

namespace svm
{

	class Bytecode
	{
	public:
		Byte *buffer;
		Length len;

	public:
		Bytecode();
		Bytecode(const std::string &path);
		~Bytecode();

		void WriteToFile(std::string filename);
		void ReadFromFile(std::string filename);
		
		Byte* operator[](Length index);
	};

}

#endif
 
