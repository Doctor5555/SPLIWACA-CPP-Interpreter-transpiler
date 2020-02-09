#pragma once

#ifndef LOG_H_SUPERNOVA_CORE_GUARD
#define LOG_H_SUPERNOVA_CORE_GUARD

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Spliwaca
{
	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#ifdef SPLW_DEBUG

// Log macros			
#define SPLW_TRACE(...)         ::Spliwaca::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SPLW_INFO(...)          ::Spliwaca::Log::GetClientLogger()->info(__VA_ARGS__)
#define SPLW_WARN(...)          ::Spliwaca::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SPLW_ERROR(...)         ::Spliwaca::Log::GetClientLogger()->error(__VA_ARGS__)
#define SPLW_CRITICAL(...)      ::Spliwaca::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LOG_INIT()              ::Spliwaca::Log::Init()

#else
#ifdef SPLW_DIST

#define SPLW_TRACE(...)         //::Spliwaca::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SPLW_INFO(...)          //::Spliwaca::Log::GetClientLogger()->info(__VA_ARGS__)
#define SPLW_WARN(...)          ::Spliwaca::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SPLW_ERROR(...)         ::Spliwaca::Log::GetClientLogger()->error(__VA_ARGS__)
#define SPLW_CRITICAL(...)      ::Spliwaca::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LOG_INIT()              ::Spliwaca::Log::Init()

#else
// Log macros			
#define SPLW_TRACE(...)         //::Spliwaca::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SPLW_INFO(...)          
#define SPLW_WARN(...)          
#define SPLW_ERROR(...)         
#define SPLW_CRITICAL(...)
#define LOG_INIT()

#endif

#endif

#endif /*LOG_H_SUPERNOVA_CORE_GUARD*/