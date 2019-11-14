#include "Lexer.h"
#include <regex>
#include <fstream>
#include "Log.h"
#include <iostream>
#include <map>

namespace Spliwaca
{
	struct reMatch
	{
		std::string prefix;
		std::string suffix;
		std::vector<std::string> match;
	};

	std::vector<std::shared_ptr<reMatch>> RegexFindall(const char* re, std::string string)
	{
		bool success = true;
		std::vector<std::shared_ptr<reMatch>> matches;

		uint32_t i = 0;
		while (success)
		{
			std::regex regex(re);
			matches.push_back(std::shared_ptr<reMatch>(new reMatch()));
			std::smatch m;
			SPLW_TRACE(i);
			success = std::regex_search(string, m, regex);
			
			if (success)
			{
				matches[i]->prefix = m.prefix();
				matches[i]->suffix = m.suffix();

				for (uint32_t j = 0; j < m.length(); j++)
				{
					matches[i]->match.push_back(m[j]);
				}

				string = matches[i]->suffix;

				i++;
			}
		}

		return matches;
	}

	std::shared_ptr<Lexer> Lexer::Create(std::string file)
	{
		return std::shared_ptr<Lexer>(new Lexer(file));
	}

	Lexer::Lexer(std::string fileLocation)
		: m_Tokens(new std::vector<std::shared_ptr<Token>>()), m_FileLocation(fileLocation)
	{
		std::ifstream file;
		file.open(m_FileLocation);
		std::string fileContents;

		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				fileContents.append(line + "\n");
			}
			SPLW_TRACE("File contents: {0}", fileContents);
			//std::cout << fileContents << "\n";
		}
		else
		{
			SPLW_ERROR("Could not open source file: {0}", m_FileLocation);
		}

		file.close();

		std::shared_ptr<Token> token;
		token.reset(new Token(TokenType::UnfinishedToken, fileContents.c_str(), 0, 0));

		m_Tokens->push_back(token);
	}

	std::shared_ptr<std::vector<std::shared_ptr<Token>>> Lexer::MakeTokens()
	{
		//SPLW_TRACE(m_Tokens->at(0)->GetContents());
		StripComments();
		MakeFunctionsProceduresStructs();
		SPLW_TRACE(m_Tokens->at(0)->GetContents());
		SPLW_TRACE(m_Tokens->at(1)->GetContents());
		return m_Tokens;
	}

	void Lexer::StripComments()
	{
		bool success = true;
		std::string program = m_Tokens->at(0)->GetContents();
		//SPLW_TRACE("Getting program string: {0}", program);
		std::string strippedProgram = "";
		std::regex re("/\\*(.|\n)*?\\*/|//.*");
		while (success)
		{
			std::smatch m;
			success = std::regex_search(program, m, re);
			if (success)
			{
				//SPLW_TRACE("Comments: m[0]: {0}", m[0]);
				strippedProgram.append(m.prefix());
				program = m.suffix();
			}
			else
			{
				strippedProgram.append(program);
				//SPLW_TRACE("Finished removing comments: {0}", strippedProgram);
			}
		}

		std::shared_ptr<Token> token;
		token.reset(new Token(TokenType::UnfinishedToken, strippedProgram.c_str(), 0, 0));

		//SPLW_TRACE(strippedProgram);
		//SPLW_TRACE(program);

		m_Tokens->pop_back();
		m_Tokens->push_back(token);
	}

	bool Lexer::MakeFunctionsProceduresStructs()
	{
		bool success = false;

		int i = 0;
		while (i < m_Tokens->size())
		{
			std::shared_ptr<Token> token = m_Tokens->at(i);
			if (token->GetType() == TokenType::UnfinishedToken)
			{
				std::vector<std::shared_ptr<Token>> newTokens;
				std::string code = token->GetContents();
				std::vector<std::shared_ptr<reMatch>> matches = 
					RegexFindall("((PROCEDURE|PROC) ([a-zA-Z_][a-zA-Z0-9_]*) (TAKES (([a-zA-Z_][a-zA-Z0-9_]*) .+?)+?)?AS *?\n+((.|\n)*?)(END (PROCEDURE|PROC)))|((FUNCTION|FUNC) ([a-zA-Z_][a-zA-Z0-9_]*) (TAKES (([a-zA-Z_][a-zA-Z0-9_]*) (.+?))+?)?(\\->|RETURNS) ([a-zA-Z_][a-zA-Z0-9_]*) AS *?\n+((.|\n)*?)\n(\t| )*?(RETURN (.*)))|((STRUCTURE|STRUCT) ([a-zA-Z_][a-zA-Z0-9_]*) AS *?\n+?(((\t| )*([a-zA-Z_][a-zA-Z0-9_]*) ([a-zA-Z_][a-zA-Z0-9_]*) *?\n)*?) *?\n*?(\t| )*?END (STRUCTURE|STRUCT))", 
						code);
				for each (std::shared_ptr<reMatch> match in matches)
				{
					newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->prefix.c_str(), 0, 0)));
					if (match->match[0][0] == 'F') //Function
					{
						//@TODO: Implement line numbers for tokens
						/*FUNC*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Function, match->match[12].c_str(), 0, 0)));
						/*x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Identifier, match->match[13].c_str(), 0, 0)));
						if (match->match[14] != "")
						{
							/*TAKES*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Takes, "TAKES", 0, 0)));
							/*type x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[15].c_str(), 0, 0)));
						}
						/*->*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::ReturnType, match->match[18].c_str(), 0, 0)));
						/*type*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[19].c_str(), 0, 0)));
						/*AS*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::As, "AS", 0, 0)));
						/*body*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[20].c_str(), 0, 0)));
						/*RETURN*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Return, "RETURN", 0, 0)));
						/*rv*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[24].c_str(), 0, 0)));
					}
					else if (match->match[0][0] == 'P') //Procedure
					{
						//@TODO: Implement line numbers for tokens
						/*PROC*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Procedure, match->match[1].c_str(), 0, 0)));
						/*x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Identifier, match->match[2].c_str(), 0, 0)));
						if (match->match[3] != "")
						{
							/*TAKES*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Takes, "TAKES", 0, 0)));
							/*type x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[4].c_str(), 0, 0)));
						}
						/*AS*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::As, "AS", 0, 0)));
						/*body*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[20].c_str(), 0, 0)));
						/*END*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::EndProc, match->match[9].c_str(), 0, 0)));
					}
					else if (match->match[0][0] == 'S') //Struct
					{
						//@TODO: Implement line numbers for tokens
						/*STRUCT*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Procedure, match->match[26].c_str(), 0, 0)));
						/*x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Identifier, match->match[27].c_str(), 0, 0)));
						/*AS*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::As, "AS", 0, 0)));
						/*body*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[28].c_str(), 0, 0)));
						/*END*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::EndProc, match->match[34].c_str(), 0, 0)));
					}
					else
					{
						SPLW_ERROR("Match does not appear to be function, procedure or struct! {0}", match->match[0]);
						__debugbreak();
					}
				}
				newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, matches.back()->suffix.c_str(), 0, 0)));
				for each (std::shared_ptr<Token> token in newTokens)
				{
					m_Tokens->emplace(m_Tokens->begin() + i, token);
				}
				i += newTokens.size();
			}
			else
				i++;
		}
		return success;
	}

	bool Lexer::MakeIfForWhileAnons()
	{
		std::vector<uint32_t> indexes;
		for each (std::shared_ptr<Token> token in *m_Tokens)
		{

		}
		return false;
	}

	bool Lexer::MakeIO()
	{
		return false;
	}

	bool Lexer::MakeCreateCastSet()
	{
		return false;
	}

	bool Lexer::MakeNumberStringLiterals()
	{
		return false;
	}

	bool Lexer::MakeRequireQuit()
	{
		return false;
	}

	bool Lexer::MakeOperators()
	{
		return false;
	}

	bool Lexer::MakeFinalIdentifiers()
	{
		return false;
	}
}
