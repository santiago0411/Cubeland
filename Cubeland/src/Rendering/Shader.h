#pragma once

#include "Rendering/Buffer.h"
#include "Utils/Filesystem.h"

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

		const std::string& GetName() const { return m_Name; }

		const Ref<UniformBuffer>& GetUniformBuffer(const std::string& binding) const
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
		std::map<std::string, Ref<UniformBuffer>> m_UniformBuffersSet;
	};

	class ShaderLibrary
	{
	public:
		static void AddShader(Ref<Shader> shader)
		{
			CL_ASSERT(shader);
			CL_ASSERT(!s_Library.contains(shader->GetName()), "Shader already exists in library!");
			s_Library[shader->GetName()] = shader;
		}

		static void CreateAndAddShader(const std::string& name, const ShaderFilesMap& shaderSources)
		{
			CL_ASSERT(!s_Library.contains(name), "Shader already exists in library!");
			s_Library[name] = CreateRef<Shader>(name, shaderSources);
		}

		static Ref<Shader> GetShader(const std::string& name)
		{
			CL_ASSERT(s_Library.contains(name));
			return s_Library[name];
		}

		static bool ReloadShader(const std::string& name)
		{
			CL_ASSERT(false, "TODO");
			return false;
		}

	private:
		inline static std::map<std::string, Ref<Shader>> s_Library;
	};
}
