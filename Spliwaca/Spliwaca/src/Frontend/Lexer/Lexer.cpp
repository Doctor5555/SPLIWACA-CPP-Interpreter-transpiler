#include "Lexer.h"

namespace Spliwaca
{
	Lexer* Lexer::Create(std::string file)
		
	{
		return new Lexer();
	}

	std::vector<Token> Lexer::MakeTokens()
	{
		std::vector<Token> tokens = { Token() };
		return tokens;
	}

	Lexer::Lexer()
		:m_Position(0), m_Tokens(std::vector<Token>())
	{

	}
}
