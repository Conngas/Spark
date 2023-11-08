#include "spkpch.h"
#include "EditorLayer.h"
#include "Spark/Math/Math.h"
#include "Spark/Utils/PlatformUtils.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Spark/Scenes/SceneSerializor.h"


#include <imgui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Spark {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
	{}

	void EditorLayer::OnAttach()
	{
		SPK_PROFILE_FUNCTION();

		m_SquareTexture = Spark::Texture2D::Create("Assets/Texture/Checkerboard.png");

		Spark::FrameBufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = Spark::FrameBuffer::Create(fbSpec);
		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0		
		auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.2f, 0.1f, 1.0f });
		m_SquareEntity = square;
		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SceondEntity = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SceondEntity.AddComponent<CameraComponent>();
		cc.Primary = false;

		// Test Cam Script Component
		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 15 - 5.0f;
			}
			virtual void OnDestory() override
			{}
			virtual void OnUpdate(Timestep ts) override
			{
				auto& transform = GetComponent<TransformComponent>().Translation;
				float speed = 3.0f;

				if (Input::IsKeyPressed(Key::A))
					transform.x -= speed * ts;
				if (Input::IsKeyPressed(Key::D))
					transform.x += speed * ts;
				if (Input::IsKeyPressed(Key::W))
					transform.y += speed * ts;
				if (Input::IsKeyPressed(Key::S))
					transform.y -= speed * ts;
			}
		};
		// 将Nativate绑定到CameraController
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SceondEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

#endif
		// Hierarchy
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
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

			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			// 设置Ratio
			m_ActiveScene->OnViewPortResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update Cam
		if(m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		m_EditorCamera.OnUpdate(ts);

		Spark::Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		Spark::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Spark::RenderCommand::Clear();

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
		//////////////////////////////////////////////////////////////////////////
		/// 测试ParticaleSystem
		////////////////////////////////////////////////////////////////////////////
		//if (Spark::Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		//{
		//	glm::vec2 mousePos = Spark::Input::GetMousePosition();
		//	auto width = Spark::Application::Get().GetWindow().GetWidth();
		//	auto height = Spark::Application::Get().GetWindow().GetHeight();
		//	auto bounds = m_CameraController.GetBounds();
		//	auto pos = m_CameraController.GetCamera().GetPosition();
		//	mousePos.x = (mousePos.x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		//	mousePos.y = bounds.GetHeight() * 0.5f - (mousePos.y / height) * bounds.GetHeight();
		//	m_Particale.Postion = { mousePos.x + pos.x, mousePos.y + pos.y };
		//	for (int i = 0; i < 5; ++i)
		//		m_ParticaleSystem.Emit(m_Particale);
		//}
		//m_ParticaleSystem.OnUpdate(ts);
		//m_ParticaleSystem.OnRender(m_CameraController.GetCamera());

		// Update Scene
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

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
		if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Docking Space
		ImGuiIO& io = ImGui::GetIO();
		// Limit Docking Width
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}
		style.WindowMinSize.x = minWinSize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling full screen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				// ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
				if (ImGui::MenuItem("New", "Ctrl + N"))
					NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl + O"))
					OpenScene();
				if (ImGui::MenuItem("Save As ...", "Ctrl + Shift + S"))
					SaveSceneAs();
				if (ImGui::MenuItem("Exit"))	Spark::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	#endif // DockSpaceOpen
		//////////////////////////////////////////////////////////////////////////
		/// Hierarchy
		//////////////////////////////////////////////////////////////////////////
		m_SceneHierarchyPanel.OnImGuiRender();

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
		ImGui::End();

		//////////////////////////////////////////////////////////////////////////
		/// 窗口铺满与聚焦功能
		//////////////////////////////////////////////////////////////////////////
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("ViewPort");
		// 更新窗口聚焦状态
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = m_FrameBuffer->GetColorAttchment();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y },ImVec2{0,1},ImVec2{1,0});
		
		//////////////////////////////////////////////////////////////////////////
		/// ImGuizmo		
		//////////////////////////////////////////////////////////////////////////
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_ImGuizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// const glm::mat4& cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
			// 用于让imGuizmo锁定UI位置
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.GetTransform();

			// Snap 间隔
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.1f;
			if (m_ImGuizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 15.0f;
			float snapValues[3] = { snapValue, snapValue, snapValue };
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_ImGuizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				// 矩阵分解使用Guizmo控制
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deleteRotation = rotation - transformComponent.Rotation;
				transformComponent.Translation = translation;
				transformComponent.Rotation += deleteRotation;
				transformComponent.Scale = scale;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Spark::Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(SPK_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			//////////////////////////////////////////////////////////////////////////
			/// Data Serialization
			//////////////////////////////////////////////////////////////////////////
			case Key::N:
			{
				if (control)
					NewScene();
				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();
				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			/// ImGuizmo
			//////////////////////////////////////////////////////////////////////////
			case Key::Q:
				m_ImGuizmoType = -1;
				break;
			case Key::W:
				m_ImGuizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				m_ImGuizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				m_ImGuizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
	}
	
	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewPortResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		// Spark Scene (*.spark) 为文件筛选器的第一个字符串 后者为限制规则，添加L使其变为WCHAR* 规格以适应Unicode字符
		std::optional<std::wstring> filePath = FileDialogs::OpenFile(L"Spark Scene (*.spark)\0*.spark\0");
		if (filePath)
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewPortResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializor sceneSerializor(m_ActiveScene);
			sceneSerializor.DeSerialize(filePath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::optional<std::wstring> filePath = FileDialogs::SaveFile(L"Spark Scene (*.spark)\0*.spark\0");
		if (filePath)
		{
			SceneSerializor sceneSerializor(m_ActiveScene);
			sceneSerializor.Serialize(filePath);
		}
	}
}

