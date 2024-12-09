#include "pch.h"
#include "Framebuffer.h"

#include <glad/glad.h>

namespace Cubeland
{
	static constexpr uint32_t MAX_FRAMEBUFFER_SIZE = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(bool multiSampled)
		{
			return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multiSampled, int32_t count, uint32_t* outId)
		{
			glCreateTextures(TextureTarget(multiSampled), count, outId);
		}

		static void BindTexture(bool multiSampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multiSampled), id);
		}

		static void AttachColorTexture(uint32_t id, int32_t samples, int32_t internalFormat, 
			GLenum format, int32_t width, int32_t height, int32_t index)
		{
			const bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int32_t samples, int32_t format, GLenum attachmentType, 
			int32_t width, int32_t height)
		{
			const bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multiSampled), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			if (format == FramebufferTextureFormat::DEPTH24STENCIL8)
				return true;

			return false;
		}

		static GLenum CubelandFBTextureFormatToGL(FramebufferTextureFormat format)
		{
			if (format == FramebufferTextureFormat::RGBA8)
				return GL_RGBA8;
			if (format == FramebufferTextureFormat::RED_INTEGER)
				return GL_RED_INTEGER;

			CL_ASSERT(false);
			return 0;
		}
	}

	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto& attachmentSpec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(attachmentSpec.TextureFormat))
				m_ColorAttachmentsSpecifications.emplace_back(attachmentSpec);
			else
				m_DepthAttachmentSpecification = attachmentSpec;
		}

		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererId);
		glDeleteTextures((int32_t)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void Framebuffer::Invalidate()
	{
		if (m_RendererId)
		{
			glDeleteFramebuffers(1, &m_RendererId);
			glDeleteTextures((int32_t)m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		const bool multiSample = m_Specification.Samples > 1;

		if (!m_ColorAttachmentsSpecifications.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentsSpecifications.size());
			Utils::CreateTextures(multiSample, (int32_t)m_ColorAttachments.size(), m_ColorAttachments.data());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multiSample, m_ColorAttachments[i]);
				const auto textureFormat = m_ColorAttachmentsSpecifications[i].TextureFormat;
				if (textureFormat == FramebufferTextureFormat::RGBA8)
				{
					Utils::AttachColorTexture(m_ColorAttachments[i], (int32_t)m_Specification.Samples, GL_RGBA8, GL_RGBA,
						(int32_t)m_Specification.Width, (int32_t)m_Specification.Height, (int32_t)i);
				}
				else if (textureFormat == FramebufferTextureFormat::RED_INTEGER)
				{
					Utils::AttachColorTexture(m_ColorAttachments[i], (int32_t)m_Specification.Samples, GL_RG32I, GL_RED_INTEGER,
						(int32_t)m_Specification.Width, (int32_t)m_Specification.Height, (int32_t)i);
				}
				else
				{
					CL_ASSERT(false, "Invalid framebuffer color attachment texture format!!");
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multiSample, 1, &m_DepthAttachment);
			Utils::BindTexture(multiSample, m_DepthAttachment);

			if (m_DepthAttachmentSpecification.TextureFormat == FramebufferTextureFormat::DEPTH24STENCIL8)
			{
				Utils::AttachDepthTexture(m_DepthAttachment, (int32_t)m_Specification.Samples, GL_DEPTH24_STENCIL8,
					GL_DEPTH_STENCIL_ATTACHMENT, (int32_t)m_Specification.Width, (int32_t)m_Specification.Height);
			}
			else
			{
				CL_ASSERT(false, "Invalid framebuffer depth attachment texture format!!");
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			CL_ASSERT(m_ColorAttachments.size() <= 4);
			constexpr GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((int32_t)m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		CL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		glViewport(0, 0, (int32_t)m_Specification.Width, (int32_t)m_Specification.Height);
	}

	void Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > MAX_FRAMEBUFFER_SIZE || height > MAX_FRAMEBUFFER_SIZE)
		{
			CL_LOG_WARN("Attempted to resize a framebuffer to an invalid size: ({0}, {1})", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	int32_t Framebuffer::ReadPixel(uint32_t attachmentIndex, int32_t x, int32_t y) const
	{
		CL_ASSERT(attachmentIndex < m_ColorAttachments.size());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int32_t pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void Framebuffer::ClearAttachment(uint32_t attachmentIndex, int32_t value) const
	{
		CL_ASSERT(attachmentIndex < m_ColorAttachments.size());
		auto& spec = m_ColorAttachmentsSpecifications[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, 
			Utils::CubelandFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}
}
