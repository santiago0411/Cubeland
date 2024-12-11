#include "pch.h"
#include "Shader.h"

#include "Utils/Filesystem.h"
#include "Utils/Timer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#define OPENGL_SHADER_CACHE_DIR	"assets/cache/shader/opengl"
#define OPENGL_CACHED_VERT_EXT	".opengl.vert.cache"
#define OPENGL_CACHED_FRAG_EXT	".opengl.frag.cache"

namespace Cubeland
{
	using namespace Utils;

	static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER:
				return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER:
				return shaderc_glsl_fragment_shader;
		}

		CL_ASSERT(false);
		return (shaderc_shader_kind)0;
	}

	static const char* GLShaderStageToString(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER:
				return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER:
				return "GL_FRAGMENT_SHADER";
		}

		CL_ASSERT(false);
		return nullptr;
	}

	static GLenum ShaderTypeToOpenGLStage(ShaderType type)
	{
		switch (type)
		{
			case ShaderType::Vertex: 
				return GL_VERTEX_SHADER;
			case ShaderType::Fragment:
				return GL_FRAGMENT_SHADER;
		}

		CL_ASSERT(false);
		return (GLenum)0;
	}

	static Filepath GetShaderCachePath(ShaderType type, const std::string& shaderName)
	{
		static Filepath cachePath(OPENGL_SHADER_CACHE_DIR);
		std::string extension;

		switch (type)
		{
			case ShaderType::Vertex:
				extension = OPENGL_CACHED_VERT_EXT;
				break;
			case ShaderType::Fragment:
				extension = OPENGL_CACHED_FRAG_EXT;
				break;
			default:
				CL_ASSERT(false);
				return "";
		}

		return cachePath / (shaderName + extension);
	}

	Shader::Shader(std::string name, const std::unordered_map<ShaderType, Filepath>& shaderSources)
		: m_Name(std::move(name))
	{
		Timer timer;

		for (auto&& [shaderType, shaderSourcePath] : shaderSources)
		{
			const Filepath shaderCachePath = GetShaderCachePath(shaderType, m_Name);
			const GLenum shaderStage = ShaderTypeToOpenGLStage(shaderType);

			if (std::filesystem::exists(shaderCachePath))
			{
				m_OpenGLSpirv[shaderStage] = Filesystem::ReadFileBytes32Bit(shaderCachePath);
			}
			else
			{
				Filesystem::CreateDirectoryIfNeeded(OPENGL_SHADER_CACHE_DIR);
				ShaderSourceInfo info{ shaderStage, Filesystem::ReadFileToString(shaderSourcePath), shaderCachePath };
				CompileOpenGLBinaries(info);
			}
		}

		CreateProgram();
		CL_LOG_DEBUG("Shader creation took {:.2f}ms.", timer.ElapsedMillis());

		for (auto&& [stage, source] : m_OpenGLSpirv)
			ReflectAndCreateBuffers(stage, source, m_Name.c_str());
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererId);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererId);
	}

	void Shader::CompileOpenGLBinaries(const ShaderSourceInfo& shaderSourceInfo)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		// options.SetOptimizationLevel(shaderc_optimization_level_performance);

		auto&& [stage, source, cachePath] = shaderSourceInfo;

		shaderc::CompilationResult result = compiler.CompileGlslToSpv(source, GLShaderStageToShaderC(stage), m_Name.c_str(), options);
		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			CL_LOG_ERROR(result.GetErrorMessage());
			CL_ASSERT(false);
			return;
		}

		const auto data = std::vector(result.cbegin(), result.cend());
		m_OpenGLSpirv[stage] = data;
		Filesystem::WriteBytes(cachePath, data.data(), data.size() * sizeof(uint32_t));
	}

	void Shader::CreateProgram()
	{
		const GLuint program = glCreateProgram();
		std::vector<GLuint> shaderIds;

		for (auto&& [stage, spirv] : m_OpenGLSpirv)
		{
			const GLuint shaderId = shaderIds.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), (GLsizei)(spirv.size() * sizeof(uint32_t)));
			glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderId);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			CL_LOG_ERROR("Shader linking failed ({0}): {1}", m_Name.c_str(), infoLog.data());

			glDeleteProgram(program);
			for (const auto id : shaderIds)
				glDeleteShader(id);
		}

		for (const auto id: shaderIds)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererId = program;
	}

	void Shader::ReflectAndCreateBuffers(uint32_t stage, const std::vector<uint32_t>& shaderData, const char* shaderName)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		CL_LOG_TRACE("Shader::Reflect - {0} {1}", GLShaderStageToString(stage), shaderName);
		CL_LOG_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		CL_LOG_TRACE("    {0} resources", resources.sampled_images.size());

		if (!resources.uniform_buffers.empty())
			CL_LOG_TRACE("Uniform buffers:");

		for (const auto& [id, typeId, baseTypeId, name] : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(baseTypeId);
			size_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(id, spv::DecorationBinding);
			size_t memberCount = bufferType.member_types.size();

			CL_LOG_TRACE("  {0}", name);
			CL_LOG_TRACE("    Size = {0}", bufferSize);
			CL_LOG_TRACE("    Binding = {0}", binding);
			CL_LOG_TRACE("    Members = {0}", memberCount);

			m_UniformBuffersSet[name] = UniformBuffer::Create((uint32_t)bufferSize, binding);
			CL_LOG_DEBUG("Created UniformBuffer {} at binding {}.", name, binding);
		}

		if (!resources.sampled_images.empty())
			CL_LOG_TRACE("Resources:");

		for (const auto& sampledImage : resources.sampled_images)
			CL_LOG_TRACE("  {0}", sampledImage.name);
	}
}
