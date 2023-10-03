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
		virtual const uint32_t GetColorAttchment() const override { return m_ColorAttachment; }
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FrameBufferSpecification m_Specification;
	};
}
