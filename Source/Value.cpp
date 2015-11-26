// Value.cpp

#include "Value.hpp" 
#include "StringUtils.hpp"

namespace svm
{
	Value::Value():
		type(SType::None), pool({.s_ = nullptr})
	{}

	Value::Value(SType type, TypePool pool):
		type(type), pool(pool)
	{}

	Value::~Value()
	{
		if (type == SType::String)
			StringUtils::Free(&pool.s_);
		pool.s_ = nullptr;
	}
	
	void Value::TransferTo(Value *destination)
	{
		destination->type = type;
		switch (type)
		{
			case SType::String:
				try
				{
					StringUtils::Free(&destination->pool.s_);
				}
				catch (...) {}
				destination->pool.s_ = StringUtils::Duplicate(pool.s_);
				break;
			case SType::Number:
				destination->pool.n_ = pool.n_;
				break;
			default: break;
		}
	}
	
	void Value::TransferFrom(Value *source)
	{
		type = source->type;
		switch (type)
		{
			case SType::String:
				try
				{
					StringUtils::Free(&pool.s_);
				}
				catch (...) {}
				pool.s_ = StringUtils::Duplicate(source->pool.s_);
				break;
			case SType::Number:
				pool.n_ = source->pool.n_;
				break;
			default: break;
		}
	}

	std::ostream& operator<<(std::ostream &o, const Value &v)
	{
		switch (v.type)
		{
			case SType::Number:
				o << "[" << v.pool.n_ << "](Number)";
				break;
			case SType::String:
				o << "[" << StringUtils::ToCString(v.pool.s_) << "](String)";
				break;
			default: break;
		}
		return o;
	}
}
