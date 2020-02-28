#include "Lexer.h"
#include <regex>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
//#include "Instrumentor.h"
#include "Log.h"
#include "UtilFunctions.h"

namespace Spliwaca
{
    bool itemInVect(const std::vector<std::string>& v, std::string t)
    {
        for (int i = 0; i < v.size(); i++)
        {
            if (v.at(i) == t)
            {
                return true;
            }
        }
        return false;
    }

    bool itemInVect(const std::vector<char>& v, char t)
    {
        for (int i = 0; i < v.size(); i++)
        {
            if (v.at(i) == t)
            {
                return true;
            }
        }
        return false;
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
        //char* fileContents;

        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                m_FileString.append(line + "\n");
            }
            //SPLW_TRACE("File contents: {0}", m_FileString);
            //std::cout << fileContents << "\n";
        }
        else
        {
            SPLW_ERROR("Could not open source file: {0}", m_FileLocation);
            system("PAUSE");
            exit(2);
        }

        file.close();

        //std::shared_ptr<Token> token;
        //token.reset(new Token(TokenType::UnfinishedToken, fileContents.c_str(), 0, 0));

        //m_Tokens->push_back(token);
    }

    void Lexer::makeToken(std::string tokenContents)
    {
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
        else if (flags == 0)
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
                flags |= 0b00100100;
                m_StoredColumnNumber = m_ColumnNumber;
                m_StoredLineNumber = m_LineNumber;
            }
            else if (tokenContents == "RAW")
            {
                flags |= 0b00100100;
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
            else if (tokenContents == std::string(1, ' ') || tokenContents == "\t" || tokenContents == "\f" || tokenContents == "")// || tokenContents == std::string(1, '\ufeff')) // Whitespace
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
                    RegisterLexicalError(0, m_LineNumber, m_ColumnNumber, tokenContents.size());
                }
            }
        }
        else if (flags & 32) // First char of RAW
        {
            flags &= 0b11011111;
            if (tokenContents != " ")
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
        }
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
            SPLW_CRITICAL("WHY ARE WE IN THE ELSE? THIS SHOULD BE IMPOSSIBLE!!!!!!!!");
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
        std::string s = m_FileString;

        std::string intermediate = "";
        std::vector<char> splitChars = { ' ', '\n', '\t', '\f', '(', ')', '[', ']', '+', '-', '/', '*', '!', '=', '%', '^', '&', '|', '<', '>', ',', '"', '\'', '.', ':' };
        std::vector<std::string> splitDuoStrings = { "/*", "*/", "//", "**", "??", "==", "!=", "<=", ">=", "<<", ">>", "<-", "->", "||", "\\'", "\\\"" };
        std::vector<std::string> splitTrioStrings = { "=/=", "===" };

        int i = 0;
        while (true)
        {
            if ((s[i] & 0xffff) == 0xffef || (s[i] & 0xffff) == 0xffbb || (s[i] & 0xffff) == 0xffbf) {
                i++;
                continue;
            }

            std::string c = std::string(1, s[i]);
            std::string duo = c; (i < s.size() - 1) ? duo += s[i + 1] : duo += "";
            std::string trio = duo; (i < s.size() - 2) ? trio += s[i + 2] : trio += "";

            if (trio == "=/=")//itemInVect(splitTrioStrings, trio))
            {
                if (intermediate != "")
                    makeToken(intermediate);
                intermediate = c + s[i + 1] + s[i+2];
                //intermediate += s[i + 1];
                //intermediate += s[i + 2];
                makeToken(intermediate);
                intermediate = "";
                i += 2;
            }
            else if (itemInVect(splitDuoStrings, duo))
            {
                if (intermediate != "")
                    makeToken(intermediate);
                intermediate = c + s[i+1];
                //intermediate += s[i + 1];
                makeToken(intermediate);
                intermediate = "";
                i++;
            }
            else if (itemInVect(splitChars, c[0]) || s_KeywordDict.find(c) != s_KeywordDict.end())
            {
                if (c != "." || charInStr(alphabetCharacters, intermediate[0]))
                {
                    if (intermediate != "")
                        makeToken(intermediate);
                    intermediate = c;
                    makeToken(intermediate);
                    intermediate = "";
                }
                else
                {
                    intermediate += c;
                }
            }
            else
            {
                intermediate += c;
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



/*
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
                m_FileString.append(line + "\n");
            }
            //SPLW_TRACE("File contents: {0}", m_FileString);
            //std::cout << fileContents << "\n";
        }
        else
        {
            SPLW_ERROR("Could not open source file: {0}", m_FileLocation);
            system("PAUSE");
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
                if (tokenContents == "*//*")
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
        }/* ---------------------------------END CLEANUP--------------------------------- *//*
        else if (flags & 32) // First char of RAW
        {
            flags &= 0b11011111;
            if (tokenContents != " ")
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
        }
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
            if (tokenContents == "*//*")
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
                flags |= 0b00100100;
                m_StoredColumnNumber = m_ColumnNumber;
                m_StoredLineNumber = m_LineNumber;
            }
            else if (tokenContents == "RAW")
            {
                flags |= 0b00100100;
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
                    RegisterLexicalError(0, m_LineNumber, m_ColumnNumber, tokenContents.size());
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
        std::string s = m_FileString;

        std::string intermediate = "";
        std::vector<std::string> splitChars = { " ", "\n", "\t", "\f", "(", ")", "[", "]", "+", "-", "/", "*", "!", "=", "%", "^", "&", "|", "<", ">", ",", "\"", "'", ".", ":" };
        std::vector<std::string> splitDuoStrings = { "/*", "*//*", "//", "**", "??", "==", "!=", "<=", ">=", "<<", ">>", "<-", "->", "||", "\\'", "\\\"" };
        std::vector<std::string> splitTrioStrings = { "=/=" };

        int i = 0;
        while (true)
        {
            std::string c = std::string(1, s[i]);
            std::string duo = c; (i < s.size() - 1) ? duo.append(std::string(1, s[i + 1])) : duo.append("");
            std::string trio = duo; (i < s.size() - 2) ? trio.append(std::string(1, s[i + 2])) : trio.append("");

            if (itemInVect(splitTrioStrings, trio))
            {
                if (intermediate != "")
                    makeToken(intermediate);
                intermediate = c;
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
                intermediate = c;
                (i < s.size() - 1) ? intermediate.append(std::string(1, s[i + 1])) : intermediate.append("");
                makeToken(intermediate);
                intermediate = "";
                i++;
            }
            else if (itemInVect(splitChars, c))
            {
                if (c != "." || charInStr(alphabetCharacters, intermediate[0]))
                {
                    if (intermediate != "")
                        makeToken(intermediate);
                    intermediate = c;
                    makeToken(intermediate);
                    intermediate = "";
                }
                else
                {
                    intermediate.append(c);
                }
            }
            else
            {
                intermediate.append(c);
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
*/