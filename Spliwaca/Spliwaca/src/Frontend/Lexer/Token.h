#pragma once
#include <string>
#include <ostream>

namespace Spliwaca
{
	enum class TokenType
	{
		None = 0,
		Int, //
		Float, //
		Complex, //
		String, //
		Bool, //
		Plus,
		Minus,
		Multiply,
		Divide,
		Intdiv,
		Modulo,
		Power,
		And,
		Or,
		Not,
		Equal,
		NotEqual,
		LessThan,
		GreaterThan,
		LessThanEqual,
		GreaterThanEqual,
		Xor,
		BitwiseAnd,
		BitwiseOr,
		ShiftRight,
		ShiftLeft,
		LParen,
		RParen,
		Function, //
		Procedure, //
		AnonFunc, //
		AnonProc, //
		Struct, //
		As, //
		Takes, //
		Return, //
		EndProc, //
		EndStruct, //
		If, //
		For, //
		While, //
		EndIf, //
		EndFor, //
		EndWhile, //
		Input, //
		Output, //
		Create, //
		Cast, //
		Raw, //kinda - not actually used as a token, just consumes the rest of the line to create a string literal
		Quit,
		Require,
		Set, //
		Type, //
		Identifier,
		UnfinishedToken,
		ReturnValue, //Possibly unused
		BooleanExpr
	};

	inline std::string TokenTypeName(TokenType type)
	{
		switch (type)
		{
		case TokenType::None: return "None";
		case TokenType::Int: return "Int";
		case TokenType::Float: return "Float";
		case TokenType::Complex: return "Complex";
		case TokenType::String: return "String";
		case TokenType::Bool: return "Bool";
		case TokenType::Plus: return "Plus";
		case TokenType::Minus: return "Minus";
		case TokenType::Multiply: return "Multiply";
		case TokenType::Divide: return "Divide";
		case TokenType::Intdiv: return "Intdiv";
		case TokenType::Modulo: return "Modulo";
		case TokenType::Power: return "Power";
		case TokenType::And: return "And";
		case TokenType::Or: return "Or";
		case TokenType::Not: return "Not";
		case TokenType::Equal: return "Equal";
		case TokenType::NotEqual: return "NotEqual";
		case TokenType::LessThan: return "LessThan";
		case TokenType::GreaterThan: return "GreaterThan";
		case TokenType::LessThanEqual: return "LessThanEqual";
		case TokenType::GreaterThanEqual: return "GreaterThanEqual";
		case TokenType::Xor: return "Xor";
		case TokenType::BitwiseAnd: return "BitwiseAnd";
		case TokenType::BitwiseOr: return "BitwiseOr";
		case TokenType::ShiftRight: return "ShiftRight";
		case TokenType::ShiftLeft: return "ShiftLeft";
		case TokenType::LParen: return "LParen";
		case TokenType::RParen: return "RParen";
		case TokenType::Function: return "Function";
		case TokenType::Procedure: return "Procedure";
		case TokenType::AnonFunc: return "AnonFunc";
		case TokenType::AnonProc: return "AnonProc";
		case TokenType::Struct: return "Struct";
		case TokenType::As: return "As";
		case TokenType::Return: return "Return";
		case TokenType::EndProc: return "EndProc";
		case TokenType::EndStruct: return "EndStruct";
		case TokenType::If: return "If";
		case TokenType::For: return "For";
		case TokenType::While: return "While";
		case TokenType::EndIf: return "EndIf";
		case TokenType::EndFor: return "EndFor";
		case TokenType::EndWhile: return "EndWhile";
		case TokenType::Input: return "Input";
		case TokenType::Output: return "Output";
		case TokenType::Create: return "Create";
		case TokenType::Cast: return "Cast";
		case TokenType::Raw: return "Raw";
		case TokenType::Quit: return "Quit";
		case TokenType::Require: return "Require";
		case TokenType::Set: return "Set";
		case TokenType::Type: return "Type";
		case TokenType::Identifier: return "Identifier";
		case TokenType::UnfinishedToken: return "UnfinishedToken";
		case TokenType::ReturnValue: return "ReturnValue";
		case TokenType::BooleanExpr: return "BooleanExpr";
		}
	}
	
	class Token
	{
	public:
		Token(TokenType tokenType, const char* contents, uint32_t lineNumber, uint32_t characterNumber)
			: m_Type(tokenType), m_Contents(contents), m_LineNumber(lineNumber), m_CharacterNumber(characterNumber)
		{  }

		virtual ~Token() = default;

		inline TokenType GetType() { return m_Type; }
		inline std::string GetContents() { return m_Contents; }
		inline uint32_t GetLineNumber() { return m_LineNumber; }
		inline uint32_t GetCharacterNumber() { return m_CharacterNumber; }

		inline const std::string& ToString() const { return "[Token " + std::to_string(m_LineNumber) + "," + std::to_string(m_CharacterNumber) + ": " + TokenTypeName(m_Type) + ": " + m_Contents + "]"; }
	private:
		TokenType m_Type;
		std::string m_Contents;
		uint32_t m_LineNumber;
		uint32_t m_CharacterNumber;
	};

	inline std::ostream& operator<<(std::ostream& os, const Token& t)
	{
		// [Token: Int: 0]
		os << t.ToString();
		return os;
	}
}
