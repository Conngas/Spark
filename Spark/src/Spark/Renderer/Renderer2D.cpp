#include "spkpch.h"
#include "Spark/Renderer/Shader.h"
#include "Spark/Renderer/Renderer2D.h"
#include "Spark/Scenes/EditorCamera.h"
#include "Spark/Renderer/VertexArray.h"
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
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndice = MaxQuads * 6;
		static const int32_t MaxTextureSlots = 32;				// TODO RenderCaps

		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture2D;
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;

		uint32_t QuadIndiceCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// TEXTURE
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;							// 0 White Texture
		// ROTATION
		glm::vec4 QuadVertexPosition[4];

		// Statistics
		Renderer2D::Statistics Stats;
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

		s_Data.QuadVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPosition[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::ShutDown()
	{
		SPK_PROFILE_FUNCTION();
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		SPK_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
		
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		SPK_PROFILE_FUNCTION();

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection",camera.GetViewProjectionMatrix());
		// 定位当前VertexBuffer位置
		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		SPK_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetViewProjection();
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		SPK_PROFILE_FUNCTION();
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		// Buffer 绑定
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndiceCount == 0)
			return;

		// 创建新的VB空间
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		// 绑定Texture
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
			s_Data.TextureSlots[i]->Bind(i);
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndiceCount);

		//Statistics Count 
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndiceCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawPart(const glm::vec3& position, const glm::vec2& size, const glm::vec2* texCoords, const size_t qVCount,
							  float rotation, float tileCount,
							  const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawPart(transform, texCoords, qVCount, tileCount, color);
	}
	void Renderer2D::DrawPart(const glm::mat4& transform, const glm::vec2* texCoords, const size_t qVCount,
							  float tileCount,
							  const glm::vec4& color)
	{
		// 边界判定
		if (s_Data.QuadIndiceCount >= Renderer2DStorage::MaxIndice)
			NextBatch();

		// 设定参数，TexIndex 0.0 是纯白Texture
		const float texIndex = 0.0f;

		// 批处理填入数据
		for (size_t i = 0; i < qVCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Positon = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TileCount = tileCount;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndiceCount += 6;

		//Statistics Count 
		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawPart(const glm::vec3& position, const glm::vec2& size, const glm::vec2* texCoords, const size_t qVCount,
							  const Ref<Texture2D>& texture,
							  float rotation , float tileCount ,
							  const glm::vec4& color )
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawPart(transform, texCoords, qVCount, texture, tileCount, color);
	}
	void Renderer2D::DrawPart(const glm::mat4& transform, const glm::vec2* texCoords, const size_t qVCount,
							  const Ref<Texture2D>& texture,
							  float tileCount,
							  const glm::vec4& color)
	{
		// 边界判定
		if (s_Data.QuadIndiceCount >= Renderer2DStorage::MaxTextureSlots)
			NextBatch();
		if (!texture)
			SPK_CORE_ASSERT(false, "No Texture to Draw!");

		// 确定当前texIndex
		// 设定参数
		float texIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
		{
			if (*s_Data.TextureSlots[i] == *texture)
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
		for (size_t i = 0; i < qVCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Positon = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVertexBufferPtr->TileCount = tileCount;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndiceCount += 6;

		//Statistics Count 
		s_Data.Stats.QuadCount++;
	}
		
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, float tileCount)
	{
		SPK_PROFILE_FUNCTION();
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 texCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		
		DrawPart(transform, texCoords, quadVertexCount ,tileCount, color);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tileCount, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		SPK_PROFILE_FUNCTION();
		// 材质剪裁
		constexpr float x = 2.0f, y = 3.0f;
		constexpr float sheetWidth = 2560, sheetHeight = 1664;
		constexpr float spriteWidth = 128, spriteHeight = 128;
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 texCoords[] =
		{
			{(x * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight},
			{((x + 1) * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight},
			{((x + 1) * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight},
			{(x * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight}
		};

		DrawPart(transform, texCoords, quadVertexCount, texture, tileCount, textureColor);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tileCount, const glm::vec4& textureColor)
	{
		SPK_PROFILE_FUNCTION();
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* texCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		// 调用绘制Texture函数
		DrawPart(transform, texCoords, quadVertexCount, texture, tileCount, textureColor);
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
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 texCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		DrawPart(position, size, texCoords, quadVertexCount, 0.0f, tileCount, color);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileCount, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tileCount, textureColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileCount, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		SPK_PROFILE_FUNCTION();
		// 材质剪裁
		constexpr float x = 2.0f, y = 3.0f;
		constexpr float sheetWidth = 2560, sheetHeight = 1664;
		constexpr float spriteWidth = 128, spriteHeight = 128;
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 texCoords[] =
		{
			{(x * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight},
			{((x + 1) * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight},
			{((x + 1) * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight},
			{(x * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight}
		};

		// 调用绘制函数，后三个参数默认
		DrawPart(position, size, texCoords, quadVertexCount,texture,0.0f, tileCount, textureColor);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tileCount, const glm::vec4& textureColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tileCount, textureColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tileCount, const glm::vec4& textureColor)
	{
		SPK_PROFILE_FUNCTION();
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* texCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		// 调用绘制Texture函数
		DrawPart(position, size, texCoords, quadVertexCount, texture, 0.0f, tileCount, textureColor);
	}

	/// <summary>
	/// 绘制旋转的Quad
	/// </summary>
	/// <param name="position">位置坐标</param>
	/// <param name="size">大小</param>
	/// <param name="color">颜色（RGB数值）</param>
	/// <param name="rotation">旋转（角度值°）</param>
	/// <param name="tileCount">密度</param>
	void Renderer2D::DrawRotationQuad(const glm::mat4& transform, const glm::vec4& color, float tileCount /*= 1.0f*/, float textureScale /*=1.0f*/)
	{
		SPK_PROFILE_FUNCTION();
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 texCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		DrawPart(transform, texCoords, quadVertexCount, tileCount, color);
	}
	void Renderer2D::DrawRotationQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tileCount /*= 1.0f*/, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		SPK_PROFILE_FUNCTION();
		// 材质剪裁
		constexpr float x = 2.0f, y = 3.0f;
		constexpr float sheetWidth = 2560, sheetHeight = 1664;
		constexpr float spriteWidth = 128, spriteHeight = 128;
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 texCoords[] =
		{
			{(x * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight},
			{((x + 1) * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight},
			{((x + 1) * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight},
			{(x * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight}
		};

		DrawPart(transform, texCoords, quadVertexCount, texture, tileCount, textureColor);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float rotation, float tileCount /*= 1.0f*/)
	{
		DrawRotationQuad({ position.x, position.y, 0.0f }, size, { color.r,color.g,color.b,1.0f }, rotation, tileCount);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, float tileCount /*= 1.0f*/)
	{
		DrawRotationQuad({ position.x, position.y, 0.0f }, size, { color.r,color.g,color.b,1.0f }, rotation, tileCount);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, float tileCount /*= 1.0f*/)
	{
		SPK_PROFILE_FUNCTION();
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 texCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		DrawPart(position, size, texCoords, quadVertexCount, rotation, tileCount, color);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float tileCount /*= 1.0f*/, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		DrawRotationQuad({ position.x, position.y, 1.0f }, size, texture, rotation, tileCount, textureColor);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float tileCount /*= 1.0f*/, const glm::vec4& textureColor /*= glm::vec4(1.0f)*/)
	{
		SPK_PROFILE_FUNCTION();
		// 材质剪裁
		constexpr float x = 2.0f, y = 3.0f;
		constexpr float sheetWidth = 2560, sheetHeight = 1664;
		constexpr float spriteWidth = 128, spriteHeight = 128;
		// 设定参数
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 texCoords[] =
		{
			{(x * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight},
			{((x + 1) * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight},
			{((x + 1) * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight},
			{(x * spriteWidth) / sheetWidth, ((y + 1) * spriteHeight) / sheetHeight}
		};

		DrawPart(position, size, texCoords, quadVertexCount, texture, rotation, tileCount, textureColor);
	}
	
	// Statistics
	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		// 记住VB位置
		StartBatch();
	}

}
