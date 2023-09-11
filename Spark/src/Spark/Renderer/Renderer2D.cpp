#include "spkpch.h"
#include "Spark/Renderer/Renderer2D.h"
// Renderer
#include "Spark/Renderer/VertexArray.h"
#include "Spark/Renderer/Shader.h"
#include "Spark/Renderer/RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Spark {
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		// VertexArray
		s_Data->QuadVertexArray = VertexArray::Create();
		float squareVertice[3 * 4] = {
			-0.5f,	-0.5f,	0.0f,
			0.5f,	-0.5f,	0.0f,
			0.5f,	0.5f,	0.0f,
			-0.5f,	0.5f,	0.0f
		};

		// VertexBuffer
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertice, sizeof(squareVertice));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "m_Position"}
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		// VertexIndexBuffer 创建绘制顺序
		uint32_t squareIndice[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndice, sizeof(squareIndice) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		// 创建Shader
		s_Data->QuadShader = Shader::Create("assets/Shader/FlatColor.glsl");
	}

	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->QuadShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->QuadShader)->UploadUniformMat4("u_ViewProjection", 
																	 camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->QuadShader)->UploadUniformMat4("u_Transform",
																	 glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, { color.r,color.g,color.b,1.0f });
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->QuadShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->QuadShader)->UploadUniformFloat4("u_Color", color);
		
		// 绘制流程
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}
