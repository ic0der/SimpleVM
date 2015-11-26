// Stack.hpp

#if !defined(SVM_STACK_H_)
#define SVM_STACK_H_

#include "Includes.hpp"
#include "Types.hpp"
#include "Value.hpp"

namespace svm
{
	class Stack
	{
	private:
		Length m_size;
		Length m_pos;
		Value **m_list;
		bool m_empty;

	public:
		Stack();
		~Stack();
		void Clear();
		void Push(Value *value);
		Value* Pop(bool release = false);
		Value* Top();
		Length Size();
		void Reserve(Length size);
		void Reverse(Length n);
	};
}

#endif
