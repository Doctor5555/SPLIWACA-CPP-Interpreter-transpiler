#pragma once
#include <memory>
#include <vector>
#include "Frontend/Lexer/Token.h"

namespace Spliwaca
{
	class EntryPoint
	{
		RequireNode* require;
		Statements* statements;
	};

	class RequireNode
	{
		Token* requireType;
	};

	class Statements
	{
		Statement* statements;
		uint32_t numStatements;
	};

	class Statement
	{
		union
		{
			IfNode* ifNode;
			SetNode* setNode;
			InputNode* inputNode;
			OutputNode* outputNode;
			IncNode* incNode;
			DecNode* decNode;
			ForNode* forNode;
			WhileNode* whileNode;
			QuitNode* quitNode;
			CallNode* callNode;
			FuncNode* funcNode;
			ProcNode* procNode;
		};
		uint8_t statementType;
	};

	class IfNode
	{
		BoolExprNode* conditions;
		Statements* bodies;
		uint32_t numElifs;
		bool elsePresent;
	};

	class SetNode
	{
		Token* id;
		Expr* expr;
	};

	class InputNode
	{
		Token* signSpec;
		Token* type;
		Token* id;
	};

	class OutputNode
	{
		Token* raw;
	};

	class IncNode
	{
		Token* id;
	};

	class DecNode
	{
		Token* id;
	};

	class ForNode
	{
		Token* id;
		union
		{
			Token* enumerableId;
			ListNode* enumerableList;
		};
		uint8_t enumerableType;
		Statements* body;
	};

	class WhileNode
	{
		BoolExprNode* condition;
		Statements* body;
	};

	class QuitNode
	{
		AtomNode* returnVal;
	};

	class CallNode
	{
		Token* funcId;
		Expr* args;
		uint32_t numArgs;
	};

	class FuncNode
	{
		Token* id;
		Token** argTypes;
		Token** argNames;
		uint32_t numArgs;
		Token* returnType;
		Statements* body;
	};

	class ProcNode
	{
		Token* id;
		Token** argTypes;
		Token** argNames;
		uint32_t numArgs;
		Statements* body;
	};
	
	class Expr
	{

	private:
		union
		{
			BoolExprNode* boolExpr;
			CreateNode* createNode;
			CastNode* castNode;
			CallNode* callNode;
			AnonfNode* anonfNode;
			AnonpNode* anonpNode;
		};
		uint8_t exprType;
	};

	class BoolExprNode
	{
	public:
		BoolExprNode()
		{ }

		~BoolExprNode() 
		{
			if (left)
				left->~AddExprNode();

			if (right)
				right->~BoolExprNode();
		}

	private:
		AddExprNode* left;
		Token* opToken;
		BoolExprNode* right;
	};

	class AddExprNode
	{
	public:
		AddExprNode(){}

		~AddExprNode()
		{

		}

	private:
		MulExprNode* left;
		Token* opToken;
		AddExprNode* right;
	};

	class MulExprNode
	{

	private:
		PowerNode* left;
		Token* opToken;
		MulExprNode* right;
	};

	class PowerNode
	{

	private:
		FactorNode* left;
		Token* opToken;
		PowerNode* right;
	};

	class FactorNode
	{

	private:
		Token* opToken;
		AtomNode* right;
	};

	class AtomNode
	{

	private:
		Token* token;
		Expr* expression;
		ListAccessNode* listAccess;
		uint8_t type;
	};

	class ListAccessNode
	{
		
	private:
		Token* identifier;
		ListNode* list;
		AtomNode* index;
	};
	
	class ListNode
	{

	private:
		DictPairNode* FirstItem;
		uint32_t numItems;
	};

	class DictPairNode
	{

	private:
		AtomNode* left;
		AtomNode* right;
	};

	class CreateNode
	{
		Token* type;
		Expr* args;
		uint32_t numArgs;
	};

	class CastNode
	{
		Token* type;
		Expr* expr;
	};

	class AnonfNode
	{
		Token** argTypes;
		Token** argNames;
		uint32_t numArgs;
		Token* returnType;
		Statements* body;
	};

	class AnonpNode
	{
		Token** argTypes;
		Token** argNames;
		uint32_t numArgs;
		Statements* body;
	};

	class Parser
	{
	public:
		std::shared_ptr<Parser> Create();
		~Parser() = default;



	};
}