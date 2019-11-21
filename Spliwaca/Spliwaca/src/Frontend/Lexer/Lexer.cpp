#include "Lexer.h"
#include <regex>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include "Instrumentor.h"
#include "Log.h"
#include "UtilFunctions.h"

namespace Spliwaca
{
	/*
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
	*/



	std::shared_ptr<Lexer> Lexer::Create(std::string file)
	{
		return std::shared_ptr<Lexer>(new Lexer(file));
	}

	Lexer::Lexer(std::string fileLocation)
		: m_Tokens(new std::vector<std::shared_ptr<Token>>()), m_FileLocation(fileLocation)
	{
		std::ifstream file;
		file.open(m_FileLocation);
		//char* fileContents;

		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				m_FileString.append(line+"\n");
			}
			//SPLW_TRACE("File contents: {0}", m_FileString);
			//std::cout << fileContents << "\n";
		}
		else
		{
			SPLW_ERROR("Could not open source file: {0}", m_FileLocation);
		}

		file.close();

		//std::shared_ptr<Token> token;
		//token.reset(new Token(TokenType::UnfinishedToken, fileContents.c_str(), 0, 0));

		//m_Tokens->push_back(token);
	}

	/*
	std::shared_ptr<std::vector<std::shared_ptr<Token>>> Lexer::MakeTokens()
	{
		//SPLW_TRACE(m_Tokens->at(0)->GetContents());
		//StripComments();
		//MakeFunctionsProceduresStructs();
		std::vector<std::string> words = split(m_FileString);

		std::string reconstruction = "";
		for (int i = 0; i < words.size(); i++)
		{
			reconstruction.append(words[i]);
		}
		SPLW_INFO("Successful string reconstruction: {0}", reconstruction == m_FileString);
		
		uint32_t lineNumber = 0;
		uint32_t columnNumber = 0;
		int i = 0;
		while (true)
		{
			std::string word = words[i];

			if (word == "\n")
			{
				lineNumber += 1;
				columnNumber = 0;
			}
			else
				columnNumber += word.size();

			if (s_KeywordDict.find(word) != s_KeywordDict.end() && word != "RAW" && word != "OUTPUT" && word != "/*" && word != "//")
			{
				//We have a keyword!
				m_Tokens->push_back(std::make_shared<Token>(Token(s_KeywordDict.at(word), word.c_str(), lineNumber, columnNumber)));
			}
			else if (word == "\"")
			{
				//Consume to next double quote as string
				uint32_t startLineNumber = lineNumber;
				uint32_t startColumnNumber = columnNumber;
				std::string str = "";
				while (true)
				{
					i++;

					if (words[i] == "\"")
					{
						columnNumber += 1;
						break;
					}
					else
					{
						if (word == "\n")
						{
							lineNumber += 1;
							columnNumber = 0;
						}
						else
							columnNumber += word.size();
						str.append(words[i]);
					}
				}
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::String, str.c_str(), startLineNumber, startColumnNumber)));

			}
			else if (word == "\'")
			{
				//Consume to next single quote as string
				uint32_t startLineNumber = lineNumber;
				uint32_t startColumnNumber = columnNumber;
				std::string str = "";
				while (true)
				{
					i++;

					if (words[i] == "\'")
					{
						columnNumber++;
						break;
					}
					else
					{
						if (word == "\n")
						{
							lineNumber += 1;
							columnNumber = 0;
						}
						else
							columnNumber += word.size();
						str.append(words[i]);
					}
				}
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::String, str.c_str(), startLineNumber, startColumnNumber)));

			}
			else if (word == "RAW")
			{
				//Consume to next newline as RAW
				uint32_t startLineNumber = lineNumber;
				uint32_t startColumnNumber = columnNumber;
				std::string str = "";
				while (true)
				{
					i++;

					if (words[i] == "\n")
					{
						i--;
						break;
					}
					else
					{
						columnNumber += word.size();
						str.append(words[i]);
					}
				}
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Raw, str.c_str(), startLineNumber, startColumnNumber)));

			}
			else if (word == "OUTPUT")
			{
				m_Tokens->push_back(std::make_shared<Token>(Token(s_KeywordDict.at(word), word.c_str(), lineNumber, columnNumber)));
				//Consume to next newline as RAW
				i++;
				uint32_t startLineNumber = lineNumber;
				uint32_t startColumnNumber = columnNumber;
				std::string str = "";
				while (true)
				{
					i++;

					if (words[i] == "\n")
					{
						i--;
						break;
					}
					else
					{
						columnNumber += word.size();
						str.append(words[i]);
					}
				}
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Raw, str.c_str(), startLineNumber, startColumnNumber)));
			}
			else if (word == "/*")
			{
				//Consume to next *//* as comment
				uint32_t startLineNumber = lineNumber;
				uint32_t startColumnNumber = columnNumber;
				std::string str = "";
				while (true)
				{
					i++;

					if (words[i] == "*//*")
					{
						columnNumber++;
						break;
					}
					else
					{
						if (words[i] == "\n")
						{
							lineNumber += 1;
							columnNumber = 0;
						}
						else
							columnNumber += words[i].size();
						str.append(words[i]);
					}
				}
			}
			else if (word == "//")
			{
				//Consume to next newline as comment
				uint32_t startLineNumber = lineNumber;
				uint32_t startColumnNumber = columnNumber;
				std::string str = "";
				while (true)
				{
					i++;

					if (words[i] == "\n")
					{
						i--;
						break;
					}
					else
					{
						columnNumber += word.size();
						str.append(words[i]);
					}
				}
			}
			else if (word == " " || word == "\t" || word == "\f" || word == "") // Whitespace
			{
			}
			else
			{
				std::smatch m;
				//Use regexes
				if (std::regex_search(word, m, std::regex("[a-zA-Z_][a-zA-Z0-9_]*(\\.[a-zA-Z_][a-zA-Z0-9_]*)*")) && m[0] == word) // Matches identifier regex
				{
					//Matched
					m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Identifier, word.c_str(), lineNumber, columnNumber)));
				}
				else if (std::regex_search(word, m, std::regex("[0-9]+(\\.[0-9]+)?i")) && m[0] == word) // Matches complex regex
				{
					m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Complex, word.c_str(), lineNumber, columnNumber)));
				}
				else if (std::regex_search(word, m, std::regex("[0-9]+\\.[0-9]+")) && m[0] == word) // Matches float regex
				{
					m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Float, word.c_str(), lineNumber, columnNumber)));
				}
				else if (std::regex_search(word, m, std::regex("[0-9]+")) && m[0] == word) // Matches int regex
				{
					m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Int, word.c_str(), lineNumber, columnNumber)));
				}
				else
				{
					//Error unexpected characters.
					SPLW_CRITICAL("Lexical Error: Unexpected characters: {0}", word);
					RegisterLexicalError(LexicalError(0, lineNumber, columnNumber, word.size()));
				}
			}

			i++;

			if (i >= words.size())
			{
				break;
			}
		}
		//SPLW_TRACE(m_Tokens->at(0)->GetContents());
		//SPLW_TRACE(m_Tokens->at(1)->GetContents());
		//return m_Tokens;
		m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::eof, "", lineNumber, columnNumber)));
		return m_Tokens;
	}
	*/

	bool Lexer::charInStr(const std::string& s, char c)
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

	void Lexer::makeToken(std::string tokenContents)
	{
		PROFILE_FUNC();
		if (tokenContents == "\x04")
		{
			//End of file. Final cleanup time, and return error if we are missing something.
			if (flags & 16)
			{
				SPLW_ERROR("Missing double quote at end of file.");
				return;
			}
			else if (flags & 8)
			{
				SPLW_ERROR("Missing single quote at end of file.");
				return;
			}
			else if (flags & 4)
			{
				flags &= 0b11111011;
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Raw, persistent_contents.c_str(), m_StoredLineNumber, m_StoredColumnNumber)));
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Newline, "\n", m_LineNumber, m_ColumnNumber)));
				return;
			}
			else if (flags & 2)
			{
				if (tokenContents == "*/")
				{
					flags &= 0b11111101;
				}
				else
					SPLW_ERROR("Missing end of block comment at end of file.");
				return;
			}
			else if (flags & 1)
			{
				flags &= 0b11111110;
				return;
			}
			m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::eof, "", m_LineNumber, m_ColumnNumber)));
			return;
		}/* ---------------------------------END CLEANUP--------------------------------- */
		else if (flags & 16) // Double quote
		{
			if (tokenContents == "\"")
			{
				flags &= 0b11101111;
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::String, persistent_contents.c_str(), m_StoredLineNumber, m_StoredColumnNumber)));
				persistent_contents = "";
			}
			else
				persistent_contents.append(tokenContents);
		}
		else if (flags & 8) // Single quote
		{
			if (tokenContents == "'")
			{
				flags &= 0b11110111;
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::String, persistent_contents.c_str(), m_StoredLineNumber, m_StoredColumnNumber)));
				persistent_contents = "";
			}
			else
				persistent_contents.append(tokenContents);
		}
		else if (flags & 4) // Raw
		{
			if (tokenContents == "\n")
			{
				flags &= 0b11111011;
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Raw, persistent_contents.c_str(), m_StoredLineNumber, m_StoredColumnNumber)));
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Newline, "\n", m_LineNumber, m_ColumnNumber)));
			}
			else
				persistent_contents.append(tokenContents);
		}
		else if (flags & 2) // Block comment
		{
			if (tokenContents == "*/")
			{
				flags &= 0b11111101;
			}
		}
		else if (flags & 1)  // Comment
		{ 
			if (tokenContents == "\n")
			{
				flags &= 0b11111110;
			}
		}
		else
		{
			if (s_KeywordDict.find(tokenContents) != s_KeywordDict.end() && tokenContents != "RAW" && tokenContents != "OUTPUT" && tokenContents != "/*" && tokenContents != "//")
			{
				//We have a keyword!
				m_Tokens->push_back(std::make_shared<Token>(Token(s_KeywordDict.at(tokenContents), tokenContents.c_str(), m_LineNumber, m_ColumnNumber)));

			}
			else if (tokenContents == "/*")
			{
				flags |= 0b00000010;
			}
			else if (tokenContents == "//")
			{
				flags |= 0b00000001;
			}
			else if (tokenContents == "OUTPUT")
			{
				m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Output, tokenContents.c_str(), m_LineNumber, m_ColumnNumber)));
				flags |= 0b00000100;
				m_StoredColumnNumber = m_ColumnNumber;
				m_StoredLineNumber = m_LineNumber;
			}
			else if (tokenContents == "RAW")
			{
				flags |= 0b00000100;
				m_StoredColumnNumber = m_ColumnNumber;
				m_StoredLineNumber = m_LineNumber;
			}
			else if (tokenContents == "\"")
			{
				flags |= 0b00010000;
				m_StoredColumnNumber = m_ColumnNumber;
				m_StoredLineNumber = m_LineNumber;
			}
			else if (tokenContents == "'")
			{
				flags |= 0b00001000;
				m_StoredColumnNumber = m_ColumnNumber;
				m_StoredLineNumber = m_LineNumber;
			}
			else if (tokenContents == " " || tokenContents == "\t" || tokenContents == "\f" || tokenContents == "") // Whitespace
			{
			}
			else
			{
				std::smatch m;
				//Use regexes
				if (std::regex_search(tokenContents, m, std::regex("[a-zA-Z_][a-zA-Z0-9_]*(\\.[a-zA-Z_][a-zA-Z0-9_]*)*")) && m[0] == tokenContents) // Matches identifier regex
				{
					//Matched
					m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Identifier, tokenContents.c_str(), m_LineNumber, m_ColumnNumber)));
				}
				else if (std::regex_search(tokenContents, m, std::regex("[0-9]+(\\.[0-9]+)?i")) && m[0] == tokenContents) // Matches complex regex
				{
					m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Complex, tokenContents.c_str(), m_LineNumber, m_ColumnNumber)));
				}
				else if (std::regex_search(tokenContents, m, std::regex("[0-9]+\\.[0-9]+")) && m[0] == tokenContents) // Matches float regex
				{
					m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Float, tokenContents.c_str(), m_LineNumber, m_ColumnNumber)));
				}
				else if (std::regex_search(tokenContents, m, std::regex("[0-9]+")) && m[0] == tokenContents) // Matches int regex
				{
					m_Tokens->push_back(std::make_shared<Token>(Token(TokenType::Int, tokenContents.c_str(), m_LineNumber, m_ColumnNumber)));
				}
				else
				{
					//Error unexpected characters.
					SPLW_ERROR("Lexical Error: Unexpected characters: {0}", tokenContents);
					RegisterLexicalError(LexicalError(0, m_LineNumber, m_ColumnNumber, tokenContents.size()));
				}
			}
		}
		if (tokenContents == "\n")
		{
			m_LineNumber += 1;
			m_ColumnNumber = 0;
		}
		else
			m_ColumnNumber += tokenContents.size();
	}

	std::shared_ptr<std::vector<std::shared_ptr<Token>>> Lexer::MakeTokens()
	{
		PROFILE_SCOPE("MakeTokens()");
		std::string s = m_FileString;

		std::string intermediate = "";
		std::vector<char> splitChars = { ' ', '\n', '\t', '\f', '(', ')', '[', ']', '+', '-', '/', '*', '!', '=', '%', '^', '&', '|', '<', '>', ',', '"', '\'', '.' };
		std::vector<std::string> splitDuoStrings = { "/*", "*/", "//", "**", "??", "==", "!=", "<=", ">=", "<<", ">>", "<-", "->", "||", "\\'", "\\\"" };
		std::vector<std::string> splitTrioStrings = { "=/=" };
		
		int i = 0;
		while (true)
		{
			PROFILE_SCOPE("MakeTokens_While_loop");
			char c = s[i];
			std::string duo = std::string(1, c); (i < s.size() - 1) ? duo.append(std::string(1, s[i + (int)1])) : duo.append("");
			std::string trio = std::string(1, c); (i < s.size() - 1) ? trio.append(std::string(1, s[i + (int)1])) : trio.append(""); (i < s.size() - 2) ? trio.append(std::string(1, s[i + 2])) : trio.append("");
			
			if (itemInVect(splitTrioStrings, trio))
			{
				if (intermediate != "")
					makeToken(intermediate);
				intermediate = std::string(1, c);
				(i < s.size() - 1) ? intermediate.append(std::string(1, s[i + 1])) : intermediate.append("");
				(i < s.size() - 2) ? intermediate.append(std::string(1, s[i + 2])) : intermediate.append("");
				makeToken(intermediate);
				intermediate = "";
				i += 2;
			}
			else if (itemInVect(splitDuoStrings, duo))
			{
				if (intermediate != "")
					makeToken(intermediate);
				intermediate = std::string(1, c);
				(i < s.size() - 1) ? intermediate.append(std::string(1, s[i + 1])) : intermediate.append("");
				makeToken(intermediate);
				intermediate = "";
				i++;
			}
			else if (itemInVect(splitChars, c))
			{
				if (c != '.' || charInStr(alphabetCharacters, intermediate[0]))
				{
					if (intermediate != "")
						makeToken(intermediate);
					intermediate = std::string(1, c);
					makeToken(intermediate);
					intermediate = "";
				}
				else
				{
					intermediate.append(std::string(1, c));
				}
			}
			else
			{
				intermediate.append(std::string(1, c));
			}
			i++;
			if (i >= s.size())
				break;
		}
		makeToken(intermediate);
		makeToken("\x004");
		return m_Tokens;
	}

	/*/
	void Lexer::StripComments()
	{
		bool success = true;
		std::string program = m_Tokens->at(0)->GetContents();
		//SPLW_TRACE("Getting program string: {0}", program);
		std::string strippedProgram = "";
		std::regex re("/\\*(.|\n)*?\\*///|//.*");
	/*
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
						//@TODO: Implement line numbers for tokens*/
						//*FUNC*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Function, match->match[12].c_str(), 0, 0)));
						//*x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Identifier, match->match[13].c_str(), 0, 0)));
						//if (match->match[14] != "")
						//{
						//	/*TAKES*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Takes, "TAKES", 0, 0)));
						//	/*type x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[15].c_str(), 0, 0)));
						//}
						//*->*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::ReturnType, match->match[18].c_str(), 0, 0)));
						//*type*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[19].c_str(), 0, 0)));
						//*AS*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::As, "AS", 0, 0)));
						//*body*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[20].c_str(), 0, 0)));
						//*RETURN*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Return, "RETURN", 0, 0)));
						//*rv*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[24].c_str(), 0, 0)));
					//}
					//else if (match->match[0][0] == 'P') //Procedure
					//{
					//	//@TODO: Implement line numbers for tokens
					//	/*PROC*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Procedure, match->match[1].c_str(), 0, 0)));
					//	/*x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Identifier, match->match[2].c_str(), 0, 0)));
					//	if (match->match[3] != "")
					//	{
					//		/*TAKES*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Takes, "TAKES", 0, 0)));
					//		/*type x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[4].c_str(), 0, 0)));
					//	}
					//	/*AS*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::As, "AS", 0, 0)));
					//	/*body*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[20].c_str(), 0, 0)));
					//	/*END*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::EndProc, match->match[9].c_str(), 0, 0)));
					//}
					//else if (match->match[0][0] == 'S') //Struct
					//{
					//	//@TODO: Implement line numbers for tokens
					//	/*STRUCT*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Procedure, match->match[26].c_str(), 0, 0)));
					//	/*x*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::Identifier, match->match[27].c_str(), 0, 0)));
					//	/*AS*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::As, "AS", 0, 0)));
					//	/*body*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, match->match[28].c_str(), 0, 0)));
					//	/*END*/newTokens.push_back(std::make_shared<Token>(Token(TokenType::EndProc, match->match[34].c_str(), 0, 0)));
					//}
					//else
					//{
					//	SPLW_ERROR("Match does not appear to be function, procedure or struct! {0}", match->match[0]);
					//	__debugbreak();
					//}
				//}
				//newTokens.push_back(std::make_shared<Token>(Token(TokenType::UnfinishedToken, matches.back()->suffix.c_str(), 0, 0)));
				//for each (std::shared_ptr<Token> token in newTokens)
				//{
				//	m_Tokens->emplace(m_Tokens->begin() + i, token);
				//}
				//i += newTokens.size();
			//}
			//else
			//	i++;
		//}
		//return success;
	//}
/*
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
	*/
}
