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
			RegisterSyntaxError(SyntaxError(0, m_Tokens->at(m_TokenIndex)->GetLineNumber(), m_Tokens->at(m_TokenIndex)->GetCharacterNumber(), m_Tokens->at(m_TokenIndex)->GetContents().size()));
		}
		else
			m_TokenIndex++; 

		//Begin constructing statements
		ep->statements = ConstructStatements();

		if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::eof)
		{
			RegisterSyntaxError(SyntaxError(0, m_Tokens->at(m_TokenIndex)->GetLineNumber(), m_Tokens->at(m_TokenIndex)->GetCharacterNumber(), m_Tokens->at(m_TokenIndex)->GetContents().size()));
			return ep;
		}
		else
			return ep;
	}

	std::shared_ptr<RequireNode> Parser::ConstructRequire()
	{
		if (m_Tokens->at(m_TokenIndex)->GetType() == TokenType::Require)
		{
			m_TokenIndex++;
			std::shared_ptr<RequireNode> rn = std::shared_ptr<RequireNode>();
			if (m_Tokens->at(m_TokenIndex)->GetType() != TokenType::Identifier)
			{
				RegisterSyntaxError(SyntaxError(1, m_Tokens->at(m_TokenIndex)->GetLineNumber(), m_Tokens->at(m_TokenIndex)->GetCharacterNumber(), m_Tokens->at(m_TokenIndex)->GetContents().size()));
				return nullptr;
			}
			rn->requireType = m_Tokens->at(m_TokenIndex);
			m_TokenIndex++;
			return rn;
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
				m_TokenIndex++;
			}

			//If we have reached the end of the file, return
			TokenType currTokType = m_Tokens->at(m_TokenIndex)->GetType();
			if (currTokType == TokenType::eof || currTokType == TokenType::End)
				break;

			//Attempt to construct statement
			std::shared_ptr<Statement> s = ConstructStatement();
			if (s != nullptr)
			{
				statements->statements.push_back(s);
			}
			else
			{
				//If we didn't get a statement back, then we are done.
				break;
			}
		}
		return statements;
	}
	std::shared_ptr<Statement> Parser::ConstructStatement()
	{
		return std::shared_ptr<Statement>();
	}
}
