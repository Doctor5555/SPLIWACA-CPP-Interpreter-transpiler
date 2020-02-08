#pragma once
#include "Token.h"
#include <memory>
#include <vector>
#include <map>
#include <sstream>

namespace Spliwaca
{
	class Lexer
	{
	public:
		static std::shared_ptr<Lexer> Create(std::string file);
		~Lexer() = default;

		std::shared_ptr<std::vector<std::shared_ptr<Token>>> MakeTokens();
		inline const std::string GetFileString() const { return m_FileString; }
		inline const std::vector<std::string> GetSplitFileString() const { return split(m_FileString, '\n'); }

	private:
		Lexer(std::string file);

		void makeToken(std::string tokenContents);

		template <typename Out>
		static void split(const std::string& s, char delim, Out result)
		{
			std::istringstream iss(s);
			std::string item;
			while (std::getline(iss, item, delim))
			{
				*result++ = item;
			}
		}

		static std::vector<std::string> split(const std::string& s, char delim)
		{
			std::vector<std::string> elems;
			split(s, delim, std::back_inserter(elems));
			return elems;
		}

	private:
		std::string m_FileLocation;
		std::shared_ptr<std::vector<std::shared_ptr<Token>>> m_Tokens;

		uint32_t m_LineNumber = 0, m_ColumnNumber = 0;
		uint32_t m_StoredLineNumber = 0, m_StoredColumnNumber = 0;

		//LEXER STATE FLAGS
		// double_quote single_quote raw block_comment line_comment
		char flags = 0;
		std::string persistent_contents;

		std::string m_FileString;

		const std::string alphabetCharacters = {
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
		};

		const std::map<std::string, TokenType> s_KeywordDict = {
			{"INT",                 TokenType::Type},
			{"Int",                 TokenType::Type},
			{"int",                 TokenType::Type},
			{"INTEGER",             TokenType::Type},
			{"Integer",             TokenType::Type},
			{"integer",             TokenType::Type},
			{"FLOAT",               TokenType::Type},
			{"Float",               TokenType::Type},
			{"float",               TokenType::Type},
			{"REAL",                TokenType::Type},
			{"Real",                TokenType::Type},
			{"real",                TokenType::Type},
			{"NUMBER",              TokenType::Type},
			{"Number",              TokenType::Type},
			{"number",              TokenType::Type},
			{"COMPLEX",             TokenType::Type},
			{"Complex",             TokenType::Type},
			{"complex",             TokenType::Type},
			{"STRING",              TokenType::Type},
			{"String",              TokenType::Type},
			{"string",              TokenType::Type},
			{"STR",                 TokenType::Type},
			{"Str",                 TokenType::Type},
			{"str",                 TokenType::Type},
			{"DICT",                TokenType::Type},
			{"Dict",                TokenType::Type},
			{"dict",                TokenType::Type},
			{"DICTIONARY",          TokenType::Type},
			{"Dictionary",          TokenType::Type},
			{"dictionary",          TokenType::Type},
			{"MAP",                 TokenType::Type},
			{"Map",                 TokenType::Type},
			{"map",                 TokenType::Type},
			{"MAPPING",             TokenType::Type},
			{"Mapping",             TokenType::Type},
			{"mapping",             TokenType::Type},
			{"LIST",                TokenType::Type},
			{"List",                TokenType::Type},
			{"list",                TokenType::Type},
			{"ARRAY",               TokenType::Type},
			{"Array",               TokenType::Type},
			{"array",               TokenType::Type},
			{"TUPLE",               TokenType::Type},
			{"Tuple",               TokenType::Type},
			{"tuple",               TokenType::Type},
			{"TRUE",                TokenType::True},
			{"True",                TokenType::True},
			{"true",                TokenType::True},
			{"FALSE",               TokenType::False},
			{"False",               TokenType::False},
			{"false",               TokenType::False},
			{"NULL",                TokenType::None},
			{"Null",                TokenType::None},
			{"null",                TokenType::None},
			{"NONE",                TokenType::None},
			{"None",                TokenType::None},
			{"none",                TokenType::None},
			{"EMPTY",               TokenType::None},
			{"Empty",               TokenType::None},
			{"empty",               TokenType::None},
			{"INFINITY",            TokenType::Float},
			{"Infinity",            TokenType::Float},
			{"infinity",            TokenType::Float},
			{"NAN",                 TokenType::Float},
			{"NaN",                 TokenType::Float},
			{"nan",                 TokenType::Float},
			{"IS",                  TokenType::Is},
			{"≣",                   TokenType::Is},
			{"≡",                   TokenType::Is},
			{"NOT",                 TokenType::Not},
			{"!",                   TokenType::Not},
			{"AND",                 TokenType::And},
			{"&&",                  TokenType::And},
			{"∧",                  TokenType::And},
			{"OR",                  TokenType::Or},
			{"∨",                  TokenType::Or},
			{"||",                  TokenType::Or},
			{"=",                   TokenType::Equal},
			{"==",                  TokenType::Equal},
			{"EQUALS",              TokenType::Equal},
			{"!=",                  TokenType::NotEqual},
			{"≠",                   TokenType::NotEqual},
			{"=/=",                 TokenType::NotEqual},
			{"<",                   TokenType::LessThan},
			{"≨",                   TokenType::LessThan},
			{"≱",                   TokenType::LessThan},
			{">",                   TokenType::GreaterThan},
			{"≩",                   TokenType::GreaterThan},
			{"≰",                   TokenType::GreaterThan},
			{"<=",                  TokenType::LessThanEqual},
			{"≤",                   TokenType::LessThanEqual},
			{"⩽",                   TokenType::LessThanEqual},
			{"≦",                  TokenType::LessThanEqual},
			{"≯",                   TokenType::LessThanEqual},
			{"⩾",                   TokenType::GreaterThanEqual},
			{">=",                  TokenType::GreaterThanEqual},
			{"≥",                   TokenType::GreaterThanEqual},
			{"≧",                  TokenType::GreaterThanEqual},
			{"≮",                   TokenType::GreaterThanEqual},
			{"*",                   TokenType::Multiply},
			{"**",                  TokenType::Power},
			{"/",                   TokenType::Divide},
			{"DIVI",                TokenType::Intdiv},
			{"+",                   TokenType::Plus},
			{"-",                   TokenType::Minus},
			{"%",                   TokenType::Modulo},
			{"^",                   TokenType::Xor},
			{"&",                   TokenType::BitwiseAnd},
			{"|",                   TokenType::BitwiseOr},
			{">>",                  TokenType::ShiftRight},
			{"<<",                  TokenType::ShiftLeft},
			{"(",                   TokenType::LParen},
			{")",                   TokenType::RParen},
			{"RAW",                 TokenType::Raw},
			{"PLAINTEXT",           TokenType::Raw},
			{"INPUT",               TokenType::Input},
			{"POS",                 TokenType::PositiveTypeMod},
			{"POSITIVE",            TokenType::PositiveTypeMod},
			{"NEG",                 TokenType::NegativeTypeMod},
			{"NEGATIVE",            TokenType::NegativeTypeMod},
			{"NONZERO",             TokenType::NonZeroTypeMod},
			{"OUTPUT",              TokenType::Output},
			{"PRINT",               TokenType::Output},
			{"CREATE",              TokenType::Create},
			{"CAST",                TokenType::Cast},
			{"CALL",                TokenType::Call},
			{"WITH",                TokenType::With},
			{"QUIT",                TokenType::Quit},
			{"EXIT",                TokenType::Quit},
			{"STOP",                TokenType::Quit},
			{"REQUIRE",             TokenType::Require},
			{"INCREMENT",           TokenType::Increment},
			{"INC",                 TokenType::Increment},
			{"DECREMENT",           TokenType::Decrement},
			{"DEC",                 TokenType::Decrement},
			{"IF",                  TokenType::If},
			{"DO",                  TokenType::Do},
			{"ELSE",                TokenType::Else},
			{"FUNCTION",            TokenType::Function},
			{"FUNC",                TokenType::Function},
			{"TAKES",               TokenType::Takes},
			{"<-",                  TokenType::Takes},
			{"DEFAULT",             TokenType::Default},
			{"RETURNS",             TokenType::ReturnType},
			{"->",                  TokenType::ReturnType},
			{"AS",                  TokenType::As},
			{"RETURN",              TokenType::Return},
			{"PROCEDURE",           TokenType::Procedure},
			{"END",                 TokenType::End},
			{"ANONF",               TokenType::AnonFunc},
			{"ANONFUNC",            TokenType::AnonFunc},
			{"ANONFUNCTION",        TokenType::AnonFunc},
			{"ANONYMOUSF",          TokenType::AnonFunc},
			{"ANONYMOUSFUNC",       TokenType::AnonFunc},
			{"ANONYMOUSFUNCTION",   TokenType::AnonFunc},
			{"ANONP",               TokenType::AnonProc},
			{"ANONPROC",            TokenType::AnonProc},
			{"ANONPROCEDURE",       TokenType::AnonProc},
			{"ANONYMOUSP",          TokenType::AnonProc},
			{"ANONYMOUSPROC",       TokenType::AnonProc},
			{"ANONYMOUSPROCEDURE",  TokenType::AnonProc},
			{"SET",                 TokenType::Set},
			{"TO",                  TokenType::To},
			{"FOR",                 TokenType::For},
			{"OF",                  TokenType::Of},
			{"WHILE",               TokenType::While},
			{"STRUCTURE",           TokenType::Struct},
			{"STRUCT",              TokenType::Struct},
			{"BREAK",               TokenType::Break},
			{"IMPORT",              TokenType::Import},
			{"//",                  TokenType::SingleLineComment},
			{"/*",                  TokenType::StartMultiLineComment},
			{"*/",                  TokenType::EndMultiLineComment},
			{":",                   TokenType::DictEquator},
			{"\n",                  TokenType::Newline},
			{",",                   TokenType::Comma},
			{".",                   TokenType::VarAccessOp}
			/*{"\f",                  TokenType::Whitespace},
			//{"\u200b",              TokenType::Whitespace},
			{"\t",                  TokenType::Whitespace},
			{" ",                   TokenType::Whitespace}*/
		};
	};
}
