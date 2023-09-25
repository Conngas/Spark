#pragma once
#include "Spark/Renderer/Texture.h"
#include "Spark/Renderer/OrthographicCamera.h"

namespace Spark {
	
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float tileCount = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float tileCount = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float tileCount = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tileCount = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));

		static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float rotation, float textureScale = 1.0f);
		static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, float textureScale = 1.0f);
		static void DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, float textureScale = 1.0f);
		static void DrawRotationQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float textureScale = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
		static void DrawRotationQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, float textureScale = 1.0f, const glm::vec4& textureColor = glm::vec4(1.0f));
	};
}
