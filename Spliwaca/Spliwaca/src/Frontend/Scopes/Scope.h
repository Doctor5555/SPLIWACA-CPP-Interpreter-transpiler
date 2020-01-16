#pragma once
#include <vector>
#include <map>
#include <string>

namespace Spliwaca
{
	class Scope
	{
	public:

	private:
		std::map<ScopeEntry> m_Entries;
		std::vector<Scope> m_SubScopes;
	};

	class ScopeEntry
	{
	public:
		
	private:
		std::string m_Symbol;
		int m_LineNumber
	};
}