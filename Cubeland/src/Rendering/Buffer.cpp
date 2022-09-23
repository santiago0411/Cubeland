#include "pch.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace Cubeland
{
	static GLenum BufferDataTypeToOpenGLType(const BufferDataType type)
	{
		switch (type)
		{
			case BufferDataType::Float:
			case BufferDataType::Float2:
			case BufferDataType::Float3:
			case BufferDataType::Float4:
			case BufferDataType::Mat3:
			case BufferDataType::Mat4:
				return GL_FLOAT;

			case BufferDataType::Int:
			case BufferDataType::Int2:
			case BufferDataType::Int3:
			case BufferDataType::Int4:
				return GL_INT;

			case BufferDataType::Bool:
				return GL_BOOL;
		}

		CL_ASSERT(false);
		return 0;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t count)
	{
		return CreateRef<VertexBuffer>(vertices, count);
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

	void VertexArray::SetVertexBuffer(Ref<VertexBuffer> vertexBuffer)
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
				case BufferDataType::Float:
				case BufferDataType::Float2:
				case BufferDataType::Float3:
				case BufferDataType::Float4:
				{
					glEnableVertexAttribArray(vertexIndex);
					glVertexAttribPointer(vertexIndex, element.Count(),
						BufferDataTypeToOpenGLType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset);
					break;
				}
				case BufferDataType::Int:
				case BufferDataType::Int2:
				case BufferDataType::Int3:
				case BufferDataType::Int4:
				case BufferDataType::Bool:
				{
					glEnableVertexAttribArray(vertexIndex);
					glVertexAttribIPointer(vertexIndex, element.Count(),
						BufferDataTypeToOpenGLType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset);
					break;
				}

				case BufferDataType::Mat3:
				case BufferDataType::Mat4:
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

	void VertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
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
