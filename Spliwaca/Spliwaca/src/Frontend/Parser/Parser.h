#pragma once
#include <memory>
#include <vector>
#include "Frontend/Lexer/Token.h"
#include "Nodes.h"
#include "Frontend/Scopes/Scope.h"

namespace Spliwaca
{
	class Parser
	{
	public:
		static std::shared_ptr<Parser> Create(std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens);
		~Parser() = default;

		std::shared_ptr<EntryPoint> ConstructAST();
		
	private:
		inline uint32_t IncIndex() { m_TokenIndex++; return m_TokenIndex; }

		Parser(std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens)
			: m_Tokens(tokens), m_TokenIndex(0) { }

		std::shared_ptr<std::vector<std::shared_ptr<Token>>> m_Tokens;
		uint32_t m_TokenIndex;

		//std::shared_ptr<Scope> m_MainScope;
		//std::vector<std::shared_ptr<Scope>> m_ScopeStack;
		//std::shared_ptr<Scope> m_CurrentScope;

	private:
		std::shared_ptr<RequireNode> ConstructRequire();
		std::shared_ptr<Statements> ConstructStatements();
		std::shared_ptr<Statement> ConstructStatement();

		std::shared_ptr<IfNode> ConstructIf();
		std::shared_ptr<SetNode> ConstructSet();
		std::shared_ptr<InputNode> ConstructInput();
		std::shared_ptr<OutputNode> ConstructOutput();
		std::shared_ptr<IncNode> ConstructIncrement();
		std::shared_ptr<DecNode> ConstructDecrement();
		std::shared_ptr<ForNode> ConstructFor();
		std::shared_ptr<WhileNode> ConstructWhile();
		std::shared_ptr<QuitNode> ConstructQuit();
		std::shared_ptr<CallNode> ConstructCall();
		std::shared_ptr<ImportNode> ConstructImport();
		std::shared_ptr<FuncNode> ConstructFunction();
		std::shared_ptr<ProcNode> ConstructProcedure();
		std::shared_ptr<StructNode> ConstructStruct();

		std::shared_ptr<Expr> ConstructExpr();

		std::shared_ptr<ListNode> ConstructList();
		std::shared_ptr<DictEntryNode> ConstructDictEntry();
		/*std::shared_ptr<BoolExprNode> ConstructBooleanExpr();
		std::shared_ptr<AddExprNode> ConstructAddExpr();
		std::shared_ptr<MulExprNode> ConstructMulExpr();
		std::shared_ptr<DivModExprNode> ConstructDivModExpr();
		std::shared_ptr<PowerNode> ConstructPower();*/
		std::shared_ptr<BinOpNode> ConstructBinOpNode();
		std::shared_ptr<FactorNode> ConstructFactor();
		std::shared_ptr<AtomNode> ConstructAtom();
		std::shared_ptr<ListAccessNode> ConstructListAccess();

		std::shared_ptr<CreateNode> ConstructCreate();
		std::shared_ptr<CastNode> ConstructCast();
		std::shared_ptr<ReturnNode> ConstructReturn();
		std::shared_ptr<AnonfNode> ConstructAnonFunc();
		std::shared_ptr<AnonpNode> ConstructAnonProc();

		std::shared_ptr<TypeNode> ConstructTypeNode();
		std::shared_ptr<IdentNode> ConstructIdentNode();

	};
}
