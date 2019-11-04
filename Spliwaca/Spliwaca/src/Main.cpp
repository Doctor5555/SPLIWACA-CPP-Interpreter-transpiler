#include <cstdint>
#include <string>
#include "Log.h"
#include "Frontend/Lexer/Lexer.h"

using namespace Spliwaca;

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

	system("PAUSE");
	return 0;
}