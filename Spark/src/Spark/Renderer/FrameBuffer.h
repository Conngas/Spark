#pragma once

namespace Spark {
	
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;
		
		// glBindTexture(0)
		bool SwapChainTarget = false;
	};
	
	class FrameBuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const uint32_t GetColorAttchment() const = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};
}
