#include "spkpch.h"
#include "Spark/Renderer/GraphicsContext.h"

#include "Spark/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Spark {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SPK_CORE_ASSERT(false, "RendererAPI is Currently Not Supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		SPK_CORE_ASSERT(false, "Unkonwn RendererAPI!");
		return nullptr;
	}
}