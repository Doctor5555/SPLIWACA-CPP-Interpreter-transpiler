#include "Parser.h"
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

	std::shared_ptr<Parser> Parser::Create(std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens)
	{
		return std::shared_ptr<Parser>(new Parser(tokens));
	}

	std::shared_ptr<EntryPoint> Parser::ConstructAST()
	{
		std::shared_ptr<EntryPoint> ep = std::make_shared<EntryPoint>();
		//m_MainScope = std::make_shared<Scope>("Main", 0, ScopeType::Main);
		//m_CurrentScope = m_MainScope;
		//m_ScopeStack.push_back(m_MainScope);

		ep->require = ConstructRequire();

		// Consume newline after require
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
		{
			RegisterSyntaxError(SyntaxErrorType::expNewline, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex(); 

		//Begin constructing statements
		ep->statements = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::eof)
		{
			RegisterSyntaxError(SyntaxErrorType::expNewline, m_Tokens->at(m_TokenIndex));
			return ep;
		}
		else
			return ep;
	}

	std::shared_ptr<RequireNode> Parser::ConstructRequire()
	{
		std::shared_ptr<RequireNode> node = std::make_shared<RequireNode>();
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Require)
		{
			IncIndex();
			node->requireType = ConstructIdentNode();
			m_TokenIndex++;
			return node;
		}
		return nullptr;
	}

	std::shared_ptr<Statements> Parser::ConstructStatements()
	{
		std::shared_ptr<Statements> statements = std::make_shared<Statements>();
		while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::eof)
		{
			//Attempt to consume newline
			while (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Newline)
			{
				IncIndex();
			}

			TokenType currTokType = m_Tokens->at(m_TokenIndex)->GetType();
			if (currTokType == TokenType::eof)
			{
				//If we have reached the end of the file, return
				break;
			}
			else if (currTokType == TokenType::End)
			{
				//Check whether this END matches the type of statement block we are in. If not, register a syntax error complaining before any
				//other error.
				TokenType nextTokType = m_Tokens->at(m_TokenIndex+1)->GetType();
				return statements;
				//ScopeType currentScopeType = m_CurrentScope->GetType();
				if (nextTokType == TokenType::Function)// && currentScopeType != ScopeType::Function)
				{
					RegisterSyntaxError(SyntaxErrorType::unexpEndFunc, m_Tokens->at(m_TokenIndex));
				}
				else if (nextTokType == TokenType::Procedure)// && currentScopeType != ScopeType::Procedure)
				{
					RegisterSyntaxError(SyntaxErrorType::unexpEndProc, m_Tokens->at(m_TokenIndex));
				}
				else if (nextTokType == TokenType::If)// && currentScopeType != ScopeType::If)
				{
					RegisterSyntaxError(SyntaxErrorType::unexpEndIf, m_Tokens->at(m_TokenIndex));
				}
				else if (nextTokType == TokenType::For)// && currentScopeType != ScopeType::For)
				{
					RegisterSyntaxError(SyntaxErrorType::unexpEndFor, m_Tokens->at(m_TokenIndex));
				}
				else if (nextTokType == TokenType::While)// && currentScopeType != ScopeType::While)
				{
					RegisterSyntaxError(SyntaxErrorType::unexpEndWhile, m_Tokens->at(m_TokenIndex));
				}
				else if (nextTokType == TokenType::Struct)// && currentScopeType != ScopeType::Struct)
				{
					RegisterSyntaxError(SyntaxErrorType::unexpEndStruct, m_Tokens->at(m_TokenIndex));
				}
				else
				{
					return statements;
				}
			}
			else if (currTokType == TokenType::Else)
			{
				TokenType nextTokType = m_Tokens->at(m_TokenIndex + 1)->GetType();
				return statements;
				//ScopeType currentScopeType = m_CurrentScope->GetType();
				if (nextTokType == TokenType::If)// && currentScopeType != ScopeType::If)
				{
					RegisterSyntaxError(SyntaxErrorType::unexpElseIf, m_Tokens->at(m_TokenIndex));
				}
				else
				{
					return statements;
				}
			}

			//IncIndex();
			//Attempt to construct statement
			std::shared_ptr<Statement> s = ConstructStatement();
			if (s != nullptr)
			{
				statements->statements.push_back(s);
			}
			else
			{
				//If we didn't get a statement back, then there was an error and we are finished, 
				//as we do not know what is supposed to happen here.
				break;
			}
		}
		return statements;
	}

	std::shared_ptr<Statement> Parser::ConstructStatement()
	{
		std::shared_ptr<Statement> s = std::make_shared<Statement>();
		switch (m_Tokens->at(m_TokenIndex)->GetType())
		{
		case TokenType::If: s->ifNode = ConstructIf(); s->statementType = 0; break;
		case TokenType::Set: s->setNode = ConstructSet(); s->statementType = 1; break;
		case TokenType::Input: s->inputNode = ConstructInput(); s->statementType = 2; break;
		case TokenType::Output: s->outputNode = ConstructOutput(); s->statementType = 3; break;
		case TokenType::Increment: s->incNode = ConstructIncrement(); s->statementType = 4; break;
		case TokenType::Decrement: s->decNode = ConstructDecrement(); s->statementType = 5; break;
		case TokenType::For: s->forNode = ConstructFor(); s->statementType = 6; break;
		case TokenType::While: s->whileNode = ConstructWhile(); s->statementType = 7; break;
		case TokenType::Quit: s->quitNode = ConstructQuit(); s->statementType = 8; break;
		case TokenType::Call: s->callNode = ConstructCall(); s->statementType = 9; break;
		case TokenType::Function: s->funcNode = ConstructFunction(); s->statementType = 10; break;
		case TokenType::Procedure: s->procNode = ConstructProcedure(); s->statementType = 11; break;
		case TokenType::Struct: s->structNode = ConstructStruct(); s->statementType = 12; break;
		case TokenType::Return: s->returnNode = ConstructReturn(); s->statementType = 13; break;
		default:
			RegisterSyntaxError(SyntaxErrorType::expStatement, m_Tokens->at(m_TokenIndex));
			return nullptr;
		}
		return s;
	}

	std::shared_ptr<IfNode> Parser::ConstructIf()
	{
		std::shared_ptr<IfNode> node = std::make_shared<IfNode>();
		IncIndex();

		node->conditions.push_back(ConstructBinOpNode());
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
		{
			//There must be a "DO"
			RegisterSyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		//m_ScopeStack.push_back(m_CurrentScope->AddSubScope("IF_line_"+std::to_string(m_Tokens->at(m_TokenIndex)->GetLineNumber()), m_Tokens->at(m_TokenIndex)->GetLineNumber(), ScopeType::If));
		//m_CurrentScope = m_ScopeStack.back();

		node->bodies.push_back(ConstructStatements());

		//m_CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
		//m_ScopeStack.pop_back();
		//m_CurrentScope = m_ScopeStack.back();

		while (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Else)
		{
			if (node->elsePresent)
			{
				//We cannot have more than one else
				RegisterSyntaxError(SyntaxErrorType::tooManyElse, m_Tokens->at(m_TokenIndex));
			}
			IncIndex();
			if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::If)
			{
				IncIndex();
				node->conditions.push_back(ConstructBinOpNode());
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
				{
					//There must be a "DO"
					RegisterSyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex));
				}
			}
			else if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
			{
				//There must be a "DO"
				RegisterSyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex));
			}
			else
			{
				node->elsePresent = true;
			}
			IncIndex();
			//m_ScopeStack.push_back(m_CurrentScope->AddSubScope("IF_line_" + std::to_string(m_Tokens->at(m_TokenIndex)->GetLineNumber()), m_Tokens->at(m_TokenIndex)->GetLineNumber(), ScopeType::If));
			//m_CurrentScope = m_ScopeStack.back();

			node->bodies.push_back(ConstructStatements());

			//m_CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
			//m_ScopeStack.pop_back();
			//m_CurrentScope = m_ScopeStack.back();
		}
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex+(uint64_t)1)->GetType() == TokenType::If)
		{
			IncIndex(); IncIndex();
		}
		else
		{
			//Must have an end if - something is wrong here
			RegisterSyntaxError(SyntaxErrorType::expEndIf, m_Tokens->at(m_TokenIndex));
		}
		return node;
	}

	std::shared_ptr<SetNode> Parser::ConstructSet()
	{
		std::shared_ptr<SetNode> node = std::make_shared<SetNode>();
		IncIndex();
		node->id = ConstructIdentNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::To)
		{
			RegisterSyntaxError(SyntaxErrorType::expTo, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		node->list = ConstructList();
		//VarType varType;
		/*switch (node->expr->exprType)
		{
		case 1:
		{
			if (node->expr->listNode->Items.size() > 1 || node->expr->listNode->Items.at(0)->hasRight)
			{
				varType = VarType::List;
			}
			else
			{
				if (node->expr->listNode->Items.at(0)->left->right != nullptr)
				{
					varType = VarType::Bool;
				}
				else
				{
					auto atom = node->expr->listNode->Items.at(0)->left->left->left->left->left->left->right;
					while (atom->expression)
					{
						atom = atom->expression->listNode->Items.at(0)->left->left->left->left->left->left->right;
					}
					if (atom->ident != nullptr)
					{
						varType = m_CurrentScope->FindIdent(atom->ident)->GetSymbolType();
					}
					else if (atom->token->GetType() == TokenType::Int)
					{

					}
				}
			}
		}
		}*/

		//m_CurrentScope->AddEntry(node->id->GetContents(), node->id->GetLineNumber(), node->expr->GetExprReturnType());
		return node;
	}

	std::shared_ptr<InputNode> Parser::ConstructInput()
	{
		std::shared_ptr<InputNode> node = std::make_shared<InputNode>();

		IncIndex();
		auto type = m_Tokens->at(m_TokenIndex)->GetType();
		if (type != TokenType::PositiveTypeMod && type != TokenType::NegativeTypeMod && type != TokenType::NonZeroTypeMod && type != TokenType::Type)
		{
			RegisterSyntaxError(SyntaxErrorType::expTypeMod, m_Tokens->at(m_TokenIndex));
		}
		else if (type == TokenType::Type)
		{
			node->type = ConstructTypeNode();
		}
		else
		{
			node->signSpec = m_Tokens->at(m_TokenIndex);
			IncIndex();
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Type)
			{
				RegisterSyntaxError(SyntaxErrorType::expTypeMod, m_Tokens->at(m_TokenIndex));
			}
			else
			{
				node->type = ConstructTypeNode();
			}
		}
		//IncIndex();

		node->id = ConstructIdentNode();
		//m_CurrentScope->AddEntry(node->id->GetContents(), node->id->GetLineNumber());

		return node;
	}

	std::shared_ptr<OutputNode> Parser::ConstructOutput()
	{
		std::shared_ptr<OutputNode> node = std::make_shared<OutputNode>();
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Raw)
		{
			RegisterSyntaxError(SyntaxErrorType::expRaw, m_Tokens->at(m_TokenIndex));
		}
		else
		{
			node->raw = m_Tokens->at(m_TokenIndex);
			IncIndex();
		}
		return node;
	}

	std::shared_ptr<IncNode> Parser::ConstructIncrement()
	{
		std::shared_ptr<IncNode> node = std::make_shared<IncNode>();
		IncIndex();
		node->id = ConstructIdentNode();
		return node;
	}

	std::shared_ptr<DecNode> Parser::ConstructDecrement()
	{
		std::shared_ptr<DecNode> node = std::make_shared<DecNode>();
		IncIndex();
		node->id = ConstructIdentNode();
		return node;
	}

	std::shared_ptr<ForNode> Parser::ConstructFor()
	{
		std::shared_ptr<ForNode> node = std::make_shared<ForNode>();
		IncIndex();

		//m_ScopeStack.push_back(m_CurrentScope->AddSubScope("FOR_line_" + std::to_string(m_Tokens->at(m_TokenIndex)->GetLineNumber()), m_Tokens->at(m_TokenIndex)->GetLineNumber(), ScopeType::For));
		//m_CurrentScope = m_ScopeStack.back();
		node->id = ConstructIdentNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Of)
		{
			RegisterSyntaxError(SyntaxErrorType::expOf, m_Tokens->at(m_TokenIndex));
		}

		node->iterableExpr = ConstructList();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
		{
			RegisterSyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex));
		}
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
		{
			RegisterSyntaxError(SyntaxErrorType::expNewline, m_Tokens->at(m_TokenIndex));
		}

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::For)
		{
			//CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
			IncIndex(); IncIndex();
		}
		else
		{
			//CurrentScope->CloseScope(0);
			RegisterSyntaxError(SyntaxErrorType::expEndFor, m_Tokens->at(m_TokenIndex));
		}

		//ScopeStack.pop_back();
		//CurrentScope = m_ScopeStack.back();
		return node;
	}

	std::shared_ptr<WhileNode> Parser::ConstructWhile()
	{
		std::shared_ptr<WhileNode> node = std::make_shared<WhileNode>();
		IncIndex();

		//m_ScopeStack.push_back(m_CurrentScope->AddSubScope("WHILE_line_" + std::to_string(m_Tokens->at(m_TokenIndex)->GetLineNumber()), m_Tokens->at(m_TokenIndex)->GetLineNumber(), ScopeType::While));
		//m_CurrentScope = m_ScopeStack.back();

		node->condition = ConstructBinOpNode();
		
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
		{
			RegisterSyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex));
		}
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
		{
			RegisterSyntaxError(SyntaxErrorType::expNewline, m_Tokens->at(m_TokenIndex));
		}

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::While)
		{
			//m_CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
			IncIndex(); IncIndex();
		}
		else
		{
			//m_CurrentScope->CloseScope(0);
			RegisterSyntaxError(SyntaxErrorType::expEndWhile, m_Tokens->at(m_TokenIndex));
		}

		//m_ScopeStack.pop_back();
		//m_CurrentScope = m_ScopeStack.back();
		return node;
	}

	std::shared_ptr<QuitNode> Parser::ConstructQuit()
	{
		std::shared_ptr<QuitNode> node = std::make_shared<QuitNode>();
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
			node->returnVal = ConstructAtom();
		return node;
	}

	std::shared_ptr<CallNode> Parser::ConstructCall()
	{
		std::shared_ptr<CallNode> node = std::make_shared<CallNode>();
		IncIndex();
		node->function = ConstructAtom();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
		{
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::With)
			{
				RegisterSyntaxError(SyntaxErrorType::expWith, m_Tokens->at(m_TokenIndex));
			}
			else
				IncIndex();

			node->args.push_back(ConstructAtom());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex));
				}
				else
					IncIndex();

				node->args.push_back(ConstructAtom());
			}
		}
		return node;
	}

	std::shared_ptr<FuncNode> Parser::ConstructFunction()
	{
		std::shared_ptr<FuncNode> node = std::make_shared<FuncNode>();
		IncIndex();

		node->id = ConstructIdentNode();

		//m_CurrentScope->AddEntry(node->id->GetContents(), node->id->GetLineNumber()); 
		//m_ScopeStack.push_back(m_CurrentScope->AddSubScope(node->id->GetContents(), node->id->GetLineNumber(), ScopeType::Function));
		//m_CurrentScope = m_ScopeStack.back();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Takes)
		{
			IncIndex();
			node->argTypes.push_back(ConstructTypeNode());
			node->argNames.push_back(ConstructIdentNode());
			//m_CurrentScope->AddEntry(node->argNames.back()->GetContents(), node->argNames.back()->GetLineNumber());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex));
				}
				else
					IncIndex();
				node->argTypes.push_back(ConstructTypeNode());
				node->argNames.push_back(ConstructIdentNode());
				//m_CurrentScope->AddEntry(node->argNames.back()->GetContents(), node->argNames.back()->GetLineNumber());
			}
		}

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
		{
			RegisterSyntaxError(SyntaxErrorType::expReturns, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		node->returnType = ConstructTypeNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::As)
		{
			RegisterSyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::Function)
		{
			//m_CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
			IncIndex(); IncIndex();
		}
		else
		{
			RegisterSyntaxError(SyntaxErrorType::expEndFunc, m_Tokens->at(m_TokenIndex));
			//m_CurrentScope->CloseScope(0);
		}
		
		//m_ScopeStack.pop_back();
		//m_CurrentScope = m_ScopeStack.back();

		return node;
	}
	
	std::shared_ptr<ProcNode> Parser::ConstructProcedure()
	{
		std::shared_ptr<ProcNode> node = std::make_shared<ProcNode>();
		IncIndex();

		node->id = ConstructIdentNode();

		//m_CurrentScope->AddEntry(node->id->GetContents(), node->id->GetLineNumber());
		//m_ScopeStack.push_back(m_CurrentScope->AddSubScope(node->id->GetContents(), node->id->GetLineNumber(), ScopeType::Procedure));
		//m_CurrentScope = m_ScopeStack.back();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
		{
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Takes)
			{
				RegisterSyntaxError(SyntaxErrorType::expReturns, m_Tokens->at(m_TokenIndex));
			}
			else
				IncIndex();
			node->argTypes.push_back(ConstructTypeNode());
			node->argNames.push_back(ConstructIdentNode());
			//m_CurrentScope->AddEntry(node->argNames.back()->GetContents(), node->argNames.back()->GetLineNumber());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex));
				}
				else
					IncIndex();
				node->argTypes.push_back(ConstructTypeNode());
				node->argNames.push_back(ConstructIdentNode());
				//m_CurrentScope->AddEntry(node->argNames.back()->GetContents(), node->argNames.back()->GetLineNumber());
			}
		}

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::As)
		{
			RegisterSyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::Function)
		{
			//m_CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
			IncIndex(); IncIndex();
		}
		else
		{
			//m_CurrentScope->CloseScope(0);
			RegisterSyntaxError(SyntaxErrorType::expEndProc, m_Tokens->at(m_TokenIndex));
		}

		//m_ScopeStack.pop_back();
		//m_CurrentScope = m_ScopeStack.back();
		return node;
	}

	std::shared_ptr<StructNode> Parser::ConstructStruct()
	{
		std::shared_ptr<StructNode> node = std::make_shared<StructNode>();
		IncIndex();

		node->id = ConstructIdentNode();

		//m_CurrentScope->AddEntry(node->id->GetContents(), node->id->GetLineNumber());
		//m_ScopeStack.push_back(m_CurrentScope->AddSubScope(node->id->GetContents(), m_Tokens->at(m_TokenIndex)->GetLineNumber(), ScopeType::If));
		//m_CurrentScope = m_ScopeStack.back();

		

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::As)
		{
			RegisterSyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
		{
			RegisterSyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		node->types.push_back(ConstructTypeNode());

		node->names.push_back(ConstructIdentNode());

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Newline)
		{
			IncIndex();
		}

		while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::End)
		{
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
			{
				RegisterSyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex));
			}
			else
				IncIndex();

			if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Newline)
			{
				IncIndex();
			}

			node->types.push_back(ConstructTypeNode());

			node->names.push_back(ConstructIdentNode());

			if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Newline)
			{
				IncIndex();
			}
		}
		
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::Struct)
		{
			//m_CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
			IncIndex(); IncIndex();
		}
		else
		{
			//m_CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
			RegisterSyntaxError(SyntaxErrorType::expEndStruct, m_Tokens->at(m_TokenIndex));
		}
		
		//m_ScopeStack.pop_back();
		//m_CurrentScope = m_ScopeStack.back();
		return node;
	}

	std::shared_ptr<Expr> Parser::ConstructExpr()
	{
		std::shared_ptr<Expr> node = std::make_shared<Expr>();
		switch (m_Tokens->at(m_TokenIndex)->GetType())
		{
		case TokenType::Create:
			node->createNode = ConstructCreate();
			node->exprType = 2; break;
		case TokenType::Cast:
			node->castNode = ConstructCast();
			node->exprType = 3; break;
		case TokenType::Call:
			node->callNode = ConstructCall();
			node->exprType = 4; break;
		case TokenType::AnonFunc:
			node->anonfNode = ConstructAnonFunc();
			node->exprType = 5; break;
		case TokenType::AnonProc:
			node->anonpNode = ConstructAnonProc();
			node->exprType = 6; break;
		default:
			node->binOpNode = ConstructBinOpNode();
			node->exprType = 1; break;
		}
		return node;
	}

	std::shared_ptr<ListNode> Parser::ConstructList()
	{
		std::shared_ptr<ListNode> node = std::make_shared<ListNode>();
		uint32_t lineNumber = m_Tokens->at(m_TokenIndex)->GetLineNumber();

		node->Items.push_back(ConstructDictEntry());
		while (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Comma)
		{
			IncIndex();
			if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Newline)
				IncIndex();
			node->Items.push_back(ConstructDictEntry());
			if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Newline)
				IncIndex();
		}

		int dictPairs = -1;
		for (auto dictNode : node->Items)
		{
			if (dictNode->hasRight && dictPairs == 0)
				RegisterSyntaxError(SyntaxErrorType::inconsistentDict, lineNumber, 0, 10, TokenType::None);
			else if (dictNode->hasRight)
				dictPairs = 1;
			else if (dictPairs == 1)
				RegisterSyntaxError(SyntaxErrorType::inconsistentDict, lineNumber, 0, 10, TokenType::None);
			else
				dictPairs = 0;
		}

		return node;
	}

	std::shared_ptr<DictEntryNode> Parser::ConstructDictEntry()
	{
		std::shared_ptr<DictEntryNode> node = std::make_shared<DictEntryNode>();
		node->left = ConstructExpr();
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::DictEquator)
		{
			IncIndex();
			node->right = ConstructExpr();
			node->hasRight = true;
		}
		else
			node->hasRight = false;

		return node;
	}

	/*
	std::shared_ptr<BoolExprNode> Parser::ConstructBooleanExpr()
	{
		std::shared_ptr<BoolExprNode> node = std::make_shared<BoolExprNode>();
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Not)
		{
			node->exprType = 1;
			IncIndex();
			node->right = ConstructBooleanExpr();
		}
		else
		{
			
			node->left = ConstructAddExpr();
			TokenType type = m_Tokens->at(m_TokenIndex)->GetType();
			if (type == TokenType::NotEqual || type == TokenType::Equal || type == TokenType::LessThan || type == TokenType::GreaterThan || type == TokenType::LessThanEqual || type == TokenType::GreaterThanEqual)
			{
				node->exprType = 2;
				node->opToken = m_Tokens->at(m_TokenIndex);
				IncIndex();
				node->right = ConstructBooleanExpr();
			}
			else
			{
				node->exprType = 3;
			}
		}
		return node;
	}

	std::shared_ptr<AddExprNode> Parser::ConstructAddExpr()
	{
		std::shared_ptr<AddExprNode> node = std::make_shared<AddExprNode>();
		node->left = ConstructMulExpr();
		TokenType type = m_Tokens->at(m_TokenIndex)->GetType();
		if (type == TokenType::Plus || type == TokenType::Minus)
		{
			node->opToken = m_Tokens->at(m_TokenIndex);
			IncIndex();
			node->right = ConstructAddExpr();
		}
		return node;
	}

	std::shared_ptr<MulExprNode> Parser::ConstructMulExpr()
	{
		std::shared_ptr<MulExprNode> node = std::make_shared<MulExprNode>();
		node->left = ConstructDivModExpr();
		TokenType type = m_Tokens->at(m_TokenIndex)->GetType();
		if (type == TokenType::Multiply || type == TokenType::Divide)
		{
			node->opToken = m_Tokens->at(m_TokenIndex);
			IncIndex();
			node->right = ConstructMulExpr();
		}
		return node;
	}

	std::shared_ptr<DivModExprNode> Parser::ConstructDivModExpr()
	{
		std::shared_ptr<DivModExprNode> node = std::make_shared<DivModExprNode>();
		node->left = ConstructPower();
		TokenType type = m_Tokens->at(m_TokenIndex)->GetType();
		if (type == TokenType::Intdiv || type == TokenType::Modulo)
		{
			node->opToken = m_Tokens->at(m_TokenIndex);
			IncIndex();
			node->right = ConstructDivModExpr();
		}
		return node;
	}

	std::shared_ptr<PowerNode> Parser::ConstructPower()
	{
		std::shared_ptr<PowerNode> node = std::make_shared<PowerNode>();
		node->left = ConstructFactor();
		TokenType type = m_Tokens->at(m_TokenIndex)->GetType();
		if (type == TokenType::Power)
		{
			node->opToken = m_Tokens->at(m_TokenIndex);
			IncIndex();
			node->right = ConstructPower();
		}
		return node;
	}
	*/

	std::shared_ptr<BinOpNode> Parser::ConstructBinOpNode()
	{
		std::shared_ptr<BinOpNode> node = std::make_shared<BinOpNode>();
		node->left = ConstructFactor();
		std::vector<TokenType> acceptedTypes = { TokenType::And, TokenType::Or, TokenType::NotEqual, TokenType::Equal, TokenType::GreaterThan,
			TokenType::GreaterThanEqual, TokenType::LessThan , TokenType::LessThanEqual, TokenType::Plus, TokenType::Minus, TokenType::Modulo,
			TokenType::Multiply, TokenType::Divide, TokenType::Intdiv, TokenType::Power, TokenType::BitwiseAnd, TokenType::BitwiseOr};

		if (itemInVect(acceptedTypes, m_Tokens->at(m_TokenIndex)->GetType()))
		{
			node->opToken = m_Tokens->at(m_TokenIndex);
			IncIndex();
			node->right = ConstructBinOpNode();
		}
		return node;
	}

	std::shared_ptr<FactorNode> Parser::ConstructFactor()
	{
		std::shared_ptr<FactorNode> node = std::make_shared<FactorNode>();
		std::vector<TokenType> acceptedTypes = { TokenType::Plus, TokenType::Minus, TokenType::Not };
		if (itemInVect(acceptedTypes, m_Tokens->at(m_TokenIndex)->GetType()))
		{
			node->opToken = m_Tokens->at(m_TokenIndex);
			IncIndex();
			node->right = ConstructAtom();
			node->opTokenPresent = true;
		}
		else
		{
			node->right = ConstructAtom();
			node->opTokenPresent = false;
		}
		return node;
	}

	std::shared_ptr<AtomNode> Parser::ConstructAtom()
	{
		std::shared_ptr<AtomNode> node = std::make_shared<AtomNode>();
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::LParen)
		{
			IncIndex();
			node->list = ConstructList();
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::RParen)
			{
				RegisterSyntaxError(SyntaxErrorType::expRParen, m_Tokens->at(m_TokenIndex));
			}
			node->type = 2;
		}
		else
		{
			std::vector<TokenType> acceptedAtomTokenTypes = { TokenType::String, TokenType::Raw, TokenType::Int, TokenType::Float, TokenType::Complex, TokenType::Identifier };
			TokenType type = m_Tokens->at(m_TokenIndex)->GetType();
			if (!itemInVect(acceptedAtomTokenTypes, type))
			{
				//If it doesn't start with an ident, then it isn't an identifier, and if it isn't any of the others, then it must be an error
				RegisterSyntaxError(SyntaxErrorType::expAtom, m_Tokens->at(m_TokenIndex));
				node->type = 0;
			}
			else if (type == TokenType::Identifier)
			{
				//std::shared_ptr<IdentNode> ident = ConstructIdentNode();
				node->ident = ConstructIdentNode();
				node->type = 3;
				//@IMPORTANT How are global variables managed? Are all variables declared in the main scope treated as globals, or do
				//           they need to be specially declared and placed in a global scope? main scope = global scope, scope state -> global variables
				// it is NOT an error to reference a variable from a previous scope before defining it in the current scope - different to python behaviour 
				// w/ functions
				/*if (m_CurrentScope->FindIdent(ident) || m_MainScope->FindIdent(ident))
				{
					node->ident = ident;
					node->type = 3;
				}
				else
				{
					//RegisterMissingVariable(ident->GetLineNumber(), ident->GetColumnNumber());
					node->type = 0;
				}*/
			}
			else
			{
				node->token = m_Tokens->at(m_TokenIndex);
				node->type = 1;
				IncIndex();
			}
		}

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::LSquareParen)
		{
			node->listAccess = ConstructListAccess();
			node->listAccessPresent = true;
		}
		return node;
	}

	std::shared_ptr<ListAccessNode> Parser::ConstructListAccess()
	{
		std::shared_ptr<ListAccessNode> node = std::make_shared<ListAccessNode>();
		IncIndex();

		node->indices.push_back(ConstructList());
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::RSquareParen)
			IncIndex();
		else
			RegisterSyntaxError(SyntaxErrorType::expRSquareParen, m_Tokens->at(m_TokenIndex));

		while (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::LSquareParen)
		{
			node->indices.push_back(ConstructList());
			if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::RSquareParen)
				IncIndex();
			else
				RegisterSyntaxError(SyntaxErrorType::expRSquareParen, m_Tokens->at(m_TokenIndex));
		}

		return std::shared_ptr<ListAccessNode>();
	}

	std::shared_ptr<CreateNode> Parser::ConstructCreate()
	{
		std::shared_ptr<CreateNode> node = std::make_shared<CreateNode>();
		IncIndex();
		node->createType = ConstructTypeNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::With)
		{
			node->args.push_back(ConstructAtom());
			while (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Comma)
			{
				node->args.push_back(ConstructAtom());
			}
		}
		return node;
	}

	std::shared_ptr<CastNode> Parser::ConstructCast()
	{
		std::shared_ptr<CastNode> node = std::make_shared<CastNode>();
		IncIndex();
		node->castType = ConstructTypeNode();
		node->list = ConstructList();
		return node;
	}

	std::shared_ptr<ReturnNode> Parser::ConstructReturn()
	{
		std::shared_ptr<ReturnNode> node = std::make_shared<ReturnNode>();
		IncIndex();
		node->list = ConstructList();
		return node;
	}

	std::shared_ptr<AnonfNode> Parser::ConstructAnonFunc()
	{
		std::shared_ptr<AnonfNode> node = std::make_shared<AnonfNode>();
		IncIndex();

		//m_ScopeStack.push_back(m_CurrentScope->AddSubScope("ANONF_line_" + std::to_string(m_Tokens->at(m_TokenIndex)->GetLineNumber()), m_Tokens->at(m_TokenIndex)->GetLineNumber(), ScopeType::Anonf));
		//m_CurrentScope = m_ScopeStack.back();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Takes)
		{
			IncIndex();
			node->argTypes.push_back(ConstructTypeNode());
			node->argNames.push_back(ConstructIdentNode());
			//m_CurrentScope->AddEntry(node->argNames.back()->GetContents(), node->argNames.back()->GetLineNumber());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex));
				}
				else
					IncIndex();
				node->argTypes.push_back(ConstructTypeNode());
				node->argNames.push_back(ConstructIdentNode());
				//m_CurrentScope->AddEntry(node->argNames.back()->GetContents(), node->argNames.back()->GetLineNumber());
			}
		}

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
		{
			RegisterSyntaxError(SyntaxErrorType::expReturns, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		node->returnType = ConstructTypeNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::As)
		{
			RegisterSyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::Function)
		{
			//m_CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
			IncIndex(); IncIndex();
		}
		else
		{
			//m_CurrentScope->CloseScope(0);
			RegisterSyntaxError(SyntaxErrorType::expEndFunc, m_Tokens->at(m_TokenIndex));
		}

		//m_ScopeStack.pop_back();
		//m_CurrentScope = m_ScopeStack.back();
		return node;
	}

	std::shared_ptr<AnonpNode> Parser::ConstructAnonProc()
	{
		std::shared_ptr<AnonpNode> node = std::make_shared<AnonpNode>();
		IncIndex();

		//m_ScopeStack.push_back(m_CurrentScope->AddSubScope("ANONP_line_" + std::to_string(m_Tokens->at(m_TokenIndex)->GetLineNumber()), m_Tokens->at(m_TokenIndex)->GetLineNumber(), ScopeType::Anonp));
		//m_CurrentScope = m_ScopeStack.back();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
		{
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Takes)
			{
				RegisterSyntaxError(SyntaxErrorType::expReturns, m_Tokens->at(m_TokenIndex));
			}
			else
				IncIndex();

			node->argTypes.push_back(ConstructTypeNode());

			node->argNames.push_back(ConstructIdentNode());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex));
				}
				else
					IncIndex();

				node->argTypes.push_back(ConstructTypeNode());

				node->argNames.push_back(ConstructIdentNode());

			}
		}

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::As)
		{
			RegisterSyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex));
		}
		else
			IncIndex();

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::Function)
		{
			//m_CurrentScope->CloseScope(m_Tokens->at(m_TokenIndex)->GetLineNumber());
			IncIndex(); IncIndex();
		}
		else
		{
			//m_CurrentScope->CloseScope(0);
			RegisterSyntaxError(SyntaxErrorType::expEndFunc, m_Tokens->at(m_TokenIndex));
		}

		//m_ScopeStack.pop_back();
		//m_CurrentScope = m_ScopeStack.back();
		return node;
	}

	std::shared_ptr<TypeNode> Parser::ConstructTypeNode()
	{
		std::shared_ptr<TypeNode> node = std::make_shared<TypeNode>();
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Type)
		{
			node->typeToken = m_Tokens->at(m_TokenIndex);
			node->type = 2;
			IncIndex();
		}
		else
		{
			node->ident = ConstructIdentNode();
			node->type = 1;
		}
		return node;
	}

	std::shared_ptr<IdentNode> Parser::ConstructIdentNode()
	{
		std::shared_ptr<IdentNode> node = std::make_shared<IdentNode>();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Identifier)
		{
			RegisterSyntaxError(SyntaxErrorType::expIdent, m_Tokens->at(m_TokenIndex));
		}
		else
		{
			node->ids.push_back(m_Tokens->at(m_TokenIndex));
			IncIndex();
		}

		while (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::VarAccessOp)
		{
			IncIndex();
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Identifier)
			{
				RegisterSyntaxError(SyntaxErrorType::expIdent, m_Tokens->at(m_TokenIndex));
			}
			else
			{
				node->ids.push_back(m_Tokens->at(m_TokenIndex));
				IncIndex();
			}
		}
		return node;
	}
}
