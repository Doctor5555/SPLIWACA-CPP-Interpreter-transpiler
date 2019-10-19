#pragma once
#include <string>

namespace Spliwaca
{
	enum class TokenType
	{
		Int = 0,
		Float,
		Plus,
		Minus,
		Multiply,
		Divide,
		LParen,
		RParen,
		Keyword,
		Identifier
	};
	
	class Token
	{
	public:
		Token();
		virtual ~Token() = default;

	private:
		TokenType m_Type;
		std::string m_Contents;
	};
}
