#include "spkpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Spark {
	// �����������ڶ����ڣ�����ݲ���Ҫ�ͷ��ڴ�
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}