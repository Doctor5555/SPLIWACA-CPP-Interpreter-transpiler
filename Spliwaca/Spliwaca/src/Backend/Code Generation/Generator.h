#pragma once
#include <memory>
#include "Frontend/Parser/Nodes.h"

namespace Spliwaca
{
	class Generator
	{
	public:
		static std::shared_ptr<Generator> Create(std::shared_ptr<EntryPoint> entryPoint);
		~Generator() = default;

	private:
		Generator()
		{
			
		}


	};
}