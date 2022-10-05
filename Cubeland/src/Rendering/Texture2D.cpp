#include "pch.h"
#include "Texture2D.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Cubeland
{
	Ref<Texture2D> Texture2D::Create(const std::string& path, bool flip)
	{
		return CreateRef<Texture2D>(path, flip);
	}

	Ref<Texture2D> Texture2D::Create(int32_t width, int32_t height)
	{
		return CreateRef<Texture2D>(width, height);
	}

	Texture2D::Texture2D(const std::string& path, bool flip)
	{
		int32_t width, height, channels;
		stbi_set_flip_vertically_on_load(flip);
		CL_LOG_DEBUG("Loading Texture2D '{}'", path);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (!data)
		{
			CL_LOG_ERROR("Failed to load Texture2D '{}'", path);
			return;
		}

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		CL_ASSERT(internalFormat & dataFormat, "Format not suppoerted!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		m_IsLoaded = true;
	}

	Texture2D::Texture2D(int32_t width, int32_t height)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);
		glTextureStorage2D(m_RendererId, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererId);
	}

	void Texture2D::SetData(const void* data, uint32_t size) const
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		CL_ASSERT(size == m_Width * m_Height * bpp, "Data must be the entire texture!");
		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererId);
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { coords.x * cellSize.x / (float)texture->GetWidth(), coords.y * cellSize.y / (float)texture->GetHeight() };
		glm::vec2 max = { (coords.x + spriteSize.x) * cellSize.x / (float)texture->GetWidth(), (coords.y + spriteSize.y) * cellSize.y / (float)texture->GetHeight() };
		return CreateRef<SubTexture2D>(texture, min, max);
	}

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}
}
