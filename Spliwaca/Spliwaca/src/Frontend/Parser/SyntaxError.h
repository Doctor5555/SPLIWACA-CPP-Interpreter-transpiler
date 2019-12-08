#pragma once
#include <cstdint>

namespace Spliwaca
{
	class SyntaxError
	{
	public:
		SyntaxError(uint8_t errorCode, uint32_t lineNumber, uint32_t columnNumber, uint16_t columnSpan = 1)
			: m_ErrorCode(errorCode), m_LineNumber(lineNumber), m_ColumnNumber(columnNumber), m_ColumnSpan(columnSpan)
		{
		}
		~SyntaxError() = default;

		inline const uint8_t  GetErrorCode() const { return m_ErrorCode; }
		inline const uint32_t GetLineNumber() const { return m_LineNumber; }
		inline const uint32_t GetColumnNumber() const { return m_ColumnNumber; }
		inline const uint16_t GetColumnSpan() const { return m_ColumnSpan; }

		//2^8 = 256 error codes available
		//Code 0: Unexpected Token -- expected newline
		//Code 1: Unexpected Token -- expected identifier

	private:
		uint8_t m_ErrorCode;
		uint32_t m_LineNumber;
		uint32_t m_ColumnNumber;
		uint16_t m_ColumnSpan;
	};
}