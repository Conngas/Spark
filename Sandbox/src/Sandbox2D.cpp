#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{}

void Sandbox2D::OnAttach()
{
	m_SquareTexture = Spark::Texture2D::Create("Assets/Texture/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Spark::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Spark::RenderCommand::Clear();
	Spark::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

	Spark::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Spark::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f,0.8f },m_SquareColor);
	Spark::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { 0.5f,0.75f },m_SquareColor);
	Spark::Renderer2D::DrawQuad({ 0.0f,0.0f, -0.1f}, { 5.0f,5.0f }, m_SquareTexture);

	// End Scene
	Spark::Renderer::EndScene();

	// SceneRender
	// TODO ADD Submit instead dynamic
	// std::dynamic_pointer_cast<Spark::OpenGLShader>(m_SquareShader)->Bind();
	// std::dynamic_pointer_cast<Spark::OpenGLShader>(m_SquareShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Spark::Event& e)
{
	m_CameraController.OnEvent(e);
}

