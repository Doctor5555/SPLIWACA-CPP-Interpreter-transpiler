#include <cstdint>
#include <string>
#include "Log.h"
#include "Frontend/Lexer/Lexer.h"

namespace Spliwaca
{
	int main()
	{
		//Compiler(Parser(Lexer()));
		//Transpiler(Parser(Lexer()));
		//Interpreter(Parser(Lexer()));
		//Parser(Lexer());
		Log::Init();
		Lexer* lexer = Lexer::Create("c:/dev/epq\ spliwaca/test_script.splw");

		return 0;
	}
}