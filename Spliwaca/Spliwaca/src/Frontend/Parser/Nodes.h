#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Frontend/Lexer/Token.h"
#include <Log.h>
#include <map>

namespace Spliwaca
{
	struct Statements;
	struct Expr;
	struct AtomNode;
	struct CallNode;
	struct ListNode;
	//class BoolExprNode;
	//class MulExprNode;

	class ImportConfig {
	public:
		ImportConfig(bool allowImport, bool allowPyImport, bool allowInstall, bool allowBare)
			: allowImport(allowImport), allowPyImport(allowPyImport), allowInstall(allowInstall), allowBare(allowBare) {}
		bool allowImport;
		bool allowPyImport;
		bool allowInstall;
		bool allowBare;
	};

	class IdentNode
	{
	public:

		std::vector<std::shared_ptr<Token>> ids;
		bool accessPresent = false;

		std::string GetContents();
		std::string GenerateGetattrTree(ImportConfig *importConfig, bool &interpreter_var, bool minus_one = false);
		std::string GenerateGetattrTree(ImportConfig *importConfig, bool minus_one = false);
		std::string GetFinalId();

		inline uint32_t GetLineNumber() { return ids.at(0)->GetLineNumber(); }
		inline uint32_t GetColumnNumber() { return ids.at(0)->GetCharacterNumber(); }
		inline uint32_t GetIdentAccessNum() { return ids.size(); }

		IdentNode()
		{
		}

	private:
		std::string cachedContents = "";
		std::string cachedGetattrMinusOne = "";
		std::string cachedGetattr = "";

	};

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
		std::shared_ptr<ListNode> list;
	};

	struct CreateNode
	{
		std::shared_ptr<TypeNode> createType;
		std::vector<std::shared_ptr<Expr>> args;
	};

	struct ListAccessNode
	{
		std::vector<std::shared_ptr<ListNode>> indices;
	};

	struct AtomNode
	{
		std::shared_ptr<Token> token; //type: 1
		std::shared_ptr<ListNode> list; //type: 2
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

	/*
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

	class MulExprNode
	{
	public:

		std::shared_ptr<DivModExprNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<MulExprNode> right;
	};

	class AddExprNode
	{
	public:
		VarType GetExprReturnType();
		
		AddExprNode()
		{
		}

		std::shared_ptr<MulExprNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<AddExprNode> right;
	};

	class BoolExprNode
	{
	public:
		VarType GetExprReturnType()
		{
			if (opToken != nullptr)
			{
				return VarType::Bool;
			}
			else
			{
				return left->GetExprReturnType();
			}
		}

		BoolExprNode()
		{
		}

		std::shared_ptr<AddExprNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<BoolExprNode> right;
		int exprType;
	};
	*/

	struct BinOpNode
	{
		std::shared_ptr<FactorNode> left;
		std::shared_ptr<Token> opToken;
		std::shared_ptr<BinOpNode> right;
	};

	struct Expr
	{
		std::shared_ptr<BinOpNode> binOpNode; // exprType: 1
		std::shared_ptr<CreateNode> createNode; // exprType: 2
		std::shared_ptr<CastNode> castNode; // exprType: 3
		std::shared_ptr<CallNode> callNode; // exprType: 4
		std::shared_ptr<AnonfNode> anonfNode; // exprType: 5
		std::shared_ptr<AnonpNode> anonpNode; // exprType: 6
		uint8_t exprType;
	};

	struct DictEntryNode
	{
		std::shared_ptr<Expr> left;
		std::shared_ptr<Expr> right;
		bool hasRight;
	};

	struct ListNode
	{
		std::vector<std::shared_ptr<DictEntryNode>> Items;
	};

	struct ProcNode
	{
		std::shared_ptr<IdentNode> id;
		std::vector<std::shared_ptr<TypeNode>> argTypes;
		std::vector<std::shared_ptr<IdentNode>> argNames;
		std::shared_ptr<Statements> body;
		uint32_t lineNumber;
	};

	struct FuncNode
	{
		std::shared_ptr<IdentNode> id;
		std::vector<std::shared_ptr<TypeNode>> argTypes;
		std::vector<std::shared_ptr<IdentNode>> argNames;
		std::shared_ptr<TypeNode> returnType;
		std::shared_ptr<Statements> body;
		uint32_t lineNumber;
	};

	struct StructNode
	{
		std::shared_ptr<IdentNode> id;
		std::vector<std::shared_ptr<TypeNode>> types;
		std::vector<std::shared_ptr<IdentNode>> names;
		uint32_t lineNumber;
	};

	struct ImportNode
	{
		std::shared_ptr<IdentNode> id;
	};

	struct ReturnNode
	{
		std::shared_ptr<ListNode> list;
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
		std::shared_ptr<BinOpNode> condition;
		std::shared_ptr<Statements> body;
		uint32_t lineNumber;
	};

	struct ForNode
	{
		std::shared_ptr<IdentNode> id;
		std::shared_ptr<ListNode> iterableExpr;
		std::shared_ptr<Statements> body;
		uint32_t lineNumber;
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
		std::shared_ptr<ListNode> list;
	};

	struct IfNode
	{
		std::vector<std::shared_ptr<ListNode>> conditions;
		std::vector<std::shared_ptr<Statements>> bodies;
		bool elsePresent;
		std::vector<uint32_t> lineNumbers;
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
		std::shared_ptr<ImportNode> importNode;
		uint8_t statementType;
		uint32_t lineNumber;
	};

	struct Statements
	{
		std::vector<std::shared_ptr<Statement>> statements;
	};

	struct RequireNode
	{
		std::shared_ptr<IdentNode> requireType;
	};

	struct VarInfo {
		int type; // 0: Callable, 1: Variable
		int declLine;
		union {
			char flags;
			struct {
				char UndefinedAtFirstUse : 1;
				char GlobalRead : 1;
				char GlobalMod : 1;
				
			} Flags;
		};
	};

	struct SemanticState {
		std::string currentScope = "globals";
		std::map<std::string, VarInfo> variables = {};
		std::string setVar = "";
		bool set = false;
		bool incdec = false;
	};

	struct EntryPoint
	{
		std::shared_ptr<RequireNode> require;
		std::shared_ptr<Statements> statements;
		bool requirePresent;

		SemanticState *semanticState;
	};
}
