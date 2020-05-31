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

std::shared_ptr<TranspilerState> state = std::make_shared<TranspilerState>();

//------------------------------------- UtilFunctions utility function definitions -------------------------------
class MissingVariable
{
	uint32_t lineNumber;
	uint32_t columnNumber;

public:
	MissingVariable(uint32_t lineNumber, uint32_t columnNumber)
		: lineNumber(lineNumber), columnNumber(columnNumber)
	{}

	uint32_t GetLineNumber() const { return lineNumber; }
	uint32_t GetColumnNumber() const { return columnNumber; }
	uint32_t GetColumnSpan() const { return 1; }
};

int RegisterLexicalError(uint8_t errorCode, uint32_t lineNumber, uint32_t columnNumber, uint16_t columnSpan)
{
	state->LexerErrors.push_back({ errorCode, lineNumber, columnNumber, columnSpan });
	return 1;
}

int RegisterSyntaxError(SyntaxErrorType type, std::shared_ptr<Token> token)
{
	state->SyntaxErrors.push_back({ type, token });
	return 1;
}

int RegisterSyntaxError(SyntaxErrorType errorCode, uint32_t lineNumber, uint32_t columnNumber, size_t columnSpan, Spliwaca::TokenType type)
{
	state->SyntaxErrors.push_back({ errorCode, lineNumber, columnNumber, columnSpan, type });
	return 1;
}

int RegisterSemanticsError(uint32_t lineNumber, uint32_t columnNumber)
{
	//state->SemanticErrors.push_back(MissingVariable(lineNumber, columnNumber));
	return 1;
}

std::string mulString(std::string s, uint32_t i)
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

template <typename T>
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

#if !SN_ENABLE_PROFILING
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
#endif

//------------------------------------- End UtilFunctions utility function definitions -------------------------------

struct transpilerOptions
{
	std::string ifile;
	std::string ofile;
	bool recursive_transpile;
};

transpilerOptions* parseCommandLineArgs(int argc, char** argv)
{
	SN_PROFILE_FUNCTION();
	transpilerOptions* options = new transpilerOptions();
	if (argc < 2)
	{
		std::cout << "Usage: transpiler FILE [-o OUTFILE]\n";
		exit(-1);
	}
	else if (argc > 2 && argc != 4)
	{
		std::cout << "Usage: transpiler FILE [-o OUTFILE]\n";
		exit(-1);
	}
	else if (argc == 4 && strcmp(argv[2], "-o"))
	{
		std::cout << "Usage: transpiler FILE [-o OUTFILE]\n";
		exit(-1);
	}
	else if (argc == 4)
	{
		options->ofile = argv[3];
	}
	else
	{
		options->ofile = "";
	}
	options->ifile = argv[1];
	return options;
}

int main(int argc, char** argv)
{
	SN_PROFILE_BEGIN_SESSION("Run", "splw-run.json");
	SN_PROFILE_FUNCTION();
	#if !SN_ENABLE_PROFILING
	Timer timer = Timer();
	#endif
	transpilerOptions* options = parseCommandLineArgs(argc, argv);


	/*
	{
		SN_PROFILE_SCOPE("Set up unicode output");
		#ifdef SPLW_WINDOWS
		SetConsoleOutputCP(CP_UTF8);
		setvbuf(stdout, nullptr, _IOFBF, 1000);
		#endif
	}
	*/

	LOG_INIT();
	bool printTokenList = false;

	Transpiler transpiler = Transpiler(options->ifile, options->ofile, state, printTokenList);
	std::string output = transpiler.Run();

	#if SN_ENABLE_PROFILING
	timer_192.Stop();
	std::cout << "#Total time taken: " << timer_192.Elapsed() << "ms" << std::endl;
	#else
	std::cout << "#Total time taken: " << timer.elapsed() << "s" << std::endl;
	#endif
	SN_PROFILE_END_SESSION();
	return 0;
}
