// Heap.hpp

#if !defined(SVM_HEAP_S_)
#define SVM_HEAP_S_

#include "Value.hpp"

namespace svm
{
	class Heap
	{
	private:
		Length m_size;
		Value **m_list;
		bool m_empty;

	private:
		void Expand();

	public:
		Heap();
		~Heap();
		void Clear();
		void Set(Length index, Value *v);
		Value* Get(Length index);
		Length Size();
		void Reserve(Length n);
		void Release(Length index);

	};
}

#endif
