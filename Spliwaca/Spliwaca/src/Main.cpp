#include <cstdint>
#include <string>
#include <iostream>
#include <chrono>

#ifdef SPLW_WINDOWS
#include <Windows.h>
#endif

//#include "Instrumentor.h"
#include "Transpiler.h"
#include "Log.h"

using namespace Spliwaca;

//------------------------------------- UtilFunctions utility function definitions -------------------------------
class MissingVariable
{
	uint32_t lineNumber;
	uint32_t columnNumber;

public:
	MissingVariable(uint32_t lineNumber, uint32_t columnNumber)
		: lineNumber(lineNumber), columnNumber(columnNumber) {}

	uint32_t GetLineNumber() const { return lineNumber; }
	uint32_t GetColumnNumber() const { return columnNumber; }
	uint32_t GetColumnSpan() const { return 1; }
};

std::shared_ptr<TranspilerState> state = std::make_shared<TranspilerState>();

int RegisterLexicalError(uint8_t errorCode, uint32_t lineNumber, uint32_t columnNumber, uint16_t columnSpan)
{
	state->LexerErrors.push_back({errorCode, lineNumber, columnNumber, columnSpan});
	return 1;
}

int RegisterSyntaxError(SyntaxErrorType type, std::shared_ptr<Token> token)
{
	state->SyntaxErrors.push_back({type, token});
	return 1;
}

int RegisterSyntaxError(SyntaxErrorType errorCode, uint32_t lineNumber, uint32_t columnNumber, size_t columnSpan, Spliwaca::TokenType type)
{
	state->SyntaxErrors.push_back({errorCode, lineNumber, columnNumber, columnSpan, type});
	return 1;
}

int RegisterSemanticsError(uint32_t lineNumber, uint32_t columnNumber)
{
	//state->SemanticErrors.push_back(MissingVariable(lineNumber, columnNumber));
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

bool charInStr(const std::string &s, char c)
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

template <typename T>
bool itemInVect(const std::vector<T> &v, T t)
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
		return std::chrono::duration_cast<second_>(clock_::now() - beg_).count();
	}

private:
	typedef std::chrono::high_resolution_clock clock_;
	typedef std::chrono::duration<double, std::ratio<1>> second_;
	std::chrono::time_point<clock_> beg_;
};

//------------------------------------- End UtilFunctions utility function definitions -------------------------------

int main(int argc, char** argv)
{
	std::string ifile, ofile;
	if (argc < 2) {
		std::cout << "Usage: transpiler FILE [-o OUTFILE]\n";
		return -1;
	}
	else if (argc > 2 && argc != 4) {
		std::cout << "Usage: transpiler FILE [-o OUTFILE]\n";
		return -1;
	}
	else if (argc == 4 && strcmp(argv[2], "-o")) {
		std::cout << "Usage: transpiler FILE [-o OUTFILE]\n";
		return -1;
	}
	else if (argc == 4) {
		ofile = argv[3];
	}
	else {
		ofile = "";
	}
	ifile = argv[1];
	Timer totalTimer = Timer();

	#ifdef SPLW_WINDOWS
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);
	#endif

	LOG_INIT();
	bool printTokenList = false;

	Transpiler transpiler = Transpiler(ifile, ofile, state, printTokenList);
	std::string output = transpiler.Run();

	//std::cout << "\nLexer took: " << lexerTime << " seconds\nParser took: " << parseTime << " seconds\nGenerator took: " << generateTime << " seconds" << std::endl;
	std::cout << "#Total time taken: " << totalTimer.elapsed() << std::endl;

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

	/*for (MissingVariable m : state->MissingVariables)
	{
		SPLW_CRITICAL("Missing variable at line {0}, column {1}", m.GetLineNumber(), m.GetColumnNumber());
		SPLW_WARN("{0}", lexer->GetSplitFileString().at(m.GetLineNumber()));
		SPLW_WARN("{0}{1}", mulString(" ", m.GetColumnNumber() - 1), mulString("^", m.GetColumnSpan()));
		std::cout << "\n";
	}

	if (state->MissingVariables.size() > 0)
	{
		SPLW_ERROR("Missing variables present: cannot continue to next stage.");
		system("PAUSE");
		return -1;
	}
	*/
	#ifdef SPLW_WINDOWS
	system("PAUSE");
	#else
		//system("read -n 1 -s -p \"Press any key to continue...\n\"");
	#endif
	return 0;
}
