#include "pch.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace Cubeland
{
	static GLenum BufferDataTypeToOpenGLType(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
				return GL_FLOAT;

			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return GL_INT;

			case ShaderDataType::Bool:
				return GL_BOOL;
		}

		CL_ASSERT(false);
		return 0;
	}

	static const char* BufferDataTypeToString(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:
				return "float";
			case ShaderDataType::Float2:
				return "vec2";
			case ShaderDataType::Float3:
				return "vec3";
			case ShaderDataType::Float4:
				return "vec4";
			case ShaderDataType::Mat3:
				return "mat3";
			case ShaderDataType::Mat4:
				return "mat4";

			case ShaderDataType::Int:
				return "int";
			case ShaderDataType::Int2:
				return "ivec2";
			case ShaderDataType::Int3:
				return "ivec3";
			case ShaderDataType::Int4:
				return "ivec4";

			case ShaderDataType::Bool:
				return "bool";
		}

		CL_ASSERT(false);
		return "";
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return CreateRef<VertexBuffer>(size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t count)
	{
		return CreateRef<VertexBuffer>(vertices, count);
	}

	VertexBuffer::VertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer(float* vertices, uint32_t count)
	{
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::Reflect() const
	{
		CL_LOG_TRACE("VertexBuffer::Reflect - {} Attributes", m_Layout.Count());
		uint32_t i = 0;
		for (const auto& element : m_Layout)
		{
			CL_LOG_TRACE("    {} - Location: {}, Offset: {}, Type: {}, Normalized: {}", 
				element.Name, i, element.Offset, BufferDataTypeToString(element.Type), element.Normalized ? "GL_TRUE" : "GL_FALSE");
			i++;
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		return CreateRef<IndexBuffer>(indices, count);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<VertexArray>();
	}

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_RendererId);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererId);
	}

	void VertexArray::SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		CL_ASSERT(!m_VertexBuffer);

		Bind();
		vertexBuffer->Bind();

		uint32_t vertexIndex = 0;
		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : layout)
		{
			switch(element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(vertexIndex);
					glVertexAttribPointer(vertexIndex, element.Count(),
						BufferDataTypeToOpenGLType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset);
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(vertexIndex);
					glVertexAttribIPointer(vertexIndex, element.Count(),
						BufferDataTypeToOpenGLType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset);
					break;
				}

				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					const uint8_t count = element.Count();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(vertexIndex);
						glVertexAttribPointer(vertexIndex, count,
							BufferDataTypeToOpenGLType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(0, 1);
					}
					break;
				}

				default:
					CL_ASSERT(false);
					break;
			}
			vertexIndex++;
		}

		m_VertexBuffer = vertexBuffer;
	}

	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		return CreateRef<UniformBuffer>(size, binding);
	}

	UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
		: m_Size(size)
	{
		glCreateBuffers(1, &m_RendererId);
		glNamedBufferData(m_RendererId, m_Size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererId);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void UniformBuffer::SetData(const void* data) const
	{
		glNamedBufferSubData(m_RendererId, 0, m_Size, data);
	}

	void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) const
	{
		glNamedBufferSubData(m_RendererId, offset, size, data);
	}
}
