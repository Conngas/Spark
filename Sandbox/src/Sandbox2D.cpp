#include "spkpch.h"
#include "Sandbox2D.h"
#include "Spark/Core/Core.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{}

void Sandbox2D::OnAttach()
{
	SPK_PROFILE_FUNCTION();

	m_SquareTexture = Spark::Texture2D::Create("Assets/Texture/Checkerboard.png");
	m_QuadRotationTexture = Spark::Texture2D::Create("Assets/Texture/ChernoLogo.png");
}

void Sandbox2D::OnDetach()
{
	SPK_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Spark::Timestep ts)
{
	// Instrument
	SPK_PROFILE_FUNCTION();

	// Update
	{
		m_CameraController.OnUpdate(ts);
	}

	Spark::Renderer2D::ResetStats();
	// Render
	{
		SPK_PROFILE_SCOPE("RenderCommand::Renderer Prep");
		Spark::RenderCommand::Clear();
		Spark::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	// Draw
	{
		SPK_PROFILE_SCOPE("Renderer2D::Renderer Draw");
		Spark::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Spark::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 0.8f,0.8f }, m_SquareColor);
		Spark::Renderer2D::DrawQuad({ 1.0f,0.0f }, { 0.2f,0.2f }, m_SquareColor);
		Spark::Renderer2D::DrawRotationQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, m_QuadRotationTexture, 45.0f);
		Spark::Renderer2D::DrawRotationQuad({ -1.0f,-2.0f }, { 0.5f,0.5f }, m_SquareColor, 45.0f);
		Spark::Renderer2D::DrawQuad({ 0.0f,0.0f, 0.0f}, { 10.0f,10.0f }, m_SquareTexture, 10.0f);
		// End Scene
		/*Spark::Renderer2D::EndScene();

		Spark::Renderer2D::BeginScene(m_CameraController.GetCamera());*/
		for (float y = -5.0f; y < 5.0f; y+=0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x+=0.5f)
			{
				glm::vec4 color = { (x + 0.5f) / 10.0f, 0.3f, (x + 5.0f) / 10.0f , 1.0f};
				Spark::Renderer2D::DrawQuad({ x, y, 0.5f }, { 0.45f,0.45f }, color);
			}
		}
		Spark::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	// Statistics
	auto stats = Spark::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Call: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Spark::Event& e)
{
	m_CameraController.OnEvent(e);
}

