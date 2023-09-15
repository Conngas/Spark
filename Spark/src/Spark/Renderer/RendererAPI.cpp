#include "spkpch.h"
#include "Spark/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Spark {
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:	SPK_CORE_ASSERT(false, "RendererAPI is Currently Not Supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateScope<OpenGLRendererAPI>();
		}

		SPK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}