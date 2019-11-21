#include <cstdint>
#include <string>
#include <iostream>
#include "Instrumentor.h"
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
	std::string init = s;
	for (size_t j = 0; j < i; j++)
	{
		s.append(init);
	}
	return s;
}

int numDigits(int32_t x)
{
	if (x >= 10000)
	{
		if (x >= 10000000)
		{
			if (x >= 100000000)
			{
				if (x >= 1000000000)
					return 10;
				return 9;
			}
			return 8;
		}
		if (x >= 100000)
		{
			if (x >= 1000000)
				return 7;
			return 6;
		}
		return 5;
	}
	if (x >= 100)
	{
		if (x >= 1000)
			return 4;
		return 3;
	}
	if (x >= 10)
		return 2;
	return 1;
}

int main()
{
	LOG_INIT();

	std::chrono::microseconds timeStartMakeTokens;
	std::chrono::microseconds timeEndMakeTokens;

	std::chrono::microseconds timeStart;
	std::chrono::microseconds timeEnd;

	Instrumentor::Get().BeginSession("Main");
	InstrumentationTimer t("Main_function");
	//Compiler(Parser(Lexer()));
	//Transpiler(Parser(Lexer()));
	//Interpreter(Parser(Lexer()));
	//Parser(Lexer());

	timeStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::shared_ptr<Lexer> lexer = Lexer::Create("c:/dev/epq spliwaca/test_script.splw");
	SPLW_INFO("Created lexer!");
	SPLW_WARN(mulString("h", 5));
	timeStartMakeTokens = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens = lexer->MakeTokens();
	timeEndMakeTokens = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	for (LexicalError l : state.LexerErrors)
	{
		SPLW_CRITICAL("Lexical Error code {2} at line {0}, column {1}", l.GetLineNumber(), l.GetColumnNumber(), l.GetErrorCode());
		SPLW_WARN("{0}", lexer->GetSplitFileString().at(l.GetLineNumber()));
		SPLW_WARN("{0}{1}", mulString(" ", l.GetColumnNumber() - 1), mulString("^", l.GetColumnSpan()));
		std::cout << "\n";
	}

	{
		PROFILE_SCOPE("Main_Output");
		int lineCount = lexer->GetSplitFileString().size();

		int i = 0;
		std::string secondReconstruction = "";
		for (std::shared_ptr<Token> t : *tokens)
		{
			if (t->GetContents() == "\n")
				SPLW_TRACE("Token {0}: {1},{2} type: {3}, contents: {4}", i, t->GetLineNumber(), t->GetCharacterNumber(), TokenTypeName(t->GetType()), "\\n");//, mulString(" ", 3 - std::to_string(i).size()), mulString(" ", numDigits(lineCount) - std::to_string(t->GetLineNumber()).size()), mulString(" ", 3 - std::to_string(t->GetCharacterNumber()).size()), mulString(" ", 16 - TokenTypeName(t->GetType()).size()));
			else if (t->GetContents() == "\t")
				SPLW_TRACE("Token {0}: {1},{2} type: {3}, contents: {4}", i, t->GetLineNumber(), t->GetCharacterNumber(), TokenTypeName(t->GetType()), "\\t");//, mulString(" ", 3 - std::to_string(i).size()), mulString(" ", numDigits(lineCount) - std::to_string(t->GetLineNumber()).size()), mulString(" ", 3 - std::to_string(t->GetCharacterNumber()).size()), mulString(" ", 16 - TokenTypeName(t->GetType()).size()));
			else if (t->GetContents() == "\f")
				SPLW_TRACE("Token {0}: {1},{2} type: {3}, contents: {4}", i, t->GetLineNumber(), t->GetCharacterNumber(), TokenTypeName(t->GetType()), "\\f");//, mulString(" ", 3 - std::to_string(i).size()), mulString(" ", numDigits(lineCount) - std::to_string(t->GetLineNumber()).size()), mulString(" ", 3 - std::to_string(t->GetCharacterNumber()).size()), mulString(" ", 16 - TokenTypeName(t->GetType()).size()));
			else
				SPLW_TRACE("Token {0}: {1},{2} type: {3}, contents: {4}", i, t->GetLineNumber(), t->GetCharacterNumber(), TokenTypeName(t->GetType()), t->GetContents());//, mulString(" ", 3 - std::to_string(i).size()), mulString(" ", numDigits(lineCount) - std::to_string(t->GetLineNumber()).size()), mulString(" ", 3 - std::to_string(t->GetCharacterNumber()).size()), mulString(" ", 16 - TokenTypeName(t->GetType()).size()));
			i++;
			secondReconstruction.append(t->GetContents());
		}

		std::cout << secondReconstruction << "\n";
	}

	SPLW_WARN("FINISHED OUTPUT!");
	timeEnd = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());

	t.Stop();
	Instrumentor::Get().EndSession();

	std::cout << "MakeTokens time taken: " << (timeEndMakeTokens - timeStartMakeTokens).count() << "\n";
	std::cout << "Main time taken: " << (timeEnd - timeStart).count() << "\n";

	system("PAUSE");
	return 0;
}
