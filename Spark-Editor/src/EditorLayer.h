#pragma once

#include "Spark.h"
#include "Spark/SubSystem/ParticaleSystem.h"

namespace Spark {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Spark::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Spark::Event& e) override;
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
	
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f , 1.0f};

		ParticaleSystem m_ParticaleSystem;
		ParticaleProps m_Particale;

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Spark::Ref<Spark::SubTexture2D>> s_TextureMap;
	};
}