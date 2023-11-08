#pragma once

#include "Spark.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Spark/Scenes/EditorCamera.h"
#include "Spark/SubSystem/ParticaleSystem.h"

namespace Spark {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Spark::Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Spark::Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		Spark::OrthographicCameraController m_CameraController;
	
		// Test 
		Ref<Shader> m_SquareShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Texture2D> m_SquareTexture;
		Ref<Texture2D> m_QuadRotationTexture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<SubTexture2D> m_SubTex;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SceondEntity;

		EditorCamera m_EditorCamera;

		bool m_PrimaryCamera = false;	
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f , 1.0f};

		ParticaleSystem m_ParticaleSystem;
		ParticaleProps m_Particale;

		int m_ImGuizmoType = -1;

		// Panel
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
