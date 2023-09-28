#include "spkpch.h"
#include "Sandbox2D.h"
#include "Spark/Core/Base.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ×Ö·û´®µØÍ¼
static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"------------------------"
"------------------------"
"-------OOOOOOOOOOOO-----"
"------OOOOOOOOOOOOOO----"
"-----OOOOOOOOOOOOOOOO---"
"-----OOOOOOOOOOOOOOO----"
"--OOO--OOOOOOOOOOOOOO---"
"-OOOOO--OOOOOOOOOOOOOO--"
"-OOOOOO-----OOOOOOOOO---"
"--OOOO-----OOOOOOOOO----"
"----OOOO-OOOOOOOOOOO----"
"-----OOOOOOOOOOOOOO-----"
"--------OOOOOOOOOO------"
"------------------------";

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{}

void Sandbox2D::OnAttach()
{
	SPK_PROFILE_FUNCTION();

	m_SquareTexture = Spark::Texture2D::Create("Assets/Texture/Checkerboard.png");
	m_QuadRotationTexture = Spark::Texture2D::Create("Assets/Texture/ChernoLogo.png");
	m_SpriteSheet = Spark::Texture2D::Create("Assets/Game/Texture/RPGpack_sheet_2X.png");

	m_SubTex = Spark::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2,1 }, { 128,128 }, { 1, 2 });
	m_MapWidth = s_MapWidth; m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
	s_TextureMap['O'] = Spark::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6,11 }, { 128,128 });
	s_TextureMap['-'] = Spark::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,11 }, { 128,128 });

	m_Particale.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particale.ColorEnd = { 254 / 255.0f, 109 / 255.0f,41 / 255.0f,1.0f };
	m_Particale.SizeBegine = 0.5f, m_Particale.SizeVariaion = 0.3f, m_Particale.SizeEnd = 0.0f;
	m_Particale.LifeTime = 1.0f;
	m_Particale.Velocity = { 0.0f, 0.0f };
	m_Particale.VelocityVariation = { 3.0f, 1.0f };
	m_Particale.Postion = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(2.0f);
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

#if 0 // Test Debug Scene
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
#endif
	// ²âÊÔParticaleSystem
	if (Spark::Input::IsMouseButtonPress(SPK_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Spark::Input::GetMousePosition();
		auto width = Spark::Application::Get().GetWindow().GetWidth();
		auto height = Spark::Application::Get().GetWindow().GetHeight();
		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particale.Postion = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; ++i)
			m_ParticaleSystem.Emit(m_Particale);
	}
	m_ParticaleSystem.OnUpdate(ts);
	m_ParticaleSystem.OnRender(m_CameraController.GetCamera());

	// ²âÊÔTextureSheet
	// GAME RPG TEST
	Spark::Renderer2D::BeginScene(m_CameraController.GetCamera());
	// Spark::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f,1.0f }, m_SpriteSheet, 1.0f);
	// Spark::Renderer2D::DrawRotationQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f,1.0f }, m_SpriteSheet, 45.0f);
	// Spark::Renderer2D::DrawQuad({ 0.0f, 0.0f, 1.0f }, { 1.0f, 2.0f }, m_SubTex);

	for (uint32_t y = 0; y < m_MapHeight; ++y)
	{
		for (uint32_t x = 0; x < m_MapWidth; ++x)
		{
			char tileType = s_MapTiles[x + y * m_MapWidth];
			Spark::Ref<Spark::SubTexture2D> subtexture;
			if (s_TextureMap.find(tileType) != s_TextureMap.end())
				subtexture = s_TextureMap[tileType];
			else
				subtexture = m_SubTex;

			Spark::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f,y - m_MapHeight / 2.0f,0.5f }, { 1.0f,1.0f }, subtexture);
		}
	}

	Spark::Renderer2D::EndScene();
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

