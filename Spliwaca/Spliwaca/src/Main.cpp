#include <cstdint>
#include <string>
#include <iostream>
#include "Instrumentor.h"
#include "Log.h"
#include "Frontend/Lexer/Lexer.h"
#include "Frontend/Lexer/LexicalError.h"
#include "Frontend/Parser/Parser.h"
#include "Frontend/Parser/SyntaxError.h"
#include "UtilFunctions.h"

using namespace Spliwaca;

//------------------------------------- UtilFunctions utility function definitions -------------------------------
struct State
{
	std::vector<LexicalError> LexerErrors;
	std::vector<SyntaxError> SyntaxErrors;
};
State state = State();

int RegisterLexicalError(LexicalError e)
{
	state.LexerErrors.push_back(e);
	return 1;
}

int RegisterSyntaxError(SyntaxError e)
{
	state.SyntaxErrors.push_back(e);
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

/* Code snippet copied from https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
   accepted answer */
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

bool charInStr(const std::string& s, char c)
{
	PROFILE_FUNC();
	for (char ch : s)
	{
		if (ch == c)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
bool itemInVect(const std::vector<T>& v, T t)
{
	for (T e : v)
	{
		if (e == t)
		{
			return true;
		}
	}
	return false;
}

//------------------------------------- End UtilFunctions utility function definitions -------------------------------

int main()
{
	LOG_INIT();

	//std::chrono::microseconds timeStartMakeTokens;
	//std::chrono::microseconds timeEndMakeTokens;

	//std::chrono::microseconds timeStart;
	//std::chrono::microseconds timeEnd;

	//Instrumentor::Get().BeginSession("Main");
	//InstrumentationTimer t("Main_function");
	//Compiler(Parser(Lexer()));
	//Transpiler(Parser(Lexer()));
	//Interpreter(Parser(Lexer()));
	//Parser(Lexer());

	//timeStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	
	std::shared_ptr<Lexer> lexer = Lexer::Create("c:/dev/epq spliwaca/test_script.splw");
	SPLW_INFO("Created lexer.");
	//SPLW_WARN(mulString("h", 5));
	
	//timeStartMakeTokens = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	
	std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens = lexer->MakeTokens();
	
	//timeEndMakeTokens = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	for (LexicalError l : state.LexerErrors)
	{
		SPLW_CRITICAL("Lexical Error code {2} at line {0}, column {1}", l.GetLineNumber(), l.GetColumnNumber(), l.GetErrorCode());
		SPLW_WARN("{0}", lexer->GetSplitFileString().at(l.GetLineNumber()));
		SPLW_WARN("{0}{1}", mulString(" ", l.GetColumnNumber() - 1), mulString("^", l.GetColumnSpan()));
		std::cout << "\n";
	}

	if (state.LexerErrors.size() > 0)
	{
		SPLW_ERROR("Lexical errors present: cannot continue to parsing stage.");
		system("PAUSE");
		return -1;
	}
	else
		SPLW_INFO("Finished constructing tokens.");

	std::shared_ptr<Parser> parser = Parser::Create(tokens);
	SPLW_INFO("Created Parser.");

	std::shared_ptr<Spliwaca::EntryPoint> ast = parser->ConstructAST();

	for (SyntaxError s : state.SyntaxErrors)
	{
		SPLW_CRITICAL("Syntax Error code {2} at line {0}, column {1}", s.GetLineNumber(), s.GetColumnNumber(), s.GetErrorCode());
		SPLW_WARN("{0}", lexer->GetSplitFileString().at(s.GetLineNumber()));
		SPLW_WARN("{0}{1}", mulString(" ", s.GetColumnNumber() - 1), mulString("^", s.GetColumnSpan()));
		std::cout << "\n";
	}

	
	{
		//PROFILE_SCOPE("Main_Output");
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

		//std::cout << secondReconstruction << "\n";
	}
	

	//SPLW_WARN("FINISHED OUTPUT!");
	//timeEnd = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());

	//t.Stop();
	//Instrumentor::Get().EndSession();

	//std::cout << "MakeTokens time taken: " << (timeEndMakeTokens - timeStartMakeTokens).count() << "\n";
	//std::cout << "Main time taken: " << (timeEnd - timeStart).count() << "\n";

	system("PAUSE");
	return 0;
}
