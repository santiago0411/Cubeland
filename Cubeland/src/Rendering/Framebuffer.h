#pragma once

namespace Cubeland
{
	enum class FramebufferTextureFormat : uint8_t
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {
		}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {
		}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		void Invalidate();

		void Bind() const;
		void Unbind() const;

		void Resize(uint32_t width, uint32_t height);
		int32_t ReadPixel(uint32_t attachmentIndex, int32_t x, int32_t y) const;

		void ClearAttachment(uint32_t attachmentIndex, int32_t value) const;

		uint32_t GetColorAttachmentRendererId(uint32_t index = 0) const
		{
			CL_ASSERT(index < m_ColorAttachments.size());
			return m_ColorAttachments[index];
		}

		const FramebufferSpecification& GetSpecification() const
		{
			return m_Specification;
		}

	private:
		uint32_t m_RendererId;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentsSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}
