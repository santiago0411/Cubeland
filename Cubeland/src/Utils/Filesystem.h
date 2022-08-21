#pragma once

#include <filesystem>
#include <vector>

namespace Cubeland::Utils
{
	using Filepath = std::filesystem::path;

	class Filesystem
	{
	public:
		static std::string ReadFileToString(const Filepath& filepath);
		static std::vector<uint8_t> ReadFileBytes(const Filepath& filepath);
		static std::vector<uint32_t> ReadFileBytes32Bit(const Filepath& filepath);
		static void CreateDirectoryIfNeeded(const Filepath& directoryPath);
		static bool WriteBytes(const Filepath& filepath, const void* data, size_t size);
	};
}
