#include "spkpch.h"
#include "Spark/Renderer/RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Spark {
	// 整个生命周期都存在，因此暂不需要释放内存
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}