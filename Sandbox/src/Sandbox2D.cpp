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
		// Spark::Renderer2D::DrawRotationQuad({ 0.5f,-0.5f }, { 0.5f,0.75f }, m_SquareColor, glm::radians(45.0f));
		Spark::Renderer2D::DrawQuad({ -5.0f,-5.0f, -0.1f}, { 10.0f,10.0f }, m_SquareTexture, 10.0f);
		// End Scene
		Spark::Renderer2D::EndScene();
	}
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

