#include "pch.h"
#include "Log.h"

#include <vector>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Cubeland
{
	Ref<spdlog::logger> Log::s_Logger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> sinks;
		sinks.emplace_back(CreateRef<spdlog::sinks::stdout_color_sink_mt>());
		sinks.emplace_back(CreateRef<spdlog::sinks::basic_file_sink_mt>("Cubeland.log", true));

		sinks[0]->set_pattern("%^[%T] %n: %v%$");
		sinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_Logger = CreateRef<spdlog::logger>("CUBELAND", begin(sinks), end(sinks));
		spdlog::register_logger(s_Logger);
		s_Logger->set_level(spdlog::level::trace);
		s_Logger->flush_on(spdlog::level::trace);
	}
}
