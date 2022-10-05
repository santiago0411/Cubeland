#pragma once

#include <glm/glm.hpp>

namespace Cubeland
{
	class Texture2D
	{
	public:
		static Ref<Texture2D> Create(const std::string& path, bool flip = false);
		static Ref<Texture2D> Create(int32_t width, int32_t height);

		Texture2D(const std::string& path, bool flip);
		Texture2D(int32_t width, int32_t height);
		~Texture2D();

		bool IsLoaded() const { return m_IsLoaded; }
		int32_t GetWidth() const { return m_Width; }
		int32_t GetHeight() const { return m_Height; }

		void SetData(const void* data, uint32_t size) const;
		void Bind(uint32_t slot) const;

		bool operator==(const Texture2D& other) const { return m_RendererId == other.m_RendererId; }

	private:
		uint32_t m_RendererId = 0;
		bool m_IsLoaded = false;
		int32_t m_Width = 0, m_Height = 0;
		uint32_t m_InternalFormat = 0, m_DataFormat = 0;
	};

	class SubTexture2D
	{
	public:
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = glm::vec2(1.0f));

		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		~SubTexture2D() = default;

		const Ref<Texture2D>& GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4]{};
	};
}
