#pragma once
#include <memory>
#include <vector>
#include "Frontend/Lexer/Token.h"

namespace Spliwaca
{
	struct Statements;
	struct Expr;
	struct AtomNode;
	struct CallNode;

	struct AnonpNode
	{
		std::vector<std::shared_ptr<Token>> argTypes;
		std::vector<std::shared_ptr<Token>> argNames;
		uint32_t numArgs;
		std::shared_ptr<Statements> body;
	};

	struct AnonfNode
	{
		std::vector<std::shared_ptr<Token>> argTypes;
		std::vector<std::shared_ptr<Token>> argNames;
		uint32_t numArgs;
		std::shared_ptr<Token> returnType;
		std::shared_ptr<Statements> body;
	};

	struct CastNode
	{
		std::shared_ptr<Token> type;
		std::shared_ptr<Expr> expr;
	};

	struct CreateNode
	{
		std::shared_ptr<Token> type;
		std::shared_ptr<Expr> args;
		uint32_t numArgs;
	};

	struct DictPairNode
	{
		std::shared_ptr<AtomNode> left;
		std::shared_ptr<AtomNode> right;
	};

	struct ListNode
	{
		std::shared_ptr<DictPairNode> FirstItem;
		uint32_t numItems;
	};

	struct ListAccessNode
	{
		std::shared_ptr<Token> identifier;
		std::shared_ptr<ListNode> list;
		std::shared_ptr<AtomNode> index;
	};

	struct AtomNode
	{
		std::shared_ptr<Token> token;
		std::shared_ptr<Expr> expression;
		std::shared_ptr<ListAccessNode> listAccess;
		uint8_t type;
	};

	struct FactorNode
	{
		std::shared_ptr<Token> opToken;
		std::shared_ptr<AtomNode> right;
	};

	struct PowerNode
	{
		std::shared_ptr<FactorNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<PowerNode> right;
	};

	struct MulExprNode
	{
		std::shared_ptr<PowerNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<MulExprNode> right;
	};

	struct AddExprNode
	{
		std::shared_ptr<MulExprNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<AddExprNode> right;
	};

	struct BoolExprNode
	{
		std::shared_ptr<AddExprNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<BoolExprNode> right;
	};

	struct Expr
	{
		union
		{
			std::shared_ptr<BoolExprNode> boolExpr;
			std::shared_ptr<CreateNode> createNode;
			std::shared_ptr<CastNode> castNode;
			std::shared_ptr<CallNode> callNode;
			std::shared_ptr<AnonfNode> anonfNode;
			std::shared_ptr<AnonpNode> anonpNode;
		};
		uint8_t exprType;
	};

	struct ProcNode
	{
		std::shared_ptr<Token> id;
		std::vector<std::shared_ptr<Token>> argTypes;
		std::vector<std::shared_ptr<Token>> argNames;
		uint32_t numArgs;
		std::shared_ptr<Statements> body;
	};

	struct FuncNode
	{
		std::shared_ptr<Token> id;
		std::vector< std::shared_ptr<Token>> argTypes;
		std::vector<std::shared_ptr<Token>> argNames;
		uint32_t numArgs;
		std::shared_ptr<Token> returnType;
		std::shared_ptr<Statements> body;
	};

	struct CallNode
	{
		std::shared_ptr<Token> funcId;
		std::shared_ptr<Expr> args;
		uint32_t numArgs;
	};

	struct QuitNode
	{
		std::shared_ptr<AtomNode> returnVal;
	};

	struct WhileNode
	{
		std::shared_ptr<BoolExprNode> condition;
		std::shared_ptr<Statements> body;
	};

	struct ForNode
	{
		std::shared_ptr<Token> id;
		union
		{
			std::shared_ptr<Token> enumerableId;
			std::shared_ptr<ListNode> enumerableList;
		};
		uint8_t enumerableType;
		std::shared_ptr<Statements> body;
	};

	struct DecNode
	{
		std::shared_ptr<Token> id;
	};

	struct IncNode
	{
		std::shared_ptr<Token> id;
	};

	struct OutputNode
	{
		std::shared_ptr<Token> raw;
	};

	struct InputNode
	{
		std::shared_ptr<Token> signSpec;
		std::shared_ptr<Token> type;
		std::shared_ptr<Token> id;
	};

	struct SetNode
	{
		std::shared_ptr<Token> id;
		std::shared_ptr<Expr> expr;
	};

	struct IfNode
	{
		std::shared_ptr<BoolExprNode> conditions;
		std::shared_ptr<Statements> bodies;
		uint32_t numElifs;
		bool elsePresent;
	};

	struct Statement
	{
		union
		{
			std::shared_ptr<IfNode> ifNode;
			std::shared_ptr<SetNode> setNode;
			std::shared_ptr<InputNode> inputNode;
			std::shared_ptr<OutputNode> outputNode;
			std::shared_ptr<IncNode> incNode;
			std::shared_ptr<DecNode> decNode;
			std::shared_ptr<ForNode> forNode;
			std::shared_ptr<WhileNode> whileNode;
			std::shared_ptr<QuitNode> quitNode;
			std::shared_ptr<CallNode> callNode;
			std::shared_ptr<FuncNode> funcNode;
			std::shared_ptr<ProcNode> procNode;
		};
		uint8_t statementType;
	};

	struct Statements
	{
		std::vector<std::shared_ptr<Statement>> statements;
		uint32_t numStatements;
	};

	struct RequireNode
	{
		std::shared_ptr<Token> requireType;
	};

	struct EntryPoint
	{
		std::shared_ptr<RequireNode> require;
		std::shared_ptr<Statements> statements;
	};
}