// SVM.cpp

#include "SVM.hpp"
#include "OpcodeInfo.hpp"

namespace svm
{
	bool Initialize()
	{
		OpcodeInfo::Initialize();
		return true;
	}
}
