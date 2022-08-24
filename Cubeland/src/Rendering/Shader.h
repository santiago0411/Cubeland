#pragma once

#include "Utils/Filesystem.h"

#include <glm/glm.hpp>

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
		Shader(std::string name, const std::unordered_map<ShaderType, Utils::Filepath>& shaderSources);
		~Shader();

		void Bind() const;

		void UploadMat4(const std::string& uniformName, const glm::mat4& mat4) const;

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
