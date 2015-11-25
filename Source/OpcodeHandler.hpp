// OpcodeHandler.hpp

#if !defined(SVM_OPCODEHANDLER_H_) 
#define SVM_OPCODEHANDLER_H_

#include "OpcodeInfo.hpp"

namespace svm
{	
	class Context;
	namespace OpcodeHandler
	{
		#define DEC_HANDLER_FUNCTOR_(Name) class F_##Name { public: bool operator()(Context&, Opcode::ID, Value*); };

		DEC_HANDLER_FUNCTOR_(System)
		DEC_HANDLER_FUNCTOR_(Call)
		DEC_HANDLER_FUNCTOR_(Stack)
		DEC_HANDLER_FUNCTOR_(Heap)
		DEC_HANDLER_FUNCTOR_(Util)
		DEC_HANDLER_FUNCTOR_(Jump)
		DEC_HANDLER_FUNCTOR_(Math)
		DEC_HANDLER_FUNCTOR_(Compare)
	}
}

#endif
