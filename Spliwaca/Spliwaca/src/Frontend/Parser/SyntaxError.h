#pragma once
#include <cstdint>

namespace Spliwaca
{
	enum class SyntaxErrorType : uint8_t
	{
		expNewline = 0,   // 0
		expIdent,         // 1
		expStatement,     // 2
		expAtom,          // 3
		expType,          // 4
		expComma,         // 5
		expRParen,        // 6
		expRSquareParen,  // 7
		expDo,	          // 8
		expTo,	          // 9
		expOf,	          // 10
		expWith,          // 11
		expTakes,         // 12
		expReturns,       // 13
		expAs,	          // 14
		expRaw,	          // 15
		expEndIf,         // 16
		expEndFor,        // 17
		expEndWhile,      // 18
		expEndFunc,	      // 19
		expEndProc,	      // 20
		expEndStruct,     // 21
		expTypeMod,	      // 22
		tooManyElse,      // 23
		unexpEndFunc,     // 24
		unexpEndProc,     // 25
		unexpEndIf,	      // 26
		unexpEndFor,      // 27
		unexpEndWhile,    // 28
		unexpEndStruct,   // 29
		unexpElseIf,      // 30
		inconsistentDict  // 31
	};

	class SyntaxError
	{
	public:
		SyntaxError(SyntaxErrorType errorCode, std::shared_ptr<Token> token)
			: m_ErrorCode(errorCode), m_LineNumber(token->GetLineNumber()), m_ColumnNumber(token->GetCharacterNumber()),
			  m_ColumnSpan(token->GetContents().length()), m_TokenType(token->GetType())
		{}

		SyntaxError(SyntaxErrorType errorCode, uint32_t lineNumber, uint32_t columnNumber, size_t columnSpan, Spliwaca::TokenType type)
			: m_ErrorCode(errorCode), m_LineNumber(lineNumber), m_ColumnNumber(columnNumber),
			m_ColumnSpan(columnSpan), m_TokenType(type)
		{}

		~SyntaxError() = default;

		inline const SyntaxErrorType  GetErrorCode() const { return m_ErrorCode; }
		inline const uint32_t GetLineNumber() const { return m_LineNumber; }
		inline const uint32_t GetColumnNumber() const { return m_ColumnNumber; }
		inline const size_t GetColumnSpan() const { return m_ColumnSpan; }
		inline const TokenType GetTokenType() const { return m_TokenType; }

		//2^8 = 256 error codes available
		//Code 0: Unexpected Token -- expected newline
		//Code 1: Unexpected Token -- expected identifier
		//Code 2: Unexpected Token -- expected statement
		//Code 3: Unexpected Token -- expected do
		//Code 4: Unexpected Token -- expected "END IF" token pair
		//Code 5: Unexpected Token "ELSE" -- cannot have more than one else in an if tree

	private:
		SyntaxErrorType m_ErrorCode;
		uint32_t m_LineNumber, m_ColumnNumber;
		size_t m_ColumnSpan;
		TokenType m_TokenType;
		//std::shared_ptr<Token> m_OffendingToken;
	};
}