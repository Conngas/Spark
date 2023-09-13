#include "spkpch.h"
#include "Spark/Renderer/Renderer2D.h"
#include <glm/gtc/matrix_transform.hpp>
// Renderer
#include "Spark/Renderer/VertexArray.h"
#include "Spark/Renderer/Shader.h"
#include "Spark/Renderer/RenderCommand.h"


namespace Spark {
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		// VertexArray
		s_Data->QuadVertexArray = VertexArray::Create();
		float squareVertice[5 * 4] = {
			-0.5f,	-0.5f,	0.0f, 0.0f, 0.0f,
			0.5f,	-0.5f,	0.0f, 1.0f, 0.0f,
			0.5f,	0.5f,	0.0f, 1.0f, 1.0f,
			-0.5f,	0.5f,	0.0f, 0.0f, 1.0f
		};

		// VertexBuffer
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertice, sizeof(squareVertice));
		squareVB->SetLayout({
				{ ShaderDataType::Float3, "m_Position"},
				{ ShaderDataType::Float2, "a_TexCoord"}			
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		// VertexIndexBuffer 创建绘制顺序
		uint32_t squareIndice[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndice, sizeof(squareIndice) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		// 创建Shader
		s_Data->QuadShader = Shader::Create("Assets/Shader/FlatColor.glsl");
		s_Data->TextureShader = Shader::Create("Assets/Shader/Texture.glsl");
	}

	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetMat4("u_ViewProjection",camera.GetViewProjectionMatrix());
		s_Data->QuadShader->SetMat4("u_Transform",glm::mat4(1.0f));

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_Transform", glm::mat4(1.0f));
		s_Data->TextureShader->SetInt("u_Texture", 0);
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
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
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetFloat4("u_Color", color);
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
							  glm::scale(glm::mat4(1.0f), {size.x,size.y,1.0f});
		s_Data->QuadShader->SetMat4("u_Transform", transform);

		// 绘制流程
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();
		texture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		
		// 绘制流程
		s_Data->TextureShader->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}
