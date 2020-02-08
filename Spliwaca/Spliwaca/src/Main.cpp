#include <cstdint>
#include <string>
#include <iostream>
#include <chrono>
#include <Windows.h>
//#include "Instrumentor.h"
#include "Log.h"
#include "Frontend/Lexer/Lexer.h"
#include "Frontend/Lexer/LexicalError.h"
#include "Frontend/Parser/Parser.h"
#include "Frontend/Parser/SyntaxError.h"
#include "Backend/Code Generation/Generator.h"
#include "UtilFunctions.h"

using namespace Spliwaca;

//------------------------------------- UtilFunctions utility function definitions -------------------------------
class MissingVariable
{
	uint32_t lineNumber;
	uint32_t columnNumber;

public:
	MissingVariable(uint32_t lineNumber, uint32_t columnNumber)
		: lineNumber(lineNumber), columnNumber(columnNumber) { }

	uint32_t GetLineNumber() const { return lineNumber; }
	uint32_t GetColumnNumber() const { return columnNumber; }
	uint32_t GetColumnSpan() const { return 1; }
};

struct State
{
	std::vector<LexicalError> LexerErrors;
	std::vector<SyntaxError> SyntaxErrors;
	std::vector<MissingVariable> MissingVariables;
};
State state = State();

int RegisterLexicalError(uint8_t errorCode, uint32_t lineNumber, uint32_t columnNumber, uint16_t columnSpan)
{
	state.LexerErrors.push_back({errorCode, lineNumber, columnNumber, columnSpan});
	return 1;
}

int RegisterSyntaxError(SyntaxErrorType type, std::shared_ptr<Token> token)
{
	state.SyntaxErrors.push_back({type, token});
	return 1;
}

int RegisterSyntaxError(SyntaxErrorType errorCode, uint32_t lineNumber, uint32_t columnNumber, size_t columnSpan, Spliwaca::TokenType type)
{
	state.SyntaxErrors.push_back({ errorCode, lineNumber, columnNumber, columnSpan, type });
	return 1;
}

int RegisterSemanticsError(uint32_t lineNumber, uint32_t columnNumber)
{
	//state.SemanticErrors.push_back(MissingVariable(lineNumber, columnNumber));
	return 1;
}

std::string mulString(std::string s, int i)
{
	if (i <= 0)
		return "";
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
	//PROFILE_FUNC();
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

class Timer
{
public:
	Timer() : beg_(clock_::now()) {}
	void reset() { beg_ = clock_::now(); }
	double elapsed() const
	{
		return std::chrono::duration_cast<second_>
			(clock_::now() - beg_).count();
	}

private:
	typedef std::chrono::high_resolution_clock clock_;
	typedef std::chrono::duration<double, std::ratio<1> > second_;
	std::chrono::time_point<clock_> beg_;
};

//------------------------------------- End UtilFunctions utility function definitions -------------------------------

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	LOG_INIT();
	bool printTokenList = false;

	Timer lexerTimer = Timer();

	std::shared_ptr<Lexer> lexer = Lexer::Create("c:/dev/epq spliwaca/test_script.splw");
	SPLW_INFO("Created lexer.");	
	std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens = lexer->MakeTokens();
	
	for (LexicalError l : state.LexerErrors)
	{
		SPLW_CRITICAL("Lexical Error code {2} at line {0}, column {1}", l.GetLineNumber(), l.GetColumnNumber(), l.GetErrorCode());
		if (l.GetLineNumber() >= lexer->GetSplitFileString().size())
			SPLW_WARN("Line {0} out of range!", l.GetLineNumber());
		else
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

	double lexerTime = lexerTimer.elapsed();
	
	if (printTokenList == true)
	{
		int i = 0;
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
		}
	}

	Timer parseTimer = Timer();

	std::shared_ptr<Parser> parser = Parser::Create(tokens);
	SPLW_INFO("Created Parser.");
	std::shared_ptr<Spliwaca::EntryPoint> ast = parser->ConstructAST();

	for (SyntaxError s : state.SyntaxErrors)
	{
		
		SPLW_CRITICAL("Syntax Error code {2} at line {0}, column {1}", s.GetLineNumber(), s.GetColumnNumber(), s.GetErrorCode());
		if (s.GetLineNumber() >= lexer->GetSplitFileString().size())
			SPLW_WARN("Line {0} out of range!", s.GetLineNumber());
		else
			SPLW_WARN("{0}", lexer->GetSplitFileString().at(s.GetLineNumber()));
		SPLW_WARN("{0}{1}", mulString(" ", s.GetColumnNumber() - 1), mulString("^", s.GetColumnSpan()));
		std::cout << "\n";
	}
	if (state.SyntaxErrors.size() > 0)
	{
		SPLW_ERROR("Syntax errors present: cannot continue to next stage.");
		if (state.MissingVariables.size() == 0)
		{
			system("PAUSE");
			return -1;
		}
	}
	else
		SPLW_INFO("Finished syntax analysis.");

	double parseTime = parseTimer.elapsed();

	Timer generateTimer = Timer();

	std::shared_ptr<Generator> codeGenerator = Generator::Create(ast);
	SPLW_INFO("Created Generator");

	std::string finalCode = codeGenerator->GenerateCode();

	double generateTime = generateTimer.elapsed();

	std::cout << finalCode << std::endl;

	SPLW_INFO("Finished code output!");

	SPLW_INFO("\nLexer took: {0} seconds\nParser took: {1} seconds\nGenerator took: {2} seconds", lexerTime, parseTime, generateTime);

	/*
	ROOT
	┝BRANCH
	│┝LEAF
	│┝LEAF
	┝BRANCH
	 ┝LEAF
	 ┝LEAF
	
	//std::cout << "ROOT\n┝BRANCH\n│┝LEAF\n│┝LEAF\n┝BRANCH\n ┝LEAF\n ┝LEAF" << "\n";
	SPLW_INFO(u8R"(ROOT
		       ┝BRANCH
		       │┝LEAF
		       │┕LEAF
		       ┕BRANCH
		        ┝LEAF
		        ┕LEAF)");
	//std::cout << test << std::endl;
	*/

	/*for (MissingVariable m : state.MissingVariables)
	{
		SPLW_CRITICAL("Missing variable at line {0}, column {1}", m.GetLineNumber(), m.GetColumnNumber());
		SPLW_WARN("{0}", lexer->GetSplitFileString().at(m.GetLineNumber()));
		SPLW_WARN("{0}{1}", mulString(" ", m.GetColumnNumber() - 1), mulString("^", m.GetColumnSpan()));
		std::cout << "\n";
	}

	if (state.MissingVariables.size() > 0)
	{
		SPLW_ERROR("Missing variables present: cannot continue to next stage.");
		system("PAUSE");
		return -1;
	}
	*/

	system("PAUSE");
	return 0;
}
