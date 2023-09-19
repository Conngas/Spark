#pragma once
#include "Spark.h"

class ExampleLayer : public Spark::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;
	
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	
	void OnUpdate(Spark::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Spark::Event& e) override;

private:
	Spark::ShaderLibrary shaderLibrary;

	Spark::Ref<Spark::Shader> m_Shader;
	Spark::Ref<Spark::VertexBuffer> m_VertexBuffer;
	Spark::Ref<Spark::VertexArray> m_VertexArray;
	Spark::Ref<Spark::IndexBuffer> m_IndexBuffer;

	Spark::Ref<Spark::Shader> m_SquareShader;
	Spark::Ref<Spark::VertexArray> m_SquareVartexArray;
	Spark::Ref<Spark::VertexBuffer> m_SquareVertexBuffer;
	Spark::Ref<Spark::IndexBuffer> m_SquareIndexBuffer;

	Spark::Ref<Spark::Texture2D> m_Texture, m_LogoTexture;

	Spark::OrthographicCameraController m_CameraController;
	glm::vec3 m_CustomColor = { 0.2f, 0.2f, 0.8f };
};
