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

/*	bool charInStr(const std::string& s, char c)
	{
		if (s.find(c) != std::string::npos)
			return true;
		else
			return false;
	}*/

	std::shared_ptr<Generator> Generator::Create(std::shared_ptr<EntryPoint> entryPoint)
	{
		return std::make_shared<Generator>(entryPoint);
	}

	std::string Generator::GenerateCode(int& errorCode)
	{
		m_Code = "import libsplw\nglobal_vars = libsplw.VariableHolder()\n\n";

		if (m_EntryPoint->require && m_EntryPoint->require->requireType->GetContents() == "transpiler_py"){
			allowPyAndPipImports = true;
		}
		else if (m_EntryPoint->requirePresent == false || m_EntryPoint->require->requireType->GetContents() == "transpiler") {

		}
		else if (m_EntryPoint->require) {
			SPLW_CRITICAL("This generator is not compatible with the specified require statement, exiting.");
			errorCode = 1;
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
			case 0:  GenerateIf(s->ifNode); break;
			case 1:  GenerateSet(s->setNode); break;
			case 2:  GenerateInput(s->inputNode); break;
			case 3:  GenerateOutput(s->outputNode); break;
			case 4:  GenerateInc(s->incNode); break;
			case 5:  GenerateDec(s->decNode); break;
			case 6:  GenerateFor(s->forNode); break;
			case 7:  GenerateWhile(s->whileNode); break;
			case 8:  GenerateQuit(s->quitNode); break;
			case 9:  GenerateCall(s->callNode, true); break;
			case 10: GenerateFunc(s->funcNode); break;
			case 11: GenerateProc(s->procNode); break;
			case 12: GenerateStruct(s->structNode); break;
			case 13: GenerateReturn(s->returnNode); break;
			case 14: GenerateImport(s->importNode); break;
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

			m_Tabs += "    ";

			GenerateStatements(node->bodies.at(i));

			m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);
		}

		if (node->elsePresent)
		{
			m_Code += m_Tabs + "else:\n";

			m_Tabs += "    ";

			GenerateStatements(node->bodies.back());

			m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);
		}
	}

	void Generator::GenerateSet(std::shared_ptr<SetNode> node)
	{
		if (node->id->accessPresent) {
			bool interpreter_var = false;
			std::string getattrTree = node->id->GenerateGetattrTree(interpreter_var, true);
			if (!interpreter_var) {
				m_Code += m_Tabs + "setattr(" + getattrTree + ", \"" + node->id->GetFinalId() + "\", "; GenerateList(node->list); m_Code += ")";
			}
			else {
				m_Code += m_Tabs + getattrTree + " = "; GenerateList(node->list);
			}
		}
		else {
			m_Code += m_Tabs + "scope_vars[\"" + node->id->GetContents() + "\"] = ";  GenerateList(node->list);
		}
	}

	void Generator::GenerateInput(std::shared_ptr<InputNode> node)
	{
		if (node->id->accessPresent) {
			bool interpreter_var = false;
			std::string getattrTree = node->id->GenerateGetattrTree(interpreter_var, true);
			if (!interpreter_var) {
				m_Code += m_Tabs + "setattr(" + getattrTree + ", \"" + node->id->GetFinalId() + "\", libsplw.handle_input('";
			}
			else {
				m_Code += m_Tabs + getattrTree + " = libsplw.handle_input('";
			}
			if (node->signSpec)
				m_Code += node->signSpec->GetContents() + " ";
			GenerateType(node->type);
			m_Code += "')";
		} else {
			m_Code += m_Tabs + "scope_vars[\"" + node->id->GetContents() + "\"] = libsplw.handle_input('";
			if (node->signSpec)
				m_Code += node->signSpec->GetContents() + " ";
			GenerateType(node->type);
			m_Code += "')";
		}
	}

	void Generator::GenerateOutput(std::shared_ptr<OutputNode> node)
	{
		m_Code += m_Tabs + "print(" + ParseRaw(node->raw) + ")";
	}

	void Generator::GenerateInc(std::shared_ptr<IncNode> node)
	{
		if (node->id->accessPresent) {
			bool interpreter_var = false;
			std::string getAttrTree = node->id->GenerateGetattrTree(interpreter_var, true);
			if (interpreter_var) {
				m_Code += m_Tabs + getAttrTree + " += 1";
			}
			else {
				m_Code += m_Tabs + "setattr(" + getAttrTree + ", \"" + node->id->GetFinalId() + "\", " + node->id->GenerateGetattrTree() + " + 1)";
			}
		} else {
			m_Code += m_Tabs + "scope_vars[\"" + node->id->GetContents() + "\"] += 1";
		}
	}

	void Generator::GenerateDec(std::shared_ptr<DecNode> node)
	{
		if (node->id->accessPresent) {
			bool interpreter_var = false;
			std::string getAttrTree = node->id->GenerateGetattrTree(interpreter_var, true);
			if (interpreter_var) {
				m_Code += m_Tabs + getAttrTree + " -= 1";
			} else {
				m_Code += m_Tabs + "setattr(" + getAttrTree + ", \"" + node->id->GetFinalId() + "\", " + node->id->GenerateGetattrTree() + " - 1)";
			}
		} else {
			m_Code += m_Tabs + "scope_vars[\"" + node->id->GetContents() + "\"] -= 1";
		}
	}

	void Generator::GenerateFor(std::shared_ptr<ForNode> node)
	{
		if (node->id->accessPresent) {
			bool interpreter_var = false;
			std::string getAttrTree = node->id->GenerateGetattrTree(interpreter_var, true);
			if (interpreter_var) {
				m_Code += m_Tabs + "for " + getAttrTree + " in "; GenerateList(node->iterableExpr); m_Code += ":\n";
			} else {
				std::string for_var = "__for_var_line_" + std::to_string(node->id->GetLineNumber());
				m_Code += m_Tabs + "for " + for_var + " in "; GenerateList(node->iterableExpr); m_Code += ":\n";
				m_Code += m_Tabs + "    setattr(" + getAttrTree + ", " + node->id->GetFinalId() + ", " + for_var + ")";
			}
		}
		else {
			std::string for_var = "__for_var_line_" + std::to_string(node->id->GetLineNumber());
			m_Code += m_Tabs + "for " + for_var + " in "; GenerateList(node->iterableExpr); m_Code += ":\n";
			m_Code += m_Tabs + "    scope_vars[\"" + node->id->GetContents() + "\"] = " + for_var;
		}
		m_Tabs += "    ";

		GenerateStatements(node->body);

		m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);
	}

	void Generator::GenerateWhile(std::shared_ptr<WhileNode> node)
	{
		m_Code += m_Tabs + "while "; GenerateBinOp(node->condition); m_Code += ":\n";
		m_Tabs += "    ";

		GenerateStatements(node->body);

		m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);
	}

	void Generator::GenerateQuit(std::shared_ptr<QuitNode> node)
	{
		m_Code += m_Tabs + "exit(";
		if (node->returnVal)
			GenerateAtom(node->returnVal);
		m_Code += ")";
	}

	void Generator::GenerateCall(std::shared_ptr<CallNode> node, bool statement)
	{
		if (statement)
			m_Code += m_Tabs;
		GenerateExpr(node->function); m_Code += "(";

		if (node->args.size() != 0)
		{
			GenerateExpr(node->args.at(0));

			for (uint32_t i = 1; i < node->args.size(); i++)
			{
				m_Code += ", "; GenerateExpr(node->args.at(i));
			}
		}
		m_Code += ")";
	}

	void Generator::GenerateFunc(std::shared_ptr<FuncNode> node)
	{
		m_Code += m_Tabs + "@libsplw.type_check\ndef " + node->id->GetContents() + "(";
		if (node->argNames.size() != 0) {
			m_Code += node->argNames.at(0)->GetContents() + ": ";
			GenerateType(node->argTypes.at(0));
		}

		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", " + node->argNames.at(i)->GetContents() + ": ";
			GenerateType(node->argTypes.at(i));
		}

		m_Code += ") -> ";
		GenerateType(node->returnType);
		m_Code += ":\n";

		m_Tabs += "    ";
		GenerateStatements(node->body);
		m_Code += m_Tabs + "raise libsplw.FunctionEndError";
		m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);
	}

	void Generator::GenerateProc(std::shared_ptr<ProcNode> node)
	{
		m_Code += m_Tabs + "@libsplw.type_check\ndef " + node->id->GetContents() + "(";
		if (node->argNames.size() != 0) {
			m_Code += node->argNames.at(0)->GetContents() + ": ";
			GenerateType(node->argTypes.at(0));
		}

		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", " + node->argNames.at(i)->GetContents() + ": ";
			GenerateType(node->argTypes.at(i));
		}
		m_Code += ") -> None:\n";

		m_Tabs += "    ";
		GenerateStatements(node->body);
		m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);
	}

	void Generator::GenerateStruct(std::shared_ptr<StructNode> node)
	{
		m_Code += m_Tabs + "class " + node->id->GetContents() + ":\n";
		m_Tabs += "    ";
		m_Code += m_Tabs + "def __init__(self";

		assert(node->names.size() == node->types.size());

		for (uint32_t i = 0; i < node->names.size(); i++)
		{
			m_Code += ", " + node->names.at(i)->GetContents() + ": ";
			m_Code += ": ";
			GenerateType(node->types.at(i));
		}
		m_Code += "):\n";

		m_Tabs += "    ";

		for (uint32_t i = 0; i < node->names.size(); i++)
		{
			m_Code += ", " + node->names.at(i)->GetContents() + " = ";
			GenerateType(node->types.at(i));
			m_Code += "\n";
		}

		m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);
		m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);
	}

	void Generator::GenerateReturn(std::shared_ptr<ReturnNode> node)
	{
		m_Code += m_Tabs + "return "; GenerateList(node->list);
	}

	void Generator::GenerateImport(std::shared_ptr<ImportNode> node)
	{
		m_Code += m_Tabs + "import " + node->id->GetContents();
	}

	void Generator::GenerateList(std::shared_ptr<ListNode> node, bool fromAtom)
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
			if (fromAtom) {
				m_Code += "(";
			}
			GenerateDictEntry(node->Items.at(0));
			if (fromAtom){
				m_Code += ")";
			}
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
			std::string opTokenStr = "";
			//std::transform(opTokenStr.begin(), opTokenStr.end(), opTokenStr.begin(),
			//	[](unsigned char c) { return std::tolower(c); });
			switch (node->opToken->GetType()) {
			case TokenType::Is:
				opTokenStr = "is"; break;
			case TokenType::Not:
				opTokenStr = "not"; break;
			case TokenType::And:
				opTokenStr = "and"; break;
			case TokenType::Or:
				opTokenStr = "or"; break;
			case TokenType::Equal:
				opTokenStr = "=="; break;
			case TokenType::NotEqual:
				opTokenStr = "!="; break;
			case TokenType::LessThan:
				opTokenStr = "<"; break;
			case TokenType::GreaterThan:
				opTokenStr = ">"; break;
			case TokenType::LessThanEqual:
				opTokenStr = "<="; break;
			case TokenType::GreaterThanEqual:
				opTokenStr = ">="; break;
			case TokenType::Multiply:
				opTokenStr = "*"; break;
			case TokenType::Divide:
				opTokenStr = "/"; break;
			case TokenType::Intdiv:
				opTokenStr = "//"; break;
			case TokenType::Plus:
				opTokenStr = "+"; break;
			case TokenType::Minus:
				opTokenStr = "-"; break;
			case TokenType::Modulo:
				opTokenStr = "%"; break;
			case TokenType::Xor:
				opTokenStr = "^"; break;
			case TokenType::BitwiseAnd:
				opTokenStr = "&"; break;
			case TokenType::BitwiseOr:
				opTokenStr = "|"; break;
			case TokenType::ShiftRight:
				opTokenStr = ">>"; break;
			case TokenType::ShiftLeft:
				opTokenStr = "<<"; break;
			default:
				SPLW_CRITICAL("Bug: Operator {0} not handled", node->opToken->GetContents());
			}
			m_Code += " " + opTokenStr + " ";
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
			else if (node->token->GetType() == TokenType::True)
				m_Code += "True";
			else if (node->token->GetType() == TokenType::False)
				m_Code += "False";
			else
				m_Code += StripLeadingZeros(node->token->GetContents());
			break;
		}
		case 2: GenerateList(node->list, true); break;
		case 3: m_Code += node->ident->GetContents(); break;
		}

		if (node->listAccessPresent)
		{
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

		std::string anonf_randomised_name = std::to_string(node->argNames.at(0)->GetLineNumber()) + "_" + std::to_string(std::rand());
		m_Code += m_Tabs + "@libsplw.type_check\ndef anonf_line_" + anonf_randomised_name + "(";
		if (node->argNames.size() != 0) {
			m_Code += node->argNames.at(0)->GetContents() + ": ";
			GenerateType(node->argTypes.at(0));
		}
		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", " + node->argNames.at(i)->GetContents() + ": ";
			GenerateType(node->argTypes.at(i));
		}

		m_Code += ") -> ";
		GenerateType(node->returnType);
		m_Code += ":\n";

		m_Tabs += "    ";
		GenerateStatements(node->body);
		m_Code += m_Tabs + "raise libsplw.FunctionEndError";
		m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);

		m_Code += code + "anonf_line_" + anonf_randomised_name;
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

		std::string anonp_randomised_name = std::to_string(node->argNames.at(0)->GetLineNumber()) + "_" + std::to_string(std::rand());
		m_Code += m_Tabs + "@libsplw.type_check\ndef anonp_line_" + anonp_randomised_name + "(";
		if (node->argNames.size() != 0) {
			m_Code += node->argNames.at(0)->GetContents() + ": ";
			GenerateType(node->argTypes.at(0));
		}
		assert(node->argNames.size() == node->argTypes.size());

		for (uint32_t i = 1; i < node->argNames.size(); i++)
		{
			m_Code += ", " + node->argNames.at(i)->GetContents() + ": ";
			GenerateType(node->argTypes.at(i));
		}
		m_Code += ") -> None:\n";

		m_Tabs += "    ";
		GenerateStatements(node->body);
		m_Tabs = m_Tabs.substr(0, m_Tabs.size() - 4);

		m_Code += code + "anonp_line_" + anonp_randomised_name;
	}

	void Generator::GenerateType(std::shared_ptr<TypeNode> node)
	{
		if (node->type == 1)
			m_Code += node->ident->GetContents();
		else {
			//m_Code += "__builtins__.";
			std::string typeTokenStr = node->typeToken->GetContents();
			std::transform(typeTokenStr.begin(), typeTokenStr.end(), typeTokenStr.begin(),
				[](unsigned char c) { return std::tolower(c); });
			if (typeTokenStr == "string")
				typeTokenStr = "str";
			else if (typeTokenStr == "real")
				typeTokenStr = "float";
			else if (typeTokenStr == "number")
				typeTokenStr = "float";
			else if (typeTokenStr == "integer")
				typeTokenStr = "int";
			else if (typeTokenStr == "dictionary")
				typeTokenStr = "dict";
			else if (typeTokenStr == "mapping")
				typeTokenStr = "dict";
			else if (typeTokenStr == "map")
				typeTokenStr = "dict";

			m_Code += typeTokenStr;
		}
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
				{
					code += "} ";
					inIdent = false;
				}
				else if (!charInStr(allowedChars, c))
					SPLW_CRITICAL("Invalid character {1} in RAW ident on line {0}, ignoring", token->GetLineNumber(), c);
				else
					code += c;
			}
			else if (c == '"')
			{
				//code += "\" + \"\\\"\" + fr\"";
				code += "\\";
			}
			else
			{
				inIdent = true;
				code += "{";
			}
		}
		if (inIdent)
			code += "}";
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
		return StripLeadingZeros(code);
	}

	std::string Generator::StripLeadingZeros(std::string token) {
		uint64_t EndOfLeadingZeros = 0;
		while (token[EndOfLeadingZeros] == '0' || token[EndOfLeadingZeros] == '_')
			EndOfLeadingZeros++;
		if (token[EndOfLeadingZeros] == '.')
			EndOfLeadingZeros--;
		return token.substr(EndOfLeadingZeros);
	}
}
