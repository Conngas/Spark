#include "spkpch.h"
#include "Spark/Renderer/Renderer2D.h"
#include "Spark/Renderer/VertexArray.h"
#include "Spark/Renderer/Shader.h"
#include "Spark/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Spark {
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture2D;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		SPK_PROFILE_FUNCTION();

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
		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertice, sizeof(squareVertice));
		squareVB->SetLayout({
				{ ShaderDataType::Float3, "m_Position"},
				{ ShaderDataType::Float2, "a_TexCoord"}
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		// VertexIndexBuffer 创建绘制顺序
		uint32_t squareIndice[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndice, sizeof(squareIndice) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		// 代码创建Texture
		s_Data->WhiteTexture2D = Texture2D::Create(1, 1);
		uint32_t whiteTexture2DData = 0xffffffff;
		s_Data->WhiteTexture2D->SetData(&whiteTexture2DData, sizeof(uint32_t));

		// 创建Shader
		s_Data->QuadShader = Shader::Create("Assets/Shader/QuadShader.glsl");
	}

	void Renderer2D::ShutDown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		SPK_PROFILE_FUNCTION();

		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetMat4("u_ViewProjection",camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		SPK_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float textureScale)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, { color.r,color.g,color.b,1.0f }, textureScale);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float textureScale)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, { color.r,color.g,color.b,1.0f }, textureScale);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float textureScale)
	{
		SPK_PROFILE_FUNCTION();

		s_Data->QuadShader->SetFloat("u_TextureScale", textureScale);
		s_Data->QuadShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture2D->Bind();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
							  glm::scale(glm::mat4(1.0f), {size.x,size.y,1.0f});
		s_Data->QuadShader->SetMat4("u_Transform", transform);

		// 绘制流程
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		
		s_Data->WhiteTexture2D->UnBind();
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureScale, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, textureScale, textureColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float textureScale, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		SPK_PROFILE_FUNCTION();

		// 绑定
		s_Data->QuadShader->SetFloat("u_TextureScale", textureScale);
		s_Data->QuadShader->SetFloat4("u_Color", textureColor);
		texture->Bind();
		// transform 绘制
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);
		
		// 绘制流程
		s_Data->QuadShader->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		texture->UnBind();
	}

	void Renderer2D::DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float rotation, float textureScale /*= 1.0f*/)
	{
		DrawRotationQuad({ position.x, position.y, 0.0f }, size, { color.r,color.g,color.b,1.0f }, rotation, textureScale);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, float textureScale /*= 1.0f*/)
	{
		DrawRotationQuad({ position.x, position.y, 0.0f }, size, { color.r,color.g,color.b,1.0f }, rotation, textureScale);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, float textureScale /*= 1.0f*/)
	{
		SPK_PROFILE_FUNCTION();

		s_Data->QuadShader->SetFloat("u_TextureScale", textureScale);
		s_Data->QuadShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture2D->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);

		// 绘制流程
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);

		s_Data->WhiteTexture2D->UnBind();
	}
	void Renderer2D::DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float textureScale /*= 1.0f*/, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		DrawRotationQuad({ position.x, position.y, 1.0f }, size, texture, rotation, textureScale, textureColor);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float textureScale /*= 1.0f*/, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		SPK_PROFILE_FUNCTION();

		// 绑定
		s_Data->QuadShader->SetFloat("u_TextureScale", textureScale);
		s_Data->QuadShader->SetFloat4("u_Color", textureColor);
		texture->Bind();
		// transform 绘制
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);

		// 绘制流程
		s_Data->QuadShader->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		texture->UnBind();
	}
}
