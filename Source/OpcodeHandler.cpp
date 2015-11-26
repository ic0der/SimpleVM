// OpcodeHandler.cpp

#include "OpcodeHandler.hpp" 
#include "Context.hpp"
#include "StringUtils.hpp"
#include "Utils.hpp"

namespace svm
{
	namespace OpcodeHandler
	{
		#define DEF_HANDLER_FUNCTOR_(Name) bool F_##Name::operator()(Context &ctx, Opcode::ID opc, Value *opr)

		DEF_HANDLER_FUNCTOR_(System)
		{
			switch (opc)
			{
				case Opcode::ID::HRSRV:
					ctx.heap.Reserve(opr->pool.n_);
					break;
				case Opcode::ID::HALT:
					ctx.Halt();
					break;
				default: return false;
			}
			return true;
		}

		DEF_HANDLER_FUNCTOR_(Call)
		{
			switch (opc)
			{
				case Opcode::ID::NATIVE:
					ctx((Hash)opr->pool.n_, true);
					break;
				case Opcode::ID::CALL:
					ctx.Call(opr->pool.n_);
					break;
				case Opcode::ID::RET:
					ctx.Return();
					break;
				default: return false;
			}
			return true;
		}
		
		DEF_HANDLER_FUNCTOR_(Stack)
		{
			switch (opc)
			{
				case Opcode::ID::PUSH:
					ctx.stack.Push(new Value());
					ctx.stack.Top()->TransferFrom(opr);
					break;
				case Opcode::ID::POP:
					ctx.stack.Pop();
					break;
				default: return false;
			}
			return true;
		}
		
		DEF_HANDLER_FUNCTOR_(Heap)
		{
			switch (opc)
			{
				case Opcode::ID::STORE:
					{
						Value *v = ctx.stack.Pop();
						ctx.heap.Set(opr->pool.n_, v);
					}
					break;
				case Opcode::ID::LOAD:
					{
						Value *v = new Value();
						v->TransferFrom(ctx.heap.Get(opr->pool.n_));
						ctx.stack.Push(v);
					};
					break;
				case Opcode::ID::REL:
					ctx.heap.Release(opr->pool.n_);
					break;
				default: return false;
			}
			return true;
		}
		
		DEF_HANDLER_FUNCTOR_(Util)
		{
			switch (opc)
			{
				case Opcode::ID::TONUM:
					{
						Value *v = ctx.stack.Pop();
						if (v->type != SType::Number)
							if (v->type == SType::String)
								ctx.stack.Push(new Value(SType::Number, {.n_ = Util::Type::FromStr<Number>(StringUtils::ToCString(v->pool.s_))}));
						delete v;
					}
					break;
				case Opcode::ID::TOSTR:
					{
						Value *v = ctx.stack.Pop();
						if (v->type != SType::String)
							ctx.stack.Push(new Value(SType::String, {.s_ = StringUtils::ToString(Util::Type::ToStr(v->pool.n_))}));
						delete v;
					}
					break;
				default: return false;
			}
			return true;
		}
		
		DEF_HANDLER_FUNCTOR_(Jump)
		{
			switch (opc)
			{
				case Opcode::ID::JNZ:
					{
						Value *v = ctx.stack.Pop();
						if (v->type == SType::Number && v->pool.n_ != 0)
							ctx.Jump(opr->pool.n_);
						delete v;
					}
					break;
				case Opcode::ID::JZ:
					{
						Value *v = ctx.stack.Pop();
						if (v->type == SType::Number && v->pool.n_ == 0)
							ctx.Jump(opr->pool.n_);
						delete v;
					}
					break;
				case Opcode::ID::JMP:
					ctx.Jump(opr->pool.n_);
					break;
				default: return false;
			}
			return true;
		}
		
		DEF_HANDLER_FUNCTOR_(Math)
		{
			switch (opc)
			{
				case Opcode::ID::ADD:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();
						if (v1->type == SType::Number && v2->type == SType::Number)
							ctx.stack.Push(new Value(SType::Number, {.n_ = v1->pool.n_ + v2->pool.n_}));
						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::SUB:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();
						if (v1->type == SType::Number && v2->type == SType::Number)
							ctx.stack.Push(new Value(SType::Number, {.n_ = v1->pool.n_ - v2->pool.n_}));
						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::MUL:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();
						if (v1->type == SType::Number && v2->type == SType::Number)
							ctx.stack.Push(new Value(SType::Number, {.n_ = v1->pool.n_ * v2->pool.n_}));
						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::DIV:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();
						if (v1->type == SType::Number && v2->type == SType::Number)
							ctx.stack.Push(new Value(SType::Number, {.n_ = v1->pool.n_ / v2->pool.n_}));
						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::MOD:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();
						if (v1->type == SType::Number && v2->type == SType::Number)
							ctx.stack.Push(new Value(SType::Number, {.n_ = v1->pool.n_ % v2->pool.n_}));
						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::INC:
					{
						Value *v1 = ctx.stack.Pop();
						if (v1->type == SType::Number)
							ctx.stack.Push(new Value(SType::Number, {.n_ = v1->pool.n_ + 1}));
						delete v1;
					}
					break;
				case Opcode::ID::DEC:
					{
						Value *v1 = ctx.stack.Pop();
						if (v1->type == SType::Number)
							ctx.stack.Push(new Value(SType::Number, {.n_ = v1->pool.n_ - 1}));
						delete v1;
					}
					break;
					break;
				default: return false;
			}
			return true;
		}
		
		DEF_HANDLER_FUNCTOR_(Compare)
		{
			switch (opc)
			{
				case Opcode::ID::CLE:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();

						if (v1->type == SType::Number && v2->type == SType::Number)
						{
							Number r = v1->pool.n_ <= v2->pool.n_ ? 1 : 0;
							ctx.stack.Push(new Value(SType::Number, {.n_ = r}));
						}

						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::CLT:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();

						if (v1->type == SType::Number && v2->type == SType::Number)
						{
							Number r = v1->pool.n_ < v2->pool.n_ ? 1 : 0;
							ctx.stack.Push(new Value(SType::Number, {.n_ = r}));
						}

						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::CGE:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();

						if (v1->type == SType::Number && v2->type == SType::Number)
						{
							Number r = v1->pool.n_ >= v2->pool.n_ ? 1 : 0;
							ctx.stack.Push(new Value(SType::Number, {.n_ = r}));
						}

						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::CGT:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();

						if (v1->type == SType::Number && v2->type == SType::Number)
						{
							Number r = v1->pool.n_ > v2->pool.n_ ? 1 : 0;
							ctx.stack.Push(new Value(SType::Number, {.n_ = r}));
						}

						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::CEQ:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();

						if (v1->type == SType::Number && v2->type == SType::Number)
						{
							Number r = v1->pool.n_ == v2->pool.n_ ? 1 : 0;
							ctx.stack.Push(new Value(SType::Number, {.n_ = r}));
						}
						else if (v1->type == SType::String && v2->type == SType::String)
						{
							Number r = StringUtils::ToCString(v1->pool.s_) == StringUtils::ToCString(v2->pool.s_) ? 1 : 0;
							ctx.stack.Push(new Value(SType::Number, {.n_ = r}));
						}

						delete v1;
						delete v2;
					}
					break;
				case Opcode::ID::CNE:
					{
						Value *v2 = ctx.stack.Pop();
						Value *v1 = ctx.stack.Pop();

						if (v1->type == SType::Number && v2->type == SType::Number)
						{
							Number r = v1->pool.n_ != v2->pool.n_ ? 1 : 0;
							ctx.stack.Push(new Value(SType::Number, {.n_ = r}));
						}
						else if (v1->type == SType::String && v2->type == SType::String)
						{
							Number r = StringUtils::ToCString(v1->pool.s_) != StringUtils::ToCString(v2->pool.s_) ? 1 : 0;
							ctx.stack.Push(new Value(SType::Number, {.n_ = r}));
						}

						delete v1;
						delete v2;
					}
					break;
				default: return false;
			}
			return true;
		}
	}
}
