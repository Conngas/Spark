#include "spkpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Spark {
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SPK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexArray>();
		}
		SPK_CORE_ASSERT(false, "Unkonwn RendererAPI!");
		return nullptr;
	}
}