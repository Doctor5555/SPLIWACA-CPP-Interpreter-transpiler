#pragma once
#include "Token.h"
#include <vector>

namespace Spliwaca
{
	class Lexer
	{
	public:
		static Lexer* Create(std::string file);
		~Lexer() = default;

	private:
		Lexer();
		std::vector<Token> MakeTokens();

	private:
		uint32_t m_Position;
		std::vector<Token> m_Tokens;
	};
}
