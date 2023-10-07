#pragma once
#include "Spark/Renderer/Camera.h"

#include <glm/glm.hpp>

namespace Spark {

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };
		 
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f ,1.0f , 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4 color)
			: Color(color) {}
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tg)
			: Tag(tg) {}
	};

	struct CameraComponent
	{
		Spark::Camera camera;
		bool Primary = true; // Scene Cam

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection) : camera(projection) {}
	};
}
