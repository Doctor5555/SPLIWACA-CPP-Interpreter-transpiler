#pragma once
#include "Frontend/Lexer/Lexer.h"
#include "Frontend/Lexer/LexicalError.h"
#include "Frontend/Parser/Parser.h"
#include "Frontend/Parser/SyntaxError.h"
#include "Backend/Code Generation/Generator.h"
#include "Frontend/Parser/SemanticError.h"
#include "UtilFunctions.h"

namespace Spliwaca
{
	struct TranspilerState
	{
		std::vector<LexicalError> LexerErrors;
		std::vector<SyntaxError> SyntaxErrors;
		//std::vector<MissingVariable> MissingVariables;
	};

	class Transpiler
	{
	public:
		Transpiler(std::string filename, std::shared_ptr<TranspilerState> state, bool printTokenList)
			: m_Filename(filename), m_State(state), m_PrintTokenList(printTokenList)
		{
		}

		std::string Run();

	private:
		std::string m_Filename;
		std::shared_ptr<TranspilerState> m_State;
		bool m_PrintTokenList;
	};
}
