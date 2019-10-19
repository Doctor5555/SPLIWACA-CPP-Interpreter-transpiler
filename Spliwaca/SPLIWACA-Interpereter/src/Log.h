#pragma once

#ifndef LOG_H_SUPERNOVA_CORE_GUARD
#define LOG_H_SUPERNOVA_CORE_GUARD

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Supernova {
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

// Client log macros			
#define SN_TRACE(...)         ::Supernova::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SN_INFO(...)          ::Supernova::Log::GetClientLogger()->info(__VA_ARGS__)
#define SN_WARN(...)          ::Supernova::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SN_ERROR(...)         ::Supernova::Log::GetClientLogger()->error(__VA_ARGS__)
#define SN_CRITICAL(...)      ::Supernova::Log::GetClientLogger()->critical(__VA_ARGS__)
#else
//Core log macros
#define SN_CORE_TRACE(...)
#define SN_CORE_INFO(...)     
#define SN_CORE_WARN(...)     
#define SN_CORE_ERROR(...)    
#define SN_CORE_CRITICAL(...) 

// Client log macros			
#define SN_TRACE(...)         
#define SN_INFO(...)          
#define SN_WARN(...)          
#define SN_ERROR(...)         
#define SN_CRITICAL(...)      
#endif

#endif /*LOG_H_SUPERNOVA_CORE_GUARD*/