#pragma once

#include "Utils/Filesystem.h"

namespace Cubeland
{
	enum class ShaderType
	{
		Vertex,
		Tessellation,
		Geometry,
		Fragment,
		Compute
	};

	class Shader
	{
	public:
		Shader(std::string&& name, std::unordered_map<ShaderType, Utils::Filepath>&& shaderSources);
		~Shader();

		void Bind() const;

	private:
		struct ShaderSourceInfo
		{
			uint32_t GlStage;
			std::string Source;
			std::filesystem::path CacheOutPath;
		};

		void CompileOpenGLBinaries(const ShaderSourceInfo& shaderSourceInfo);
		void CreateProgram();

	private:
		uint32_t m_RendererId = 0;
		std::string m_Name;

		std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSpirv;
	};
}
