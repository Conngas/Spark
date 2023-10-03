#include "spkpch.h"
#include "Spark/Renderer/Renderer.h"
#include "Spark/Renderer/FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Spark {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	SPK_CORE_ASSERT(false, "RendererAPI::None is Currently Not Supported!");
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFrameBuffer>(spec);
		}

		SPK_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}



}