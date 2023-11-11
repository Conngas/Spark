#pragma once
#include "spark/Renderer/FrameBuffer.h"

namespace Spark {
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();
		void Invalidate();
		
		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual const uint32_t GetColorAttchmentRendererID(uint32_t index = 0) const override 
		{
			SPK_CORE_ASSERT(index < m_ColorAttachments.size(), "ColorAttachment Size Error");
			return m_ColorAttachments[index];
		}
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecification;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification = FrameBufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}
