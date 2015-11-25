// TU.hpp

#if !defined(SVM_TU_H_)
#define SVM_TU_H_

#include "../Includes.hpp"
#include "../Types.hpp"

namespace svm
{
	namespace Compiler
	{
		class TranslationUnit
		{
		public:
			class Operand
			{
			public:
				SType type;
				TypePool pool;

			public:
				Operand();
				Operand(SType type, TypePool p);
			};

		public:
			BCLength bytesize; /* Total size of the instructions in bytes */
			BCLength headerbytesize; /* The size needed for header in bytes */
			std::unordered_map<std::string, BCOffset> labels; /* Address of the labels */
			std::vector<std::tuple<Byte, bool, Operand>> instructions;
			std::vector<std::pair<Hash, BCOffset>> exports;

		public:
			TranslationUnit();
		};
	}
}

#endif
