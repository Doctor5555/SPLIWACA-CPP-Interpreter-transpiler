#pragma once
#include "Frontend/Parser/Nodes.h"

namespace Spliwaca
{
	class Generator
	{
	public:
		static std::shared_ptr<Generator> Create(std::shared_ptr<EntryPoint> entryPoint);
		~Generator() = default;

		std::string GenerateCode();

		Generator(std::shared_ptr<EntryPoint> ep)
			: m_EntryPoint(ep), m_Tabs(""), m_Code("")
		{}
	private:

		std::shared_ptr<EntryPoint> m_EntryPoint;
		std::string m_Tabs;
		std::string m_Code;

	private:
		void GenerateStatements(std::shared_ptr<Statements> s);
		
		void GenerateIf(std::shared_ptr<IfNode> node);
		void GenerateSet(std::shared_ptr<SetNode> node);
		void GenerateInput(std::shared_ptr<InputNode> node);
		void GenerateOutput(std::shared_ptr<OutputNode> node);
		void GenerateInc(std::shared_ptr<IncNode> node);
		void GenerateDec(std::shared_ptr<DecNode> node);
		void GenerateFor(std::shared_ptr<ForNode> node);
		void GenerateWhile(std::shared_ptr<WhileNode> node);
		void GenerateQuit(std::shared_ptr<QuitNode> node);
		void GenerateCall(std::shared_ptr<CallNode> node, bool statement);
		void GenerateFunc(std::shared_ptr<FuncNode> node);
		void GenerateProc(std::shared_ptr<ProcNode> node);
		void GenerateStruct(std::shared_ptr<StructNode> node);
		void GenerateReturn(std::shared_ptr<ReturnNode> node);
		void GenerateImport(std::shared_ptr<ImportNode> node);
		
		void GenerateList(std::shared_ptr<ListNode> node);
		void GenerateDictEntry(std::shared_ptr<DictEntryNode> node);
		void GenerateExpr(std::shared_ptr<Expr> node);
		void GenerateBinOp(std::shared_ptr<BinOpNode> node);
		void GenerateFactor(std::shared_ptr<FactorNode> node);
		void GenerateAtom(std::shared_ptr<AtomNode> node);

		void GenerateCreate(std::shared_ptr<CreateNode> node);
		void GenerateCast(std::shared_ptr<CastNode> node);
		void GenerateAnonf(std::shared_ptr<AnonfNode> node);
		void GenerateAnonp(std::shared_ptr<AnonpNode> node);
		
		void GenerateType(std::shared_ptr<TypeNode> node);
		//void GenerateIdent(std::shared_ptr<IdentNode> node);

		std::string ParseRaw(std::shared_ptr<Token> token);
		std::string ParseComplex(std::shared_ptr<Token> token);
	};

	/*std::shared_ptr<IfNode> ifNode;
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
		std::shared_ptr<ReturnNode> returnNode;*/
}