#include <cstdint>
#include <string>
#include "Lexer/Lexer.h"

namespace Spliwaca
{
	int main()
	{
		//Compiler(Parser(Lexer()));
		//Transpiler(Parser(Lexer()));
		//Interpreter(Parser(Lexer()));
		//Parser(Lexer());
		Lexer* lexer = Lexer::Create("a.splw");

		return 0;
	}
}