#pragma once
#include "Spark.h"

class Sandbox2D : public Spark::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Spark::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Spark::Event& e) override;
private:
	Spark::OrthographicCameraController m_CameraController;
	
	// Test 
	Spark::Ref<Spark::Shader> m_SquareShader;
	Spark::Ref<Spark::VertexArray> m_SquareVA;
	Spark::Ref<Spark::Texture2D> m_SquareTexture;
	Spark::Ref<Spark::Texture2D> m_QuadRotationTexture;
	
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f , 1.0f};
};
