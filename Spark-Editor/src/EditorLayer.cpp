#include "spkpch.h"
#include "EditorLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Spark {

	// 字符串地图
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

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
	{}

	void EditorLayer::OnAttach()
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

		//////////////////////////////////////////////////////////////////////////
		/// 创建帧缓冲
		//////////////////////////////////////////////////////////////////////////
		Spark::FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = Spark::FrameBuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();
		auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		m_SquareEntity = square;
		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SceondEntity = m_ActiveScene->CreateEntity("Clip-Space Entity");
		auto& cc = m_SceondEntity.AddComponent<CameraComponent>();
		cc.Primary = false;

		// Test Cam Script Component
		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{}
			void OnDestory()
			{}
			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 3.0f;

				if (Input::IsKeyPressed(Key::A))
					transform[3][0] -= speed * ts;
				if (Input::IsKeyPressed(Key::D))
					transform[3][0] += speed * ts;
				if (Input::IsKeyPressed(Key::W))
					transform[3][1] += speed * ts;
				if (Input::IsKeyPressed(Key::S))
					transform[3][1] -= speed * ts;
			}
		};
		// 将Nativate绑定到CameraController
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void EditorLayer::OnDetach()
	{
		SPK_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		// Instrument
		SPK_PROFILE_FUNCTION();

		// Resize 当屏幕比例不相等时立即Resize
		if (Spark::FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			// 设置Ratio
			m_ActiveScene->OnViewPortResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update Cam
		if(m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		Spark::Renderer2D::ResetStats();
		// Render
		{
			SPK_PROFILE_SCOPE("RenderCommand::Renderer Prep");
			// FB
			m_FrameBuffer->Bind();

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
		// 测试ParticaleSystem
		if (Spark::Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		{
			glm::vec2 mousePos = Spark::Input::GetMousePosition();
			auto width = Spark::Application::Get().GetWindow().GetWidth();
			auto height = Spark::Application::Get().GetWindow().GetHeight();
			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			mousePos.x = (mousePos.x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			mousePos.y = bounds.GetHeight() * 0.5f - (mousePos.y / height) * bounds.GetHeight();
			m_Particale.Postion = { mousePos.x + pos.x, mousePos.y + pos.y };
			for (int i = 0; i < 5; ++i)
				m_ParticaleSystem.Emit(m_Particale);
		}
		m_ParticaleSystem.OnUpdate(ts);
		m_ParticaleSystem.OnRender(m_CameraController.GetCamera());

		// 测试TextureSheet
		// GAME RPG TEST
		Spark::Renderer2D::BeginScene(m_CameraController.GetCamera());

		// Update Scene
		m_ActiveScene->OnUpdate(ts);

		// Update Map Tile
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

				Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f,y - m_MapHeight / 2.0f, -1.0f }, { 1.0f,1.0f }, subtexture);
			}
		}
		Spark::Renderer2D::EndScene();

		//FB
		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnImGuiRender()
	{
	#define DockSpaceOpen 1
		//////////////////////////////////////////////////////////////////////////
		/// DockSpace Show
		//////////////////////////////////////////////////////////////////////////
	#ifdef DockSpaceOpen
		static bool dockSpaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dock space
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling full screen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit"))	Spark::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	#endif // DockSpaceOpen
	
		//////////////////////////////////////////////////////////////////////////
		/// Editor Bar
		//////////////////////////////////////////////////////////////////////////
		ImGui::Begin("Settings");
		// Statistics
		auto stats = Spark::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Call: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		// Entity Test
		if (m_SquareEntity)
		{
			ImGui::Separator();
			auto& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());

			auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
			ImGui::Separator();
		}

		ImGui::DragFloat("Camera Transform",glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
		if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
		{
			m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
			m_SceondEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		}

		{
			auto& camera = m_SceondEntity.GetComponent<CameraComponent>().camera;
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Second Camera Orthographic Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);
		}

		ImGui::End();

		//////////////////////////////////////////////////////////////////////////
		// 窗口铺满与聚焦功能
		//////////////////////////////////////////////////////////////////////////
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("ViewPort");
		// 更新窗口聚焦状态
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = m_FrameBuffer->GetColorAttchment();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y },ImVec2{0,1},ImVec2{1,0});
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Spark::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}

