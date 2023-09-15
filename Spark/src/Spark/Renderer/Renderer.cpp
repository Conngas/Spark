#include "spkpch.h"
#include "Spark/Renderer/Renderer.h"
#include "Spark/Renderer/Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Spark {
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::ShutDown()
	{
		Renderer2D::ShutDown();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(	const Ref<Shader>& shader, 
							const Ref<VertexArray>& vertexArray,
							const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_Transform", transform);
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}


	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}