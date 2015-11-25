// Value.hpp

#if !defined(SVM_VALUE_H_) 
#define SVM_VALUE_H_

#include "Includes.hpp"
#include "Types.hpp"

namespace svm
{
	class Value
	{
	public:
		SType type;
		TypePool pool;

	public:
		Value();
		Value(SType type, TypePool pool);
		~Value();

		void TransferTo(Value *destination);
		void TransferFrom(Value *source);

		friend std::ostream& operator<<(std::ostream &o, const Value &v);
	};
}

#endif
