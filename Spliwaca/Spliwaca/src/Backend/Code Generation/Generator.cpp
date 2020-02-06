#include "Generator.h"
#include "UtilFunctions.h"

namespace Spliwaca
{
	template<typename T>
	bool itemInVect(const std::vector<T>& v, T t)
	{
		for (T e : v)
		{
			if (e == t)
			{
				return true;
			}
		}
		return false;
	}

	std::shared_ptr<Generator> Generator::Create(std::shared_ptr<EntryPoint> entryPoint)
	{
		return std::make_shared<Generator>(entryPoint);
	}

	std::string Generator::GenerateCode()
	{
		m_Code = "";

		if (m_EntryPoint->require && !itemInVect({ "cpp_transpiler","CPPTranspiler","Transpiler","transpiler" }, 
													m_EntryPoint->require->requireType->GetContents()))
		{
			SPLW_CRITICAL("This generator is not compatible with the specified require statement, exiting.");
			return m_Code;
		}
		
		GenerateStatements(m_EntryPoint->statements);

		return m_Code;
	}

	void Generator::GenerateStatements(std::shared_ptr<Statements> statements)
	{
		for (std::shared_ptr<Statement> s : statements->statements)
		{
			switch (s->statementType)
			{
			case 0:  GenerateIf    (s->ifNode    ); break;
			case 1:  GenerateSet   (s->setNode   ); break;
			case 2:  GenerateInput (s->inputNode ); break;
			case 3:  GenerateOutput(s->outputNode); break;
			case 4:  GenerateInc   (s->incNode   ); break;
			case 5:  GenerateDec   (s->decNode   ); break;
			case 6:  GenerateFor   (s->forNode   ); break;
			case 7:  GenerateWhile (s->whileNode ); break;
			case 8:  GenerateQuit  (s->quitNode  ); break;
			case 9:  GenerateCall  (s->callNode, true); break;
			case 10: GenerateFunc  (s->funcNode  ); break;
			case 11: GenerateProc  (s->procNode  ); break;
			case 12: GenerateStruct(s->structNode); break;
			case 13: GenerateReturn(s->returnNode); break;
			}
		}

		m_Code += "\n";
	}

	void Generator::GenerateIf(std::shared_ptr<IfNode> node)
	{
		for (uint32_t i = 0; i < node->conditions.size(); i++)
		{
			if (i == 0)
			{
				m_Code += m_Tabs + "if ";
			}
			else
			{
				m_Code += m_Tabs + "elif ";
			}

			GenerateList(node->conditions.at(i));
			m_Code += ":\n";

			m_Tabs.append("\t");

			GenerateStatements(node->bodies.at(i));

			m_Tabs.pop_back();
		}

		if (node->elsePresent)
		{
			m_Code += m_Tabs + "else:\n";

			m_Tabs.append("\t");

			GenerateStatements(node->bodies.back());

			m_Tabs.pop_back();
		}
	}

	void Generator::GenerateSet(std::shared_ptr<SetNode> node)
	{
		m_Code += m_Tabs + node->id->GetContents() + " = ";  GenerateList(node->list);
	}

	void Generator::GenerateInput(std::shared_ptr<InputNode> node)
	{
		m_Code += m_Tabs + "libsplw."; GenerateType(node->type); m_Code += "_" + node->signSpec->GetContents() + "_input()";
	}

	void Generator::GenerateOutput(std::shared_ptr<OutputNode> node)
	{
		m_Code += m_Tabs + "print(" + ParseRaw(node->raw) + ")";
	}

	void Generator::GenerateInc(std::shared_ptr<IncNode> node)
	{
		m_Code += m_Tabs + node->id->GetContents() + " += 1";
	}

	void Generator::GenerateDec(std::shared_ptr<DecNode> node)
	{
		m_Code += m_Tabs + node->id->GetContents() + " -= 1";
	}

	void Generator::GenerateFor(std::shared_ptr<ForNode> node)
	{
		m_Code += m_Tabs + "for " + node->id->GetContents() + " in "; GenerateList(node->iterableExpr); m_Code += ":\n";
		m_Tabs += "\t";

		GenerateStatements(node->body);
		
		m_Tabs.pop_back();
	}

	void Generator::GenerateWhile(std::shared_ptr<WhileNode> node)
	{
		m_Code += m_Tabs + "while "; GenerateBinOp(node->condition); m_Code += ":\n";
		m_Tabs += "\t";

		GenerateStatements(node->body);

		m_Tabs.pop_back();
	}

	void Generator::GenerateQuit(std::shared_ptr<QuitNode> node)
	{
		m_Code += m_Tabs + "quit("; GenerateAtom(node->returnVal); m_Code += ")";
	}

	void Generator::GenerateCall(std::shared_ptr<CallNode> node, bool statement)
	{
		if (statement)
			m_Code += m_Tabs; 
		GenerateExpr(node->function); m_Code += "("; GenerateExpr(node->args.at(0));

		for (uint32_t i = 1; i < node->args.size(); i++)
		{
			m_Code += ", "; GenerateExpr(node->args.at(i));
		}

		m_Code += ")";
	}

	void Generator::GenerateFunc(std::shared_ptr<FuncNode> node)
	{
		m_Code += m_Tabs + "def "; GenerateIdent(node->id); m_Code += "("; GenerateIdent(node->argNames.at(0)); m_Code += ": "; GenerateType(node->argTypes.at(0));
		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", ";
			GenerateIdent(node->argNames.at(i));
			m_Code += ": ";
			GenerateType(node->argTypes.at(i));
		}

		m_Code += ") -> ";
		GenerateType(node->returnType);
		m_Code += ":\n";

		m_Tabs += "\t";
		GenerateStatements(node->body);
		m_Tabs.pop_back();
	}

	void Generator::GenerateProc(std::shared_ptr<ProcNode> node)
	{
		m_Code += m_Tabs + "def "; GenerateIdent(node->id); m_Code += "("; GenerateIdent(node->argNames.at(0)); m_Code += ": "; GenerateType(node->argTypes.at(0));
		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", ";
			GenerateIdent(node->argNames.at(i));
			m_Code += ": ";
			GenerateType(node->argTypes.at(i));
		}
		m_Code += "):\n";

		m_Tabs += "\t";
		GenerateStatements(node->body);
		m_Tabs.pop_back();
	}

	void Generator::GenerateStruct(std::shared_ptr<StructNode> node)
	{
		m_Code += m_Tabs + "class "; GenerateIdent(node->id); m_Code += ":\n";
		m_Tabs += "\t";
		m_Code += m_Tabs + "def __init__(self";

		assert(node->names.size() == node->types.size());

		for (uint32_t i = 0; i < node->names.size(); i++)
		{
			m_Code += ", ";
			GenerateIdent(node->names.at(i));
			m_Code += ": ";
			GenerateType(node->types.at(i));
		}
		m_Code += "):\n";

		m_Tabs += "\t";

		for (uint32_t i = 0; i < node->names.size(); i++)
		{
			m_Code += m_Tabs + "self.";
			GenerateIdent(node->names.at(i));
			m_Code += " = ";
			GenerateType(node->types.at(i));
			m_Code += "\n";
		}

		m_Tabs.pop_back();
		m_Tabs.pop_back();
	}

	void Generator::GenerateReturn(std::shared_ptr<ReturnNode> node)
	{
		m_Code += m_Tabs + "return "; GenerateList(node->list);
	}
}
