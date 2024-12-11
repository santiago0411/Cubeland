#pragma once

#include "Rendering/Buffer.h"
#include "Utils/Filesystem.h"

#include <map>
#include <ranges>

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
		void Unbind() const;

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

		void ClearCachedSpriv();

	private:
		uint32_t m_RendererId = 0;
		std::string m_Name;

		std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSpirv;
		std::map<std::string, Ref<UniformBuffer>> m_UniformBuffersSet;

		// Only for debug, add #if def in the future
		ShaderFilesMap m_ShaderSources;

		friend class ShaderLibrary;
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
			CL_ASSERT(s_Library.contains(name));
			return ReloadShader(s_Library[name]);
		}

		static bool ReloadShader(Ref<Shader> shader)
		{
			// TODO change shader creation to validate if they were successfully compiled/loaded?
			CL_LOG_INFO("Reloading shader {}...", shader->GetName());
			shader->Unbind();
			shader->ClearCachedSpriv();
			s_Library[shader->GetName()] = CreateRef<Shader>(shader->GetName(), shader->m_ShaderSources);
			CL_LOG_INFO("Shader reloaded successfully.");
			return true;
		}

		static auto GetAllShaders()
		{
			return s_Library | std::views::values;
		}

	private:
		inline static std::map<std::string, Ref<Shader>> s_Library;
	};
}
