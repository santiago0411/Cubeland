#include "pch.h"
#include "Filesystem.h"

#include <fstream>

#include "Core/Log.h"

namespace Cubeland::Utils
{
	std::string Filesystem::ReadFileToString(const Filepath& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			const size_t size = in.tellg();

			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				CL_LOG_ERROR("Could not read from file '{0}'", filepath.string());
			}
		}
		else
		{
			CL_LOG_ERROR("Could not open shader file '{0}'", filepath.string());
		}

		return result;
	}

	std::vector<uint8_t> Filesystem::ReadFileBytes(const Filepath& filepath)
	{
		std::vector<uint8_t> data;
		std::ifstream stream(filepath, std::ios::in | std::ios::binary);

		if (stream.is_open())
		{
			stream.seekg(0, std::ios::end);
			const size_t size = stream.tellg();
			stream.seekg(0, std::ios::beg);

			data.resize(size);
			stream.read((char*)data.data(), size);
			stream.close();
		}

		return data;
	}

	std::vector<uint32_t> Filesystem::ReadFileBytes32Bit(const Filepath& filepath)
	{
		std::vector<uint32_t> data;
		std::ifstream stream(filepath, std::ios::in | std::ios::binary);

		if (stream.is_open())
		{
			stream.seekg(0, std::ios::end);
			const size_t size = stream.tellg();
			stream.seekg(0, std::ios::beg);

			data.resize(size / sizeof(uint32_t));
			stream.read((char*)data.data(), size);
			stream.close();
		}

		return data;
	}

	void Filesystem::CreateDirectoryIfNeeded(const Filepath& directoryPath)
	{
		if (!exists(directoryPath))
			create_directories(directoryPath);
	}

	bool Filesystem::WriteBytes(const Filepath& filepath, const void* data, size_t size)
	{
		std::ofstream stream(filepath, std::ios::out | std::ios::binary);

		if (stream.is_open())
		{
			stream.write((char*)data, size);
			stream.flush();
			stream.close();
			return true;
		}

		return false;
	}
}
