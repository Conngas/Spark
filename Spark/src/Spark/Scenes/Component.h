#pragma once
#include "Spark/Scenes/SceneCamera.h"
#include "Spark/Scenes/ScriptableEntity.h"

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
		SceneCamera camera;
		bool Primary = true; // Scene Cam
		bool FixedAspectionRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// 可继承的原生Script
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestoryScirpt)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{	
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestoryScirpt = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}
