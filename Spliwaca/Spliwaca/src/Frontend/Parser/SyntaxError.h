#pragma once
#include <cstdint>

namespace Spliwaca
{
	enum class SyntaxErrorType : uint8_t
	{
		expNewline = 0,
		expIdent,
		expStatement,
		expAtom,
		expType,
		expComma,
		expRParen,
		expDo,
		expTo,
		expOf,
		expWith,
		expTakes,
		expReturns,
		expAs,
		expRaw,
		expEndIf,
		expEndFor,
		expEndWhile,
		expEndFunc,
		expEndProc,
		expEndStruct,
		expTypeMod,
		tooManyElse,
	};

	class SyntaxError
	{
	public:
		SyntaxError(SyntaxErrorType errorCode, std::shared_ptr<Token> token)
			: m_ErrorCode(errorCode), m_OffendingToken(token)
		{
		}

		~SyntaxError() = default;

		inline const SyntaxErrorType  GetErrorCode() const { return m_ErrorCode; }
		inline const uint32_t GetLineNumber() const { return m_OffendingToken->GetLineNumber(); }
		inline const uint32_t GetColumnNumber() const { return m_OffendingToken->GetCharacterNumber(); }
		inline const size_t GetColumnSpan() const { return m_OffendingToken->GetContents().length(); }
		inline const TokenType GetTokenType() const { return m_OffendingToken->GetType(); }

		//2^8 = 256 error codes available
		//Code 0: Unexpected Token -- expected newline
		//Code 1: Unexpected Token -- expected identifier
		//Code 2: Unexpected Token -- expected statement
		//Code 3: Unexpected Token -- expected do
		//Code 4: Unexpected Token -- expected "END IF" token pair
		//Code 5: Unexpected Token "ELSE" -- cannot have more than one else in an if tree

	private:
		SyntaxErrorType m_ErrorCode;
		std::shared_ptr<Token> m_OffendingToken;
	};
}