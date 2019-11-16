#pragma once
#include <string>
#include "Log.h"

namespace Spliwaca
{
	class LexicalError
	{
	public:
		LexicalError(uint8_t errorCode, uint32_t lineNumber, uint32_t columnNumber, uint16_t columnSpan = 1)
			: m_ErrorCode(errorCode), m_LineNumber(lineNumber), m_ColumnNumber(columnNumber), m_ColumnSpan(columnSpan)
		{
		}
		~LexicalError() = default;

		inline const uint8_t  GetErrorCode() const { return m_ErrorCode; }
		inline const uint32_t GetLineNumber() const { return m_LineNumber; }
		inline const uint32_t GetColumnNumber() const { return m_ColumnNumber; }
		inline const uint16_t GetColumnSpan() const { return m_ColumnSpan; }

	private:
		uint8_t m_ErrorCode;
		uint32_t m_LineNumber;
		uint32_t m_ColumnNumber;
		uint16_t m_ColumnSpan;
	};
};
