#include <cstdint>
#include <string>
#include <iostream>
#include "Log.h"
#include "Frontend/Lexer/Lexer.h"
#include "Frontend/Lexer/LexicalError.h"
#include "UtilFunctions.h"

using namespace Spliwaca;

struct State
{
	std::vector<LexicalError> LexerErrors;
};
State state = State();

int RegisterLexicalError(LexicalError e)
{
	state.LexerErrors.push_back(e);
	return 1;
}

std::string mulString(std::string s, int i)
{
	for (size_t j = 0; j < i; j++)
	{
		s.append(s);
	}
	return s;
}

int main()
{
	//Compiler(Parser(Lexer()));
	//Transpiler(Parser(Lexer()));
	//Interpreter(Parser(Lexer()));
	//Parser(Lexer());
	Log::Init();
	std::shared_ptr<Lexer> lexer = Lexer::Create("c:/dev/epq spliwaca/test_script.splw");
	SPLW_INFO("Created lexer!");
	std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens = lexer->MakeTokens();
	/*for (LexicalError l : state.LexerErrors)
	{
		SPLW_CRITICAL("Lexical Error code {2} at line {0}, column {1}", l.GetLineNumber(), l.GetColumnNumber(), l.GetErrorCode());
		SPLW_WARN("{0}", lexer->GetSplitFileString()[l.GetLineNumber()]);
		SPLW_WARN("{0}{1}", mulString(" ", l.GetColumnNumber() - 1), mulString("^", l.GetColumnSpan()));
		std::cout << "\n";
	}
	*/

	int i = 0;
	std::string secondReconstruction = "";
	for (std::shared_ptr<Token> t : *tokens)
	{
		if (t->GetContents() == "\n")
			SPLW_TRACE("Token {0}: {1},{2} type: {3}, contents: {4}", i, t->GetLineNumber(), t->GetCharacterNumber(), TokenTypeName(t->GetType()), "\\n");
		else if (t->GetContents() == "\t")
			SPLW_TRACE("Token {0}: {1},{2} type: {3}, contents: {4}", i, t->GetLineNumber(), t->GetCharacterNumber(), TokenTypeName(t->GetType()), "\\t");
		else if (t->GetContents() == "\f")
			SPLW_TRACE("Token {0}: {1},{2} type: {3}, contents: {4}", i, t->GetLineNumber(), t->GetCharacterNumber(), TokenTypeName(t->GetType()), "\\f");
		else
			SPLW_TRACE("Token {0}: {1},{2} type: {3}, contents: {4}", i, t->GetLineNumber(), t->GetCharacterNumber(), TokenTypeName(t->GetType()), t->GetContents());
		i++;
		secondReconstruction.append(t->GetContents());
		secondReconstruction.append(" ");
	}

	std::cout << secondReconstruction << "\n";
	system("PAUSE");
	return 0;
}
