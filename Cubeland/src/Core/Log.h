#pragma once

#include <spdlog/spdlog.h>

#include "Core/Base.h"

namespace Cubeland
{
	class Log
	{
	public:
		Log() = delete;
		~Log() = delete;

		static void Init();
		static Ref<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static Ref<spdlog::logger> s_Logger;
	};
}

#define CL_LOG_TRACE(...)		::Cubeland::Log::GetLogger()->trace(__VA_ARGS__)
#define CL_LOG_DEBUG(...)		::Cubeland::Log::GetLogger()->debug(__VA_ARGS__)
#define CL_LOG_INFO(...)		::Cubeland::Log::GetLogger()->info(__VA_ARGS__)
#define CL_LOG_WARN(...)		::Cubeland::Log::GetLogger()->warn(__VA_ARGS__)
#define CL_LOG_ERROR(...)		::Cubeland::Log::GetLogger()->error(__VA_ARGS__)
#define CL_LOG_CRITICAL(...)	::Cubeland::Log::GetLogger()->critical(__VA_ARGS__)