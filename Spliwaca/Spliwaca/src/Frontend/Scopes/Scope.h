#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <Log.h>

namespace Spliwaca
{
	enum class ScopeType
	{
		Main = 0,
		If,
		For,
		While,
		Function,
		Procedure,
		Anonf,
		Anonp,
		Struct
	};

	class ScopeEntry
	{
	public:
		ScopeEntry(std::string symbol, int lineNumber)
			: m_Symbol(symbol), m_LineNumber(lineNumber)
		{
		}

		ScopeEntry()
			: m_Symbol(""), m_LineNumber(0) { }

		const std::string GetSymbol() const { return m_Symbol; }
		const int GetLineNumber() const { return m_LineNumber; }
	private:
		std::string m_Symbol;
		uint32_t m_LineNumber;
	};

	class Scope
	{
	public:
		Scope(std::string name, uint32_t lineNumber, ScopeType type)
			: m_Name(name), m_StartLineNumber(lineNumber), m_Type(type)
		{
		}

		void CloseScope(uint32_t lineNumber)
		{
			m_EndLineNumber = lineNumber;
			m_Readonly = true;
		}

		void AddEntry(std::string symbol, uint32_t lineNumber)
		{
			if (m_Readonly)
			{
				SPLW_WARN("Attempted to add an entry to a closed scope. This should not happen.");
				return;
			}
			size_t hashed = std::hash<std::string>()(symbol);
			m_Entries.insert(std::map<size_t, ScopeEntry>::value_type(hashed, ScopeEntry(symbol, lineNumber)));
		}

		std::shared_ptr<Scope> AddSubScope(std::string name, uint32_t lineNumber, ScopeType type)
		{
			if (m_Readonly)
			{
				SPLW_WARN("Attempted to add a sub-scope to a closed scope. This should not happen.");
				return nullptr;
			}
			m_SubScopes.push_back(std::make_shared<Scope>(name, lineNumber, type));
			return m_SubScopes.at(m_SubScopes.size() - 1);
		}

		uint32_t FindIdent(std::string ident)
		{
			size_t hashed = std::hash<std::string>()(ident);
			if (m_Entries.find(hashed) != m_Entries.end())
				return m_Entries[hashed].GetLineNumber();
			else
				return false;
		}

		ScopeType GetType() { return m_Type; }

	private:
		std::map<size_t, ScopeEntry> m_Entries;
		std::vector<std::shared_ptr<Scope>> m_SubScopes;

		std::string m_Name;
		ScopeType m_Type;
		uint32_t m_StartLineNumber;
		uint32_t m_EndLineNumber;
		bool m_Readonly = false;
	};
}