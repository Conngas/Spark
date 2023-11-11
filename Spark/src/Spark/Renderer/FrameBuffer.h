#pragma once

namespace Spark {
	
	enum class FrameBufferTextureFormat
	{
		None = 0,
		// Color
		RGBA8,
		// Depth
		DEPTH24STENCIL8,
		// Default
		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			: TextureFormat(format) {}

		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width = 0 , Height = 0; 
		FrameBufferAttachmentSpecification Attachments;
		uint32_t Sample = 1;

		// glBindTexture(0)
		bool SwapChainTarget = false;
	};
	
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const uint32_t GetColorAttchmentRendererID(uint32_t index = 0) const = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};
}
