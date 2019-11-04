#pragma once
#include "Token.h"
#include <memory>
#include <vector>

namespace Spliwaca
{
	class Lexer
	{
	public:
		static std::shared_ptr<Lexer> Create(std::string file);
		~Lexer() = default;

		std::shared_ptr<std::vector<std::shared_ptr<Token>>> MakeTokens();

		void StripComments();
		bool MakeFunctionsProceduresStructs();
		bool MakeIfForWhileAnons();
		bool MakeIO();
		bool MakeCreateCastSet();
		bool MakeNumberStringLiterals();
		bool MakeRequireQuit();
		bool MakeOperators();
		bool MakeFinalIdentifiers();

	private:
		Lexer(std::string file);

	private:
		std::string m_FileLocation;
		std::shared_ptr<std::vector<std::shared_ptr<Token>>> m_Tokens;
	};
}
