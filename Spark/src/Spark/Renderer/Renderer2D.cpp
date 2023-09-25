#include "spkpch.h"
#include "Spark/Renderer/Renderer2D.h"
#include "Spark/Renderer/VertexArray.h"
#include "Spark/Renderer/Shader.h"
#include "Spark/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Spark {

	struct QuadVertex
	{
		glm::vec3 Positon;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TileCount;
	};

	struct Renderer2DStorage
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndice = MaxQuads * 6;
		static const int32_t MaxTextureSlots = 32;				// TODO RenderCaps

		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture2D;
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;

		uint32_t QuadIndiceCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;							// 0 White Texture
	};

	static Renderer2DStorage s_Data;

	void Renderer2D::Init()
	{
		SPK_PROFILE_FUNCTION();

		// VertexArray
		s_Data.QuadVertexArray = VertexArray::Create();

		// VertexBuffer
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"},
				{ ShaderDataType::Float4, "a_Color"},
				{ ShaderDataType::Float2, "a_TexCoord"},
				{ ShaderDataType::Float,  "a_TexIndex"},
				{ ShaderDataType::Float,  "a_TexTileCount"}
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		// VertexIndexBuffer 创建绘制顺序，并在堆上分配空间
		uint32_t* quadIndice = new uint32_t[s_Data.MaxIndice];
		
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndice; i += 6)
		{
			quadIndice[i + 0] = offset + 0;
			quadIndice[i + 1] = offset + 1;
			quadIndice[i + 2] = offset + 2;
			
			quadIndice[i + 3] = offset + 2;
			quadIndice[i + 4] = offset + 3;
			quadIndice[i + 5] = offset + 0;

			// +=4 因为Quad四个点
			offset += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndice, s_Data.MaxIndice);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndice;

		// 代码创建Texture
		s_Data.WhiteTexture2D = Texture2D::Create(1, 1);
		uint32_t whiteTexture2DData = 0xffffffff;
		s_Data.WhiteTexture2D->SetData(&whiteTexture2DData, sizeof(uint32_t));

		// 创建Shader
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; ++i)
			samplers[i] = i;
		s_Data.QuadShader = Shader::Create("Assets/Shader/QuadShader.glsl");
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set Zero
		s_Data.TextureSlots[0] = s_Data.WhiteTexture2D;
	}

	void Renderer2D::ShutDown()
	{
		SPK_PROFILE_FUNCTION();
	}


	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		SPK_PROFILE_FUNCTION();

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection",camera.GetViewProjectionMatrix());

		//记住VB位置
		s_Data.QuadIndiceCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		SPK_PROFILE_FUNCTION();
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		// Buffer 绑定
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush()
	{
		// 绑定Texture
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
			s_Data.TextureSlots[i]->Bind(i);
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndiceCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float tileCount)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, { color.r,color.g,color.b,1.0f }, tileCount);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float tileCount)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, { color.r,color.g,color.b,1.0f }, tileCount);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float tileCount)
	{
		SPK_PROFILE_FUNCTION();
		// 设定参数，TexIndex 0.0 是纯白Texture
		const float texIndex = 0.0f;
		s_Data.QuadVertexBufferPtr->Positon = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TileCount = tileCount;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Positon = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TileCount = tileCount;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Positon = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TileCount = tileCount;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Positon = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TileCount = tileCount;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndiceCount += 6;
#ifdef OLD_PATH
		s_Data.QuadShader->SetFloat("u_TextureScale", textureScale);
		s_Data.QuadShader->SetFloat4("u_Color", color);
		s_Data.WhiteTexture2D->Bind();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
							  glm::scale(glm::mat4(1.0f), {size.x,size.y,1.0f});
		s_Data.QuadShader->SetMat4("u_Transform", transform);

		// 绘制流程
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
		
		s_Data.WhiteTexture2D->UnBind();
#endif // OLD_PATH
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileCount, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tileCount, textureColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileCount, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		SPK_PROFILE_FUNCTION();
		// 设定参数
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float texIndex = 0.0f;
		// 确定当前texIndex
		for (uint32_t i = 1; i<s_Data.TextureSlotIndex;++i)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}
		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
		// 批处理填入数据
		s_Data.QuadVertexBufferPtr->Positon = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TileCount = tileCount;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Positon = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TileCount = tileCount;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Positon = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TileCount = tileCount;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Positon = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
		s_Data.QuadVertexBufferPtr->TileCount = tileCount;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndiceCount += 6;		
#ifdef OLD_PATH
		// 绑定
		s_Data.QuadShader->SetFloat("u_TextureScale", textureScale);
		s_Data.QuadShader->SetFloat4("u_Color", textureColor);
		texture->Bind();
		// transform 绘制
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data.QuadShader->SetMat4("u_Transform", transform);
		
		// 绘制流程
		s_Data.QuadShader->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
		texture->UnBind();
#endif // OLD_PATH
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

		s_Data.QuadShader->SetFloat("u_TextureScale", textureScale);
		s_Data.QuadShader->SetFloat4("u_Color", color);
		s_Data.WhiteTexture2D->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data.QuadShader->SetMat4("u_Transform", transform);

		// 绘制流程
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);

		s_Data.WhiteTexture2D->UnBind();
	}
	void Renderer2D::DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float textureScale /*= 1.0f*/, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		DrawRotationQuad({ position.x, position.y, 1.0f }, size, texture, rotation, textureScale, textureColor);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float textureScale /*= 1.0f*/, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		SPK_PROFILE_FUNCTION();

		// 绑定
		s_Data.QuadShader->SetFloat("u_TextureScale", textureScale);
		s_Data.QuadShader->SetFloat4("u_Color", textureColor);
		texture->Bind();
		// transform 绘制
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data.QuadShader->SetMat4("u_Transform", transform);

		// 绘制流程
		s_Data.QuadShader->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
		texture->UnBind();
	}
}
