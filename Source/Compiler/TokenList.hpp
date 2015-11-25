// TokenList.hpp

#if !defined(SVM_TOKENLIST_H_) 
#define SVM_TOKENLIST_H_

#include "../Includes.hpp"
#include "../Types.hpp"

namespace svm
{
	namespace Compiler
	{
		enum class TokenType : Byte
		{
			None,
			NotAssigned, 	// P2
			String,			// P2
			Number,			// P2
			Opcode,			// P2
			LabelDef,		// P2
			LabelName,		//  P3
			Name,			//  P3
			D_None,			// P2
			D_Export,		// P2
		};

		class Token
		{
		public:
			TokenType type;
			std::string value;
			svm::Length linenum;
			svm::Length column;

		public:
			Token(TokenType type, std::string value, Length line, Length col);

		};

		class TokenList; /* prototype needed for TokenListNode */
		class TokenListNode: BasicClass
		{
		public:
			TokenListNode *next;
			TokenListNode *prev;
			TokenList *list;
			Token *value;

		public:
			TokenListNode(Token *token);
			~TokenListNode();

		};

		class TokenList: BasicClass
		{
		private:
			Length m_count;

		public:
			TokenListNode *head;
			TokenListNode *tail;

			std::vector<std::tuple<std::string, TokenListNode*, std::unique_ptr<std::vector<TokenListNode*>>>> labels;
			std::vector<std::string> exports;

		private:
			TokenListNode* MakeNode(Token *value);

		public:
			TokenList();
			~TokenList();
			void AddAfter(TokenListNode *node, Token *value);
			void AddBefore(TokenListNode *node, Token *value);
			void AddFirst(Token *value);
			void AddLast(Token *value);
			void Remove(TokenListNode *node);
			Length Count();

			TokenListNode* operator[](Length index);
			//friend std::ostream& operator<<(std::ostream &o, const TokenList &tl); // Out
		};
	}
}

#endif
