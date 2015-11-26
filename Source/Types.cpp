// Types.cpp

#include "Types.hpp"
#include "Utils.hpp"

namespace svm
{
	BasicClass::BasicClass(Length _typesize):
		m_uid(Util::GenUID(_typesize))
	{}

	bool operator==(BasicClass &lhv, BasicClass &rhv)
	{
		return lhv.m_uid == rhv.m_uid;
	}

	bool operator!=(BasicClass &lhv, BasicClass &rhv)
	{
		return lhv.m_uid != rhv.m_uid;
	}
}
