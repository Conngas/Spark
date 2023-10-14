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
		std::function<void()> InstantiateFunction;
		std::function<void()> DestoryInstanceFunction;
		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestoryFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		template<typename T>
		void Bind()
		{	
			InstantiateFunction = [&]() { Instance = new T(); };
			DestoryInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };
			// 核心循环
			OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnDestoryFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestory(); };
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(ts); };
		}
	};
}
