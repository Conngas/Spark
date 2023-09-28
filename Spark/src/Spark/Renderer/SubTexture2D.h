#pragma once
#include "Spark/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Spark {

	class SubTexture2D 
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2 min, const glm::vec2 max);
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, 
												  const glm::vec2& coords, 
												  const glm::vec2& cellSize, 
												  const glm::vec2& spriteSize = { 1.0f, 1.0f });
		
		const Ref<Texture2D> GetTexture() { return m_Texture; }
		const glm::vec2* GetTexCoords() { return m_TexCoords; }
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}
