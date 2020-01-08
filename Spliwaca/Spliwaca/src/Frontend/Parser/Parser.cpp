#include "Parser.h"
#include "UtilFunctions.h"

namespace Spliwaca
{
	std::shared_ptr<Parser> Parser::Create(std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens)
	{
		return std::shared_ptr<Parser>(new Parser(tokens));
	}

	std::shared_ptr<EntryPoint> Parser::ConstructAST()
	{
		std::shared_ptr<EntryPoint> ep = std::shared_ptr<EntryPoint>();
		//Begin loop through tokens
		ep->require = ConstructRequire();

		// Consume newline after require
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expNewline, m_Tokens->at(m_TokenIndex)));
		}
		else
			IncIndex(); 

		//Begin constructing statements
		ep->statements = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::eof)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expNewline, m_Tokens->at(m_TokenIndex)));
			return ep;
		}
		else
			return ep;
	}

	std::shared_ptr<RequireNode> Parser::ConstructRequire()
	{
		std::shared_ptr<RequireNode> node = std::shared_ptr<RequireNode>();
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
		std::shared_ptr<Statements> statements = std::shared_ptr<Statements>();
		while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::eof)
		{
			//Attempt to consume newline
			while (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Newline)
			{
				IncIndex();
			}

			//If we have reached the end of the file, return
			TokenType currTokType = m_Tokens->at(m_TokenIndex)->GetType();
			if (currTokType == TokenType::eof || currTokType == TokenType::End || currTokType == TokenType::Else)
				break;

			IncIndex();
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
		std::shared_ptr<Statement> s = std::shared_ptr<Statement>();
		switch (m_Tokens->at(m_TokenIndex)->GetType())
		{
		case TokenType::If: s->ifNode = ConstructIf(); s->statementType = 0;
		case TokenType::Set: s->setNode = ConstructSet(); s->statementType = 1;
		case TokenType::Input: s->inputNode = ConstructInput(); s->statementType = 2;
		case TokenType::Output: s->outputNode = ConstructOutput(); s->statementType = 3;
		case TokenType::Increment: s->incNode = ConstructIncrement(); s->statementType = 4;
		case TokenType::Decrement: s->decNode = ConstructDecrement(); s->statementType = 5;
		case TokenType::For: s->forNode = ConstructFor(); s->statementType = 6;
		case TokenType::While: s->whileNode = ConstructWhile(); s->statementType = 7;
		case TokenType::Quit: s->quitNode = ConstructQuit(); s->statementType = 8;
		case TokenType::Function: s->funcNode = ConstructFunction(); s->statementType = 9;
		case TokenType::Procedure: s->procNode = ConstructProcedure(); s->statementType = 10;
		default:
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expStatement, m_Tokens->at(m_TokenIndex)));
			return nullptr;
		}
		return s;
	}

	std::shared_ptr<IfNode> Parser::ConstructIf()
	{
		std::shared_ptr<IfNode> node = std::shared_ptr<IfNode>();
		IncIndex();
		node->conditions.push_back(ConstructBooleanExpr());
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
		{
			//There must be a "DO"
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex)));
		}
		node->bodies.push_back(ConstructStatements());

		while (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Else)
		{
			if (node->elsePresent)
			{
				//We cannot have more than one else
				RegisterSyntaxError(SyntaxError(SyntaxErrorType::tooManyElse, m_Tokens->at(m_TokenIndex)));
			}
			IncIndex();
			if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::If)
			{
				IncIndex();
				node->conditions.push_back(ConstructBooleanExpr());
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
				{
					//There must be a "DO"
					RegisterSyntaxError(SyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex)));
				}
			}
			else if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
			{
				//There must be a "DO"
				RegisterSyntaxError(SyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex)));
			}
			else
			{
				node->elsePresent = true;
			}
			IncIndex();
			node->bodies.push_back(ConstructStatements());
		}
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex+(uint64_t)1)->GetType() == TokenType::If)
		{
			IncIndex(); IncIndex();
		}
		else
		{
			//Must have an end if - something is wrong here
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expEndIf, m_Tokens->at(m_TokenIndex)));
		}
		return node;
	}

	std::shared_ptr<SetNode> Parser::ConstructSet()
	{
		std::shared_ptr<SetNode> node = std::shared_ptr<SetNode>();

		IncIndex();

		node->id = ConstructIdentNode();
		IncIndex();
		

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::To)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expTo, m_Tokens->at(m_TokenIndex)));
		}
		else
			IncIndex();

		node->expr = ConstructExpr();

		return node;
	}

	std::shared_ptr<InputNode> Parser::ConstructInput()
	{
		std::shared_ptr<InputNode> node = std::shared_ptr<InputNode>();

		IncIndex();
		auto type = m_Tokens->at(m_TokenIndex)->GetType();
		if (type != TokenType::PositiveTypeMod && type != TokenType::NegativeTypeMod && type != TokenType::NonZeroTypeMod && type != TokenType::Type)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expTypeMod, m_Tokens->at(m_TokenIndex)));
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
				RegisterSyntaxError(SyntaxError(SyntaxErrorType::expTypeMod, m_Tokens->at(m_TokenIndex)));
			}
			else
			{
				node->type = m_Tokens->at(m_TokenIndex);
			}
		}
		IncIndex();

		node->id = ConstructIdentNode();

		return node;
	}

	std::shared_ptr<OutputNode> Parser::ConstructOutput()
	{
		std::shared_ptr<OutputNode> node = std::shared_ptr<OutputNode>();
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Raw)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expRaw, m_Tokens->at(m_TokenIndex)));
		}
		else
		{
			node->raw = m_Tokens->at(m_TokenIndex);
		}
		return node;
	}

	std::shared_ptr<IncNode> Parser::ConstructIncrement()
	{
		std::shared_ptr<IncNode> node = std::shared_ptr<IncNode>();
		IncIndex();
		node->id = ConstructIdentNode();
		return node;
	}

	std::shared_ptr<DecNode> Parser::ConstructDecrement()
	{
		std::shared_ptr<DecNode> node = std::shared_ptr<DecNode>();
		IncIndex();
		node->id = ConstructIdentNode();
		return node;
	}

	std::shared_ptr<ForNode> Parser::ConstructFor()
	{
		std::shared_ptr<ForNode> node = std::shared_ptr<ForNode>();
		IncIndex();
		node->id = ConstructIdentNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Of)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expOf, m_Tokens->at(m_TokenIndex)));
		}

		node->iterableExpr = ConstructExpr();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex)));
		}
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expNewline, m_Tokens->at(m_TokenIndex)));
		}

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::For)
		{
			IncIndex(); IncIndex();
		}
		else
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expEndFor, m_Tokens->at(m_TokenIndex)));
		}

		return node;
	}

	std::shared_ptr<WhileNode> Parser::ConstructWhile()
	{
		std::shared_ptr<WhileNode> node = std::shared_ptr<WhileNode>();
		IncIndex();
		node->condition = ConstructBooleanExpr();
		
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Do)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expDo, m_Tokens->at(m_TokenIndex)));
		}
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expNewline, m_Tokens->at(m_TokenIndex)));
		}

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::While)
		{
			IncIndex(); IncIndex();
		}
		else
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expEndWhile, m_Tokens->at(m_TokenIndex)));
		}

		return node;
	}

	std::shared_ptr<QuitNode> Parser::ConstructQuit()
	{
		std::shared_ptr<QuitNode> node = std::shared_ptr<QuitNode>();
		IncIndex();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
			node->returnVal = ConstructAtom();
		return node;
	}

	std::shared_ptr<CallNode> Parser::ConstructCall()
	{
		std::shared_ptr<CallNode> node = std::shared_ptr<CallNode>();
		IncIndex();
		node->funcId = ConstructExpr();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
		{
			IncIndex();
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::With)
			{
				RegisterSyntaxError(SyntaxError(SyntaxErrorType::expWith, m_Tokens->at(m_TokenIndex)));
			}
			else
				IncIndex();

			node->args.push_back(ConstructExpr());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Newline)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex)));
				}
				else
					IncIndex();

				node->args.push_back(ConstructExpr());
			}
		}
		return node;
	}

	std::shared_ptr<FuncNode> Parser::ConstructFunction()
	{
		std::shared_ptr<FuncNode> node = std::shared_ptr<FuncNode>();
		IncIndex();

		node->id = ConstructIdentNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Takes)
		{
			IncIndex();

			node->argTypes.push_back(ConstructTypeNode());

			node->argNames.push_back(ConstructIdentNode());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex)));
				}
				else
					IncIndex();

				node->argTypes.push_back(ConstructTypeNode());

				node->argNames.push_back(ConstructIdentNode());
			}
		}

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expReturns, m_Tokens->at(m_TokenIndex)));
		}
		else
			IncIndex();

		node->returnType = ConstructTypeNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::As)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex)));
		}
		else
			IncIndex();

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::Function)
		{
			IncIndex(); IncIndex();
		}
		else
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expEndFunc, m_Tokens->at(m_TokenIndex)));
		}
		return node;
	}
	
	std::shared_ptr<ProcNode> Parser::ConstructProcedure()
	{
		std::shared_ptr<ProcNode> node = std::shared_ptr<ProcNode>();
		IncIndex();

		node->id = ConstructIdentNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
		{
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Takes)
			{
				RegisterSyntaxError(SyntaxError(SyntaxErrorType::expReturns, m_Tokens->at(m_TokenIndex)));
			}
			else
				IncIndex();

			node->argTypes.push_back(ConstructTypeNode());

			node->argNames.push_back(ConstructIdentNode());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex)));
				}
				else
					IncIndex();

				node->argTypes.push_back(ConstructTypeNode());

				node->argNames.push_back(ConstructIdentNode());

			}
		}

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::As)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex)));
		}
		else
			IncIndex();

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::Function)
		{
			IncIndex(); IncIndex();
		}
		else
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expEndFunc, m_Tokens->at(m_TokenIndex)));
		}
		return node;
	}

	std::shared_ptr<Expr> Parser::ConstructExpr()
	{
		std::shared_ptr<Expr> node = std::shared_ptr<Expr>();
		switch (m_Tokens->at(m_TokenIndex)->GetType())
		{
		case TokenType::Create:
			node->createNode = ConstructCreate();
			node->exprType = 2;
		case TokenType::Cast:
			node->castNode = ConstructCast();
			node->exprType = 3;
		case TokenType::Call:
			node->callNode = ConstructCall();
			node->exprType = 4;
		case TokenType::AnonFunc:
			node->anonfNode = ConstructAnonFunc();
			node->exprType = 5;
		case TokenType::AnonProc:
			node->anonpNode = ConstructAnonProc();
			node->exprType = 6;
		default:
			node->listNode = ConstructList();
			node->exprType = 1;
		}
		return node;
	}

	std::shared_ptr<ListNode> Parser::ConstructList()
	{
		std::shared_ptr<ListNode> node = std::shared_ptr<ListNode>();

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
		return node;
	}

	std::shared_ptr<DictEntryNode> Parser::ConstructDictEntry()
	{
		std::shared_ptr<DictEntryNode> node = std::shared_ptr<DictEntryNode>();
		node->left = ConstructBooleanExpr();
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::DictEquator)
		{
			IncIndex();
			node->right = ConstructBooleanExpr();
			node->hasRight = true;
		}
		else
			node->hasRight = false;

		return node;
	}

	std::shared_ptr<BoolExprNode> Parser::ConstructBooleanExpr()
	{
		std::shared_ptr<BoolExprNode> node = std::shared_ptr<BoolExprNode>();
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
		std::shared_ptr<AddExprNode> node = std::shared_ptr<AddExprNode>();
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
		std::shared_ptr<MulExprNode> node = std::shared_ptr<MulExprNode>();
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
		std::shared_ptr<DivModExprNode> node = std::shared_ptr<DivModExprNode>();
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
		std::shared_ptr<PowerNode> node = std::shared_ptr<PowerNode>();
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

	std::shared_ptr<FactorNode> Parser::ConstructFactor()
	{
		std::shared_ptr<FactorNode> node = std::shared_ptr<FactorNode>();
		TokenType type = m_Tokens->at(m_TokenIndex)->GetType();
		if (type == TokenType::Plus || type == TokenType::Minus)
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
		std::shared_ptr<AtomNode> node = std::shared_ptr<AtomNode>();
		std::shared_ptr<Expr> firstExpr;
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::LCurlyParen)
		{
			IncIndex();
			node->expression = ConstructExpr();
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::RParen)
			{
				RegisterSyntaxError(SyntaxError(SyntaxErrorType::expRParen, m_Tokens->at(m_TokenIndex)));
			}
			node->type = 2;
		}
		else
		{
			std::vector<TokenType> acceptedAtomTokenTypes = { TokenType::String, TokenType::Raw, TokenType::Int, TokenType::Float, TokenType::Complex, TokenType::Identifier };
			TokenType type = m_Tokens->at(m_TokenIndex)->GetType();
			if (!itemInVect(acceptedAtomTokenTypes, m_Tokens->at(m_TokenIndex)->GetType()))
			{
				//If it doesn't start with an ident, then it isn't an identifier, and if it isn't any of the others, then it must be an error
				RegisterSyntaxError(SyntaxError(SyntaxErrorType::expAtom, m_Tokens->at(m_TokenIndex)));
				node->type = 0;
			}
			else if (type == TokenType::Identifier)
			{
				node->ident = ConstructIdentNode();
				node->type = 3;
			}
			else
			{
				node->token = m_Tokens->at(m_TokenIndex);
				node->type = 1;
				IncIndex();
			}
		}
		return node;
	}

	std::shared_ptr<CreateNode> Parser::ConstructCreate()
	{
		std::shared_ptr<CreateNode> node = std::shared_ptr<CreateNode>();
		IncIndex();
		node->createType = ConstructTypeNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::With)
		{
			node->args.push_back(ConstructExpr());
			while (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Comma)
			{
				node->args.push_back(ConstructExpr());
			}
		}
		return node;
	}

	std::shared_ptr<CastNode> Parser::ConstructCast()
	{
		std::shared_ptr<CastNode> node = std::shared_ptr<CastNode>();
		IncIndex();
		node->castType = ConstructTypeNode();
		node->expr = ConstructExpr();
		return node;
	}

	std::shared_ptr<AnonfNode> Parser::ConstructAnonFunc()
	{
		std::shared_ptr<AnonfNode> node = std::shared_ptr<AnonfNode>();
		IncIndex();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Takes)
		{
			IncIndex();

			node->argTypes.push_back(ConstructTypeNode());

			node->argNames.push_back(ConstructIdentNode());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex)));
				}
				else
					IncIndex();

				node->argTypes.push_back(ConstructTypeNode());

				node->argNames.push_back(ConstructIdentNode());
			}
		}

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expReturns, m_Tokens->at(m_TokenIndex)));
		}
		else
			IncIndex();

		node->returnType = ConstructTypeNode();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::As)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex)));
		}
		else
			IncIndex();

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::Function)
		{
			IncIndex(); IncIndex();
		}
		else
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expEndFunc, m_Tokens->at(m_TokenIndex)));
		}
		return node;
	}

	std::shared_ptr<AnonpNode> Parser::ConstructAnonProc()
	{
		std::shared_ptr<AnonpNode> node = std::shared_ptr<AnonpNode>();
		IncIndex();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
		{
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Takes)
			{
				RegisterSyntaxError(SyntaxError(SyntaxErrorType::expReturns, m_Tokens->at(m_TokenIndex)));
			}
			else
				IncIndex();

			node->argTypes.push_back(ConstructTypeNode());

			node->argNames.push_back(ConstructIdentNode());

			while (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::ReturnType)
			{
				if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Comma)
				{
					RegisterSyntaxError(SyntaxError(SyntaxErrorType::expComma, m_Tokens->at(m_TokenIndex)));
				}
				else
					IncIndex();

				node->argTypes.push_back(ConstructTypeNode());

				node->argNames.push_back(ConstructIdentNode());

			}
		}

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::As)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expAs, m_Tokens->at(m_TokenIndex)));
		}
		else
			IncIndex();

		node->body = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::End && m_Tokens->at((uint64_t)m_TokenIndex + (uint64_t)1)->GetType() == TokenType::Function)
		{
			IncIndex(); IncIndex();
		}
		else
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expEndFunc, m_Tokens->at(m_TokenIndex)));
		}
		return node;
	}

	std::shared_ptr<TypeNode> Parser::ConstructTypeNode()
	{
		std::shared_ptr<TypeNode> node = std::shared_ptr<TypeNode>();
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
		std::shared_ptr<IdentNode> node = std::shared_ptr<IdentNode>();
		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Identifier)
		{
			RegisterSyntaxError(SyntaxError(SyntaxErrorType::expIdent, m_Tokens->at(m_TokenIndex)));
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
				RegisterSyntaxError(SyntaxError(SyntaxErrorType::expIdent, m_Tokens->at(m_TokenIndex)));
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
