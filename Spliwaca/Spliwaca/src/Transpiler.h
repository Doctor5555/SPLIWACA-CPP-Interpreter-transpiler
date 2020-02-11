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
		std::string GetSyntaxErrorMessage(SyntaxErrorType type)
		{
			switch (type)
			{
			case expNewline:   // 0
			case expIdent:         // 1
			case expStatement:     // 2
			case expAtom:          // 3
			case expType:          // 4
			case expComma:         // 5
			case expRParen:        // 6
			case expRSquareParen: // 7
			case expDo:         // 8
			case expTo:	          // 9
			case expOf:	          // 10
					case expWith          // 11
					case expTakes        // 12
					case expReturns       // 13
					case expAs	          // 14
					case expRaw          // 15
					case expEndIf         // 16
					case expEndFor       // 17
					case expEndWhile      // 18
					case expEndFunc	      // 19
					case expEndProc	      // 20
					case expEndStruct     // 21
					case expTypeMod	      // 22
					case tooManyElse      // 23
					case unexpEndFunc     // 24
					case unexpEndProc    // 25
					case unexpEndIf	      // 26
					case unexpEndFor      // 27
					case unexpEndWhile  // 28
					case unexpEndStruct  // 29
					case unexpElseIf      // 30
					case inconsistentDict
			}
		}

	private:
		std::string m_Filename;
		std::shared_ptr<TranspilerState> m_State;
		bool m_PrintTokenList;
	};
}
