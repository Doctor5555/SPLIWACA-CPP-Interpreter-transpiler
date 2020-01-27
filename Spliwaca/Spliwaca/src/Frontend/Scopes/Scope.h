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
		ScopeEntry(std::string symbol, int lineNumber, VarType type)
			: m_Symbol(symbol), m_LineNumber(lineNumber), m_AmbiguousType(false), m_SymbolType(type)
		{
		}

		ScopeEntry()
			: m_Symbol(""), m_LineNumber(0), m_AmbiguousType(false), m_SymbolType(VarType::None) { }

		const std::string GetSymbol() const { return m_Symbol; }
		const int GetLineNumber() const { return m_LineNumber; }
		const VarType GetSymbolType() const { return m_SymbolType; }

		void Ambiguous() { m_AmbiguousType = true; }
	private:
		std::string m_Symbol;
		uint32_t m_LineNumber;
		VarType m_SymbolType;
		bool m_AmbiguousType;
	};

	class Scope
	{
	public:
		Scope(std::string name, uint32_t lineNumber, ScopeType type)
			: m_Name(name), m_StartLineNumber(lineNumber), m_Type(type)
		{
			if (m_Type == ScopeType::Anonf || m_Type == ScopeType::Anonp || m_Type == ScopeType::Function || m_Type == ScopeType::Procedure)
				m_BlockingScope = true;
			else
				m_BlockingScope = false;
		}

		void CloseScope(uint32_t lineNumber)
		{
			m_EndLineNumber = lineNumber;
			m_Readonly = true;
		}

		void AddEntry(std::string symbol, uint32_t lineNumber, VarType type)
		{
			if (m_Readonly)
			{
				SPLW_WARN("Attempted to add an entry to a closed scope. This should not happen.");
				return;
			}
			size_t hashed = std::hash<std::string>()(symbol);
			m_Entries.insert(std::map<size_t, std::shared_ptr<ScopeEntry>>::value_type(hashed, std::make_shared<ScopeEntry>(symbol, lineNumber, type)));
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

		std::shared_ptr<ScopeEntry> FindIdent(std::shared_ptr<IdentNode> ident)
		{
			size_t hashed = std::hash<std::string>()(ident->GetContents());
			if (m_Entries.find(hashed) != m_Entries.end())
				return m_Entries[hashed];
			else if (ident->GetIdentAccessNum() == 1)
				return nullptr;
			return nullptr;
		}

		ScopeType GetType() { return m_Type; }

	private:
		std::map<size_t, std::shared_ptr<ScopeEntry>> m_Entries;
		std::vector<std::shared_ptr<Scope>> m_SubScopes;
		std::shared_ptr<Scope> m_ParentScope;

		std::string m_Name;
		ScopeType m_Type;
		uint32_t m_StartLineNumber;
		uint32_t m_EndLineNumber;
		bool m_Readonly = false;
		bool m_BlockingScope;
	};

	class StructScope : Scope
	{
	public:
		StructScope(std::string name, uint32_t lineNumber, ScopeType type)
			: Scope(name, lineNumber, type) {}

	private:
		bool m_Accessible = false;
	};

	class MainScope : Scope
	{
	public:
		MainScope(std::string name, uint32_t lineNumber, ScopeType type)
			: Scope(name, lineNumber, type) {}

	private:
		std::map<size_t, StructScope> m_Structs;
	};
}