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

	bool charInStr(const std::string& s, char c)
	{
		if (s.find(c) != std::string::npos)
			return true;
		else
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
			m_Code += "\n";
		}
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
		m_Code += m_Tabs + "def " + node->id->GetContents() + "(" + node->argNames.at(0)->GetContents() + ": "; GenerateType(node->argTypes.at(0));
		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", " + node->argNames.at(i)->GetContents() + ": ";
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
		m_Code += m_Tabs + "def " + node->id->GetContents() + "(" + node->argNames.at(0)->GetContents() + ": "; GenerateType(node->argTypes.at(0));
		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", " + node->argNames.at(i)->GetContents() + ": ";
			GenerateType(node->argTypes.at(i));
		}
		m_Code += "):\n";

		m_Tabs += "\t";
		GenerateStatements(node->body);
		m_Tabs.pop_back();
	}

	void Generator::GenerateStruct(std::shared_ptr<StructNode> node)
	{
		m_Code += m_Tabs + "class " + node->id->GetContents() + ":\n";
		m_Tabs += "\t";
		m_Code += m_Tabs + "def __init__(self";

		assert(node->names.size() == node->types.size());

		for (uint32_t i = 0; i < node->names.size(); i++)
		{
			m_Code += ", " + node->names.at(i)->GetContents() + ": ";
			m_Code += ": ";
			GenerateType(node->types.at(i));
		}
		m_Code += "):\n";

		m_Tabs += "\t";

		for (uint32_t i = 0; i < node->names.size(); i++)
		{
			m_Code += ", " + node->names.at(i)->GetContents() + " = ";
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
	
	void Generator::GenerateList(std::shared_ptr<ListNode> node)
	{
		if (node->Items.at(0)->hasRight)
		{
			m_Code += "{";
		}
		else if (node->Items.size() > 1)
		{
			m_Code += "[";
		}
		else
		{
			GenerateDictEntry(node->Items.at(0));
			return;
		}

		GenerateDictEntry(node->Items.at(0));
		for (uint32_t i = 1; i < node->Items.size(); i++)
		{
			m_Code += ", ";
			GenerateDictEntry(node->Items.at(i));
		}

		if (node->Items.at(0)->hasRight)
		{
			m_Code += "}";
		}
		else if (node->Items.size() > 1)
		{
			m_Code += "]";
		}
	}

	void Generator::GenerateDictEntry(std::shared_ptr<DictEntryNode> node)
	{
		GenerateExpr(node->left);
		if (node->hasRight)
		{
			m_Code.append(": ");
			GenerateExpr(node->right);
		}
	}

	void Generator::GenerateExpr(std::shared_ptr<Expr> node)
	{
		switch (node->exprType)
		{
		case 1: GenerateBinOp(node->binOpNode); break;
		case 2: GenerateCreate(node->createNode); break;
		case 3: GenerateCast(node->castNode); break;
		case 4: GenerateCall(node->callNode, false); break;
		case 5: GenerateAnonf(node->anonfNode); break;
		case 6: GenerateAnonp(node->anonpNode); break;
		}
	}

	void Generator::GenerateBinOp(std::shared_ptr<BinOpNode> node)
	{
		GenerateFactor(node->left);
		if (node->opToken)
		{
			m_Code += node->opToken->GetContents();
			GenerateBinOp(node->right);
		}
	}

	void Generator::GenerateFactor(std::shared_ptr<FactorNode> node)
	{
		if (node->opTokenPresent)
		{
			m_Code += node->opToken->GetContents();
		}
		GenerateAtom(node->right);
	}

	void Generator::GenerateAtom(std::shared_ptr<AtomNode> node)
	{
		switch (node->type)
		{
		case 1:
		{
			if (node->token->GetType() == TokenType::Raw)
				m_Code += ParseRaw(node->token);
			else if (node->token->GetType() == TokenType::String)
				m_Code += "\"" + node->token->GetContents() + "\"";
			else if (node->token->GetType() == TokenType::Complex)
				m_Code += ParseComplex(node->token);
			else
				m_Code += node->token->GetContents();
			break;
		}
		case 2: GenerateList(node->list); break;
		case 3: m_Code += node->ident->GetContents(); break;
		}

		if (node->listAccessPresent){
			for (std::shared_ptr<ListNode> n : node->listAccess->indices)
			{
				m_Code += "[";
				GenerateList(n);
				m_Code += "]";
			}
		}
	}

	void Generator::GenerateCreate(std::shared_ptr<CreateNode> node)
	{
		GenerateType(node->createType); m_Code += "("; GenerateExpr(node->args.at(0));

		for (uint32_t i = 1; i < node->args.size(); i++)
		{
			m_Code += ", "; GenerateExpr(node->args.at(i));
		}

		m_Code += ")";
	}

	void Generator::GenerateCast(std::shared_ptr<CastNode> node)
	{
		GenerateType(node->castType); m_Code += "("; GenerateList(node->list); m_Code += ")";
	}

	void Generator::GenerateAnonf(std::shared_ptr<AnonfNode> node)
	{
		uint32_t charIndex = m_Code.size() - 1;
		std::string code = "";
		while (m_Code.at(charIndex) != '\n' && charIndex != -1)
		{
			code = m_Code.back() + code;
			m_Code.pop_back();
			charIndex--;
		}

		m_Code += m_Tabs + "def anonf_line_" + std::to_string(node->argNames.at(0)->GetLineNumber()) +"_" + std::to_string(std::rand()) + "(" + node->argNames.at(0)->GetContents() + ": "; GenerateType(node->argTypes.at(0));
		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", " + node->argNames.at(i)->GetContents() + ": ";
			GenerateType(node->argTypes.at(i));
		}

		m_Code += ") -> ";
		GenerateType(node->returnType);
		m_Code += ":\n";

		m_Tabs += "\t";
		GenerateStatements(node->body);
		m_Tabs.pop_back();

		m_Code += code;
	}

	void Generator::GenerateAnonp(std::shared_ptr<AnonpNode> node)
	{
		uint32_t charIndex = m_Code.size() - 1;
		std::string code = "";
		while (m_Code.at(charIndex) != '\n' && charIndex != -1)
		{
			code = m_Code.back() + code;
			m_Code.pop_back();
			charIndex--;
		}

		m_Code += m_Tabs + "def anonf_line_" + std::to_string(node->argNames.at(0)->GetLineNumber()) + "_" + std::to_string(std::rand()) + "(" + node->argNames.at(0)->GetContents() + ": "; GenerateType(node->argTypes.at(0));
		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", " + node->argNames.at(i)->GetContents() + ": ";
			GenerateType(node->argTypes.at(i));
		}
		m_Code += "):\n";

		m_Tabs += "\t";
		GenerateStatements(node->body);
		m_Tabs.pop_back();

		m_Code += code;
	}

	void Generator::GenerateType(std::shared_ptr<TypeNode> node)
	{
		if (node->type == 1)
			m_Code += node->ident->GetContents();
		else
			m_Code += node->typeToken->GetContents();
	}

	/*void Generator::GenerateIdent(std::shared_ptr<IdentNode> node)
	{
		m_Code += node->GetContents();
	}*/

	std::string Generator::ParseRaw(std::shared_ptr<Token> token)
	{
		std::string code = "fr\"";
		bool inIdent = false;
		std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789.";
		for (char c : token->GetContents())
		{
			if (!charInStr("$\"", c) && !inIdent)
				code += c;
			else if (inIdent)
			{
				if (c == ' ')
					code += "} ";
				else if (!charInStr(allowedChars, c))
					SPLW_CRITICAL("Invalid character {1} in RAW ident on line {0}, ignoring", token->GetLineNumber(), c);
				else
					code += c;
			}
			else if (c == '"')
			{
				code += "\" + \"\\\"\" + fr\"";
			}
			else
			{
				inIdent = true;
				code += "{";
			}
		}
		return code + "\"";
	}

	std::string Generator::ParseComplex(std::shared_ptr<Token> token)
	{
		std::string code = "";
		for (char c : token->GetContents())
		{
			if (c == 'i')
				code += 'j';
			else
				code += c;
		}
		return code;
	}
}
