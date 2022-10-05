#pragma once

#include "Rendering/Buffer.h"
#include "Utils/Filesystem.h"

#include <glm/glm.hpp>
#include <map>

namespace Cubeland
{
	enum class ShaderType
	{
		Vertex,
		Fragment
	};

	using ShaderFilesMap = std::unordered_map<ShaderType, Utils::Filepath>;

	class Shader
	{
	public:
		Shader(std::string name, const ShaderFilesMap& shaderSources);
		~Shader();

		void Bind() const;

		const Ref<UniformBuffer>& GetUniformBuffer(uint32_t binding) const
		{
			CL_ASSERT(m_UniformBuffersSet.contains(binding));
			return m_UniformBuffersSet.at(binding);
		}

	private:
		struct ShaderSourceInfo
		{
			uint32_t GlStage;
			std::string Source;
			std::filesystem::path CacheOutPath;
		};

		void CompileOpenGLBinaries(const ShaderSourceInfo& shaderSourceInfo);
		void CreateProgram();
		void ReflectAndCreateBuffers(uint32_t stage, const std::vector<uint32_t>& shaderData, const char* shaderName);

	private:
		uint32_t m_RendererId = 0;
		std::string m_Name;

		std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSpirv;
		std::map<uint32_t, Ref<UniformBuffer>> m_UniformBuffersSet;
	};
}
