#include "spkpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
//#include "Platform/Direct"

namespace Spark {

	/////////////////////////////////////////////////////////////////////
	// VertexBuffer
	/////////////////////////////////////////////////////////////////////

	Ref<VertexBuffer> VertexBuffer::Create(float* vertice, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPK_CORE_ASSERT(false, "RenderAPI::None is currently NOT supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(vertice, size);
			break;
		}

		SPK_CORE_ASSERT(false, "Unkonwn RendererAPI!");
		return nullptr;
	}

	/////////////////////////////////////////////////////////////////////
	// IndexBuffer
	/////////////////////////////////////////////////////////////////////

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indice, uint32_t size)
	{
		switch(Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPK_CORE_ASSERT(false, "RendererAPI::None is currently NOT supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(indice, size);
		}

		SPK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}