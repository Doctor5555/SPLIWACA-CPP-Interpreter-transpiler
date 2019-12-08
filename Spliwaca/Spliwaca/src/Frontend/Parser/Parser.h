#pragma once
#include <memory>
#include <vector>
#include "Frontend/Lexer/Token.h"
#include "Nodes.h"

namespace Spliwaca
{
	class Parser
	{
	public:
		std::shared_ptr<Parser> Create(std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens);
		~Parser() = default;

		std::shared_ptr<EntryPoint> ConstructAST();
		
	private:
		std::shared_ptr<RequireNode> ConstructRequire();
		std::shared_ptr<Statements> ConstructStatements();
		std::shared_ptr<Statement> ConstructStatement();

		Parser(std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens)
			: m_Tokens(tokens), m_TokenIndex(0) { }

		std::shared_ptr<std::vector<std::shared_ptr<Token>>> m_Tokens;
		uint32_t m_TokenIndex;
	};
}
