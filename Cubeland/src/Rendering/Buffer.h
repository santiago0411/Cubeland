#pragma once

#include "Core/Base.h"

#include <vector>

namespace Cubeland
{
	enum class BufferDataType
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
		BufferDataType Type;
		uint16_t Offset;
		bool Normalized;

		BufferElement(std::string&& name, const BufferDataType type, const bool normalized = false)
			: Name(name), Type(type), Offset(0), Normalized(normalized) {}

		uint8_t Size() const
		{
			switch (Type)
			{
				case BufferDataType::Float:		return 4;
				case BufferDataType::Float2:	return 4 * 2;
				case BufferDataType::Float3:	return 4 * 3;
				case BufferDataType::Float4:	return 4 * 4;
				case BufferDataType::Int:		return 4;
				case BufferDataType::Int2:		return 4 * 2;
				case BufferDataType::Int3:		return 4 * 3;
				case BufferDataType::Int4:		return 4 * 4;
				case BufferDataType::Mat3:		return 4 * 3 * 3;
				case BufferDataType::Mat4:		return 4 * 4 * 4;
				case BufferDataType::Bool:		return 1;
			}

			CL_ASSERT(false);
			return 0;
		}

		uint8_t Count() const
		{
			switch (Type)
			{
				case BufferDataType::Float:		return 1;
				case BufferDataType::Float2:	return 2;
				case BufferDataType::Float3:	return 3;
				case BufferDataType::Float4:	return 4;
				case BufferDataType::Int:		return 1;
				case BufferDataType::Int2:		return 2;
				case BufferDataType::Int3:		return 3;
				case BufferDataType::Int4:		return 4;
				case BufferDataType::Mat3:		return 3;
				case BufferDataType::Mat4:		return 4;
				case BufferDataType::Bool:		return 1;
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
		static Ref<VertexBuffer> Create(float* vertices, uint32_t count);
		VertexBuffer(float* vertices, uint32_t count);
		~VertexBuffer();

		void Bind() const;

		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

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

		const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

		void SetVertexBuffer(Ref<VertexBuffer> vertexBuffer);
		void SetIndexBuffer(Ref<IndexBuffer> indexBuffer);

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
