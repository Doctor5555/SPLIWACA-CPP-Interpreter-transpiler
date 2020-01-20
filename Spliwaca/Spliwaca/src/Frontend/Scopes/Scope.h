#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>

namespace Spliwaca
{
	class Scope
	{
	public:
		Scope(std::string name, uint32_t lineNumber)
			: m_Name(name), m_LineNumber(lineNumber)
		{
		}

		void AddEntry(std::string symbol, uint32_t lineNumber)
		{
			size_t hashed = std::hash<std::string>()(symbol);
			m_Entries.insert(std::map<size_t, ScopeEntry>::value_type(hashed, ScopeEntry(symbol, lineNumber)));
		}

		std::shared_ptr<Scope> AddSubScope(std::string name, uint32_t lineNumber)
		{
			m_SubScopes.push_back(std::make_shared<Scope>(name, lineNumber));
			return m_SubScopes.at(m_SubScopes.size() - 1);
		}
	private:
		std::map<size_t, ScopeEntry> m_Entries;
		std::vector<std::shared_ptr<Scope>> m_SubScopes;

		std::string m_Name;
		uint32_t m_LineNumber;
	};

	class ScopeEntry
	{
	public:
		ScopeEntry(std::string symbol, int lineNumber)
			: m_Symbol(symbol), m_LineNumber(lineNumber) { }

		const std::string GetSymbol() const { return m_Symbol; }
		const int GetLineNumber() const { return m_LineNumber; }
	private:
		std::string m_Symbol;
		uint32_t m_LineNumber;
	};
}