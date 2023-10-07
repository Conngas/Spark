#pragma once
#include "Spark/Renderer/Camera.h"
#include "Spark/Renderer/Texture.h"
#include "Spark/Renderer/SubTexture2D.h"
#include "Spark/Renderer/OrthographicCamera.h"

namespace Spark {
	
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera); // TODO Remove
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, float tileCount = 1.0f);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float tileCount = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float tileCount = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float tileCount = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));

		static void DrawRotationQuad(const glm::mat4& transform, const glm::vec4& color, float tileCount, float textureScale = 1.0f);
		static void DrawRotationQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
		static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float tileCount, float textureScale = 1.0f);
		static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float tileCount, float textureScale = 1.0f);
		static void DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float tileCount, float textureScale = 1.0f);
		static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
		static void DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));

		// RenderStats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();
	private:
		static void DrawPart(const glm::vec3& position, const glm::vec2& size, const glm::vec2* texCoords, const size_t qVCount,
							 float rotation, float tileCount,
							 const glm::vec4& color);
		static void DrawPart(const glm::mat4& transform, const glm::vec2* texCoords, const size_t qVCount,
							 float tileCount,
							 const glm::vec4& color);
		static void DrawPart(const glm::vec3& position, const glm::vec2& size, const glm::vec2* texCoords, const size_t qVCount,
							 const Ref<Texture2D>& texture,
							 float rotation = 0.0f , float tileCount = 1.0f, 
							 const glm::vec4& color = glm::vec4(1.0f));
		static void DrawPart(const glm::mat4& transform, const glm::vec2* texCoords, const size_t qVCount,
							 const Ref<Texture2D>& texture,
							 float tileCount = 1.0f,
							 const glm::vec4& color = glm::vec4(1.0f));
		static void FlushAndReset();

	};
}
