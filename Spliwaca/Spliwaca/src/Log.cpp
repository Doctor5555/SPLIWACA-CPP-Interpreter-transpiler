//#include "splwpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Spliwaca
{
	
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%H:%M:%S:%f] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("Supernova");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}