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
			exit(2);
		}

		file.close();

		//std::shared_ptr<Token> token;
		//token.reset(new Token(TokenType::UnfinishedToken, fileContents.c_str(), 0, 0));

		//m_Tokens->push_back(token);
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
				persistent_contents = "";
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
		std::vector<char> splitChars = { ' ', '\n', '\t', '\f', '(', ')', '[', ']', '+', '-', '/', '*', '!', '=', '%', '^', '&', '|', '<', '>', ',', '"', '\'', '.', ':' };
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
}
