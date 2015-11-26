//TokenList.cpp

#include "TokenList.hpp"
#include "../Utils.hpp"
#include "TokenHelper.hpp"

namespace svm
{
	namespace Compiler
	{
		Token::Token(TokenType type, std::string value, Length line, Length col):
			type(type), value(value), linenum(line), column(col)
		{}

		
		TokenListNode::TokenListNode(Token *value):
			BasicClass(sizeof(*this)), value(value)
		{}

		TokenListNode::~TokenListNode()
		{
			try
			{
				delete value;
			}
			catch (...) {}
		}

		
		TokenList::TokenList():
			BasicClass(sizeof(*this)), m_count(0), head(nullptr), tail(nullptr)
		{}

		TokenList::~TokenList()
		{
			TokenListNode *cn = head;
			while (cn != nullptr)
			{
				TokenListNode *tmp = cn->next;
				delete cn;
				cn = tmp;
			}
		}

		void TokenList::AddAfter(TokenListNode *node, Token *value)
		{
			if (node->list == this)
			{
				node->next = MakeNode(value);
				node->next->prev = node;
				m_count++;
				if (tail == node)
					tail = node->next;
			}
		}

		void TokenList::AddBefore(TokenListNode *node, Token *value)
		{
			if (node->list == this)
			{
				node->prev = MakeNode(value);
				node->prev->next = node;
				m_count++;
				if (head == node)
					head = node->prev;
			}
		}

		void TokenList::AddLast(Token *value)
		{
			TokenListNode *node = MakeNode(value);
			m_count++;

			if (head == nullptr)
				head = tail = node;
			else
			{
				tail->next = node;
				node->prev = tail;
				tail = node;
			}
		}

		void TokenList::AddFirst(Token *value)
		{
			TokenListNode *node = MakeNode(value);
			m_count++;

			if (head == nullptr)
				head = tail = node;
			else
			{
				head->prev = node;
				node->next = head;
				head = node;
			}
		}

		void TokenList::Remove(TokenListNode *node)
		{
			if (node->list == this)
			{
				if (node->prev != nullptr)
					node->prev->next = node->next;
				if (node->next != nullptr)
					node->next->prev = node->prev;
				delete node;
			}
		}

		Length TokenList::Count()
		{
			return m_count;
		}

		TokenListNode* TokenList::MakeNode(Token *value)
		{
			TokenListNode *node = new TokenListNode(value);
			node->list = this;
			node->next = node->prev = nullptr;
			return node;
		}

		TokenListNode* TokenList::operator[](Length index)
		{
			if (index < m_count)
			{
				TokenListNode *cn = head;
				for (Length i = 0; i < index; i++)
					cn = cn->next;
				return cn;
			}
			return nullptr;
		}
		/*
		std::ostream& operator<<(std::ostream &o, const TokenList &tl)
		{
			o << "Tokens:" << std::endl;
			TokenListNode *cn = tl.head;
			while (cn != nullptr)
			{
				o << "  <" << cn->value->value << "> as " << TokenHelper::TokenTypeName(cn->value->type) << " at " << cn->value->linenum << ":" << cn->value->column << std::endl;
				cn = cn->next;
			}

			o << "Labels:" << std::endl;
			for (const auto &l : tl.labels)
				o << "  <" << std::get<0>(l) << "> at " << std::get<1>(l)->value->linenum << ":" << std::get<1>(l)->value->column << " jumped " << std::get<2>(l)->size() << " time(s)" << std::endl;

			o << "Exports:" << std::endl;
			for (std::string e : tl.exports)
				o << "  <" << e << ">" << std::endl;
			return o;
		}
		*/
	}
}
