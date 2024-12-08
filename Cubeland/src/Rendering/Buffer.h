#pragma once

#include "Core/Base.h"

#include <vector>

namespace Cubeland
{
	enum class ShaderDataType
	{
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
		Mat3,
		Mat4,
		Bool
	};

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint16_t Offset;
		bool Normalized;

		BufferElement(std::string&& name, const ShaderDataType type, const bool normalized = false)
			: Name(name), Type(type), Offset(0), Normalized(normalized) {}

		uint8_t Size() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 4;
				case ShaderDataType::Float2:	return 4 * 2;
				case ShaderDataType::Float3:	return 4 * 3;
				case ShaderDataType::Float4:	return 4 * 4;
				case ShaderDataType::Int:		return 4;
				case ShaderDataType::Int2:		return 4 * 2;
				case ShaderDataType::Int3:		return 4 * 3;
				case ShaderDataType::Int4:		return 4 * 4;
				case ShaderDataType::Mat3:		return 4 * 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4 * 4;
				case ShaderDataType::Bool:		return 1;
			}

			CL_ASSERT(false);
			return 0;
		}

		uint8_t Count() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Mat3:		return 3;
				case ShaderDataType::Mat4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			CL_ASSERT(false);
			return 0;
		}
	};

	struct BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& layout)
			: m_Layout(layout)
		{
			CalculateStrideAndOffset();
		}

		uint16_t GetStride() const { return m_Stride; }
		size_t Count() const { return m_Layout.size(); }

		std::vector<BufferElement>::iterator begin() { return m_Layout.begin(); }
		std::vector<BufferElement>::iterator end() { return  m_Layout.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Layout.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return  m_Layout.end(); }

	private:
		void CalculateStrideAndOffset()
		{
			uint16_t offset = 0;

			for (auto& element : m_Layout)
			{
				element.Offset = offset;
				m_Stride += element.Size();
				offset += element.Size();
			}
		}

	private:
		uint16_t m_Stride = 0;
		std::vector<BufferElement> m_Layout;
	};

	class VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t count);
		VertexBuffer(uint32_t size);
		VertexBuffer(float* vertices, uint32_t count);
		~VertexBuffer();

		void Bind() const;

		void SetData(const void* data, uint32_t size) const;

		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

		void Reflect() const;

	private:
		BufferLayout m_Layout;
		uint32_t m_RendererId = 0;
	};

	class IndexBuffer
	{
	public:
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_Count;
		uint32_t m_RendererId = 0;
	};

	class VertexArray
	{
	public:
		static Ref<VertexArray> Create();
		VertexArray();
		~VertexArray();

		void Bind() const;

		const Ref<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
		const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

		void SetVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererId = 0;
	};

	class UniformBuffer
	{
	public:
		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
		UniformBuffer(uint32_t size, uint32_t binding);
		~UniformBuffer();

		void SetData(const void* data) const;
		void SetData(const void* data, uint32_t size, uint32_t offset) const;

	private:
		uint32_t m_Size = 0;
		uint32_t m_RendererId = 0;
	};
}
