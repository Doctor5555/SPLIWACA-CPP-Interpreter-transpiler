#include "Transpiler.h"
#include "Log.h"
#include <iostream>
#include <fstream>

namespace Spliwaca
{
	std::string Transpiler::Run()
	{
		//Timer lexerTimer = Timer();

		std::shared_ptr<Lexer> lexer = Lexer::Create(m_Filename);
		SPLW_INFO("Created lexer.");
		std::shared_ptr<std::vector<std::shared_ptr<Token>>> tokens = lexer->MakeTokens();

		for (LexicalError l : m_State->LexerErrors)
		{
			SPLW_CRITICAL("Lexical Error code {2} at line {0}, column {1}", l.GetLineNumber(), l.GetColumnNumber(), l.GetErrorCode());
			if (l.GetLineNumber() >= lexer->GetSplitFileString().size())
				SPLW_WARN("Line {0} out of range!", l.GetLineNumber());
			else
				SPLW_WARN("{0}", lexer->GetSplitFileString().at(l.GetLineNumber()));
			SPLW_WARN("{0}{1}", mulString(" ", l.GetColumnNumber() - 1), mulString("^", l.GetColumnSpan()));
			std::cout << "\n";
		}
		if (m_State->LexerErrors.size() > 0)
		{
			SPLW_ERROR("Lexical errors present: cannot continue to parsing stage.");
			system("PAUSE");
			return "";
		}
		else
			SPLW_INFO("Finished constructing tokens.");

		//double lexerTime = lexerTimer.elapsed();

		if (m_PrintTokenList)
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

		//Timer parseTimer = Timer();

		std::shared_ptr<Parser> parser = Parser::Create(tokens);
		SPLW_INFO("Created Parser.");
		std::shared_ptr<Spliwaca::EntryPoint> ast = parser->ConstructAST();

		for (SyntaxError s : m_State->SyntaxErrors)
		{
			SPLW_CRITICAL("Syntax Error code {2} at line {0}, column {1}", s.GetLineNumber(), s.GetColumnNumber(), s.GetErrorCode());
			SPLW_CRITICAL(GetSyntaxErrorMessage(s.GetErrorCode()), TokenTypeName(s.GetTokenType()));
			if (s.GetLineNumber() >= lexer->GetSplitFileString().size())
				SPLW_WARN("Line {0} out of range!", s.GetLineNumber());
			else
				SPLW_WARN("{0}", lexer->GetSplitFileString().at(s.GetLineNumber()));
			SPLW_WARN("{0}{1}", mulString(" ", s.GetColumnNumber() - 1), mulString("^", s.GetColumnSpan()));
			std::cout << "\n";
		}
		if (m_State->SyntaxErrors.size() > 0)
		{
			SPLW_ERROR("Syntax errors present: cannot continue to next stage.");
			//if (m_State->MissingVariables.size() == 0)
			//{
			//}
			#ifdef SPLW_WINDOWS
			system("PAUSE");
			#else
			system("read -n 1 -s -p \"Press any key to continue...\n\"");
			#endif
			return "";
		}
		else
			SPLW_INFO("Finished syntax analysis.");

		//double parseTime = parseTimer.elapsed();

		//Timer generateTimer = Timer();

		std::shared_ptr<Generator> codeGenerator = Generator::Create(ast);
		SPLW_INFO("Created Generator");

		int errorCode;
		std::string finalCode = codeGenerator->GenerateCode(errorCode);
		if (errorCode)
			return "";

		if (m_Output != "") {
			std::ofstream outputFile;
			outputFile.open(m_Output, std::ios::trunc);
			outputFile << finalCode << "\n";
			outputFile.close();
		}
		else {
			std::cout << finalCode << std::endl;
		}

		//double generateTime = generateTimer.elapsed();

		//std::cout << finalCode << std::endl;

		SPLW_INFO("Finished code output!");

		return finalCode;
	}
}