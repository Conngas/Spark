#include "spkpch.h"
#include "Spark/Renderer/Texture.h"
#include "Spark/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Spark {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SPK_CORE_ASSERT(false, "RendererAPI::None is currently not supported !");
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(width,height);
		}

		SPK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	SPK_CORE_ASSERT(false, "RendererAPI::None is currently not supported !");
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLTexture2D>(path);
		}

		SPK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}