#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#pragma warning(push, 0)
#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/bundled/format.h>
#pragma warning(pop)

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

namespace std::filesystem
{
	class path;
}

template<glm::length_t L, typename T, glm::qualifier Q>
struct fmt::formatter<glm::vec<L, T, Q>> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template <typename FormatContext>
	auto format(const glm::vec<L, T, Q>& vector, FormatContext& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{}", glm::to_string(vector));
	}
};

template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
struct fmt::formatter<glm::mat<C, R, T, Q>> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template <typename FormatContext>
	auto format(const glm::mat<C, R, T, Q>& matrix, FormatContext& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{}", glm::to_string(matrix));
	}
};

template<typename T, glm::qualifier Q>
struct fmt::formatter<glm::qua<T, Q>> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template <typename FormatContext>
	auto format(const glm::qua<T, Q>& quaternion, FormatContext& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{}", glm::to_string(quaternion));
	}
};

template<>
struct fmt::formatter<std::filesystem::path> : formatter<std::string_view> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template <typename FormatContext>
	auto format(const std::filesystem::path& path, FormatContext& ctx) -> decltype(ctx.out()) {
		return formatter<std::string_view>::format(path.string(), ctx);
	}
};

#define CL_LOG_TRACE(...)		::Cubeland::Log::GetLogger()->trace(__VA_ARGS__)
#define CL_LOG_DEBUG(...)		::Cubeland::Log::GetLogger()->debug(__VA_ARGS__)
#define CL_LOG_INFO(...)		::Cubeland::Log::GetLogger()->info(__VA_ARGS__)
#define CL_LOG_WARN(...)		::Cubeland::Log::GetLogger()->warn(__VA_ARGS__)
#define CL_LOG_ERROR(...)		::Cubeland::Log::GetLogger()->error(__VA_ARGS__)
#define CL_LOG_CRITICAL(...)	::Cubeland::Log::GetLogger()->critical(__VA_ARGS__)