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
	struct IdentNode;
	struct BoolExprNode;
	struct MulExprNode;

	struct TypeNode
	{
		std::shared_ptr<IdentNode> ident; // 1
		std::shared_ptr<Token> typeToken; // 2
		int type;
	};

	struct AnonpNode
	{
		std::vector<std::shared_ptr<TypeNode>> argTypes;
		std::vector<std::shared_ptr<IdentNode>> argNames;
		std::shared_ptr<Statements> body;
	};

	struct AnonfNode
	{
		std::vector<std::shared_ptr<TypeNode>> argTypes;
		std::vector<std::shared_ptr<IdentNode>> argNames;
		std::shared_ptr<TypeNode> returnType;
		std::shared_ptr<Statements> body;
	};

	struct CastNode
	{
		std::shared_ptr<TypeNode> castType;
		std::shared_ptr<Expr> expr;
	};

	struct CreateNode
	{
		std::shared_ptr<TypeNode> createType;
		std::vector<std::shared_ptr<Expr>> args;
	};

	struct DictEntryNode
	{
		std::shared_ptr<BoolExprNode> left;
		std::shared_ptr<BoolExprNode> right;
		bool hasRight;
	};

	struct IdentNode
	{
		std::vector<std::shared_ptr<Token>> ids;
	};

	struct ListNode
	{
		std::vector<std::shared_ptr<DictEntryNode>> Items;
	};

	struct ListAccessNode
	{
		std::vector<AtomNode> indices;
	};

	struct AtomNode
	{
		std::shared_ptr<Token> token; //type: 1
		std::shared_ptr<Expr> expression; //type: 2
		std::shared_ptr<IdentNode> ident; //type: 3
		std::shared_ptr<ListAccessNode> listAccess;
		uint8_t type;
		bool listAccessPresent;
	};

	struct FactorNode
	{
		std::shared_ptr<Token> opToken;
		std::shared_ptr<AtomNode> right;
		bool opTokenPresent;
	};

	struct PowerNode
	{
		std::shared_ptr<FactorNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<PowerNode> right;
	};

	struct DivModExprNode
	{
		std::shared_ptr<PowerNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<DivModExprNode> right;
	};

	struct MulExprNode
	{
		std::shared_ptr<DivModExprNode> left;
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
		int exprType;
	};

	struct Expr
	{
		std::shared_ptr<ListNode> listNode; // exprType: 1
		std::shared_ptr<CreateNode> createNode; // exprType: 2
		std::shared_ptr<CastNode> castNode; // exprType: 3
		std::shared_ptr<CallNode> callNode; // exprType: 4
		std::shared_ptr<AnonfNode> anonfNode; // exprType: 5
		std::shared_ptr<AnonpNode> anonpNode; // exprType: 6
		uint8_t exprType;
	};

	struct ProcNode
	{
		std::shared_ptr<IdentNode> id;
		std::vector<std::shared_ptr<TypeNode>> argTypes;
		std::vector<std::shared_ptr<IdentNode>> argNames;
		std::shared_ptr<Statements> body;
	};

	struct FuncNode
	{
		std::shared_ptr<IdentNode> id;
		std::vector<std::shared_ptr<TypeNode>> argTypes;
		std::vector<std::shared_ptr<IdentNode>> argNames;
		std::shared_ptr<TypeNode> returnType;
		std::shared_ptr<Statements> body;
	};

	struct StructNode
	{
		std::shared_ptr<IdentNode> id;
		std::vector<std::shared_ptr<TypeNode>> types;
		std::vector<std::shared_ptr<IdentNode>> names;
	};

	struct ReturnNode
	{
		std::shared_ptr<Expr> expr;
	};

	struct CallNode
	{
		std::shared_ptr<Expr> function;
		std::vector<std::shared_ptr<Expr>> args;
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
		std::shared_ptr<IdentNode> id;
		std::shared_ptr<Expr> iterableExpr;
		std::shared_ptr<Statements> body;
	};

	struct DecNode
	{
		std::shared_ptr<IdentNode> id;
	};

	struct IncNode
	{
		std::shared_ptr<IdentNode> id;
	};

	struct OutputNode
	{
		std::shared_ptr<Token> raw;
	};

	struct InputNode
	{
		std::shared_ptr<Token> signSpec;
		std::shared_ptr<TypeNode> type;
		std::shared_ptr<IdentNode> id;
	};

	struct SetNode
	{
		std::shared_ptr<IdentNode> id;
		std::shared_ptr<Expr> expr;
	};

	struct IfNode
	{
		std::vector<std::shared_ptr<BoolExprNode>> conditions;
		std::vector<std::shared_ptr<Statements>> bodies;
		bool elsePresent;
	};

	struct Statement
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
		std::shared_ptr<StructNode> structNode;
		std::shared_ptr<ReturnNode> returnNode;
		uint8_t statementType;
	};

	struct Statements
	{
		std::vector<std::shared_ptr<Statement>> statements;
	};

	struct RequireNode
	{
		std::shared_ptr<IdentNode> requireType;
	};

	struct EntryPoint
	{
		std::shared_ptr<RequireNode> require;
		std::shared_ptr<Statements> statements;
	};
}