#include "spkpch.h"
#include "Spark/Scenes/Scene.h"
#include "Spark/Scenes/Entity.h"
#include "Spark/Scenes/Component.h"
#include "Spark/Renderer/Renderer2D.h"

namespace Spark {

	static void DoMath(const glm::mat4& transform)
	{

	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity& entity)
	{

	}

	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		// entiry=uint32_t entity操作通过Registry实现
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
		
		if (m_Registry.has<TransformComponent>(entity))
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}

		auto view = m_Registry.view(TransformComponent)();
		for (auto entity:view)
		{
			TransformComponent& transform = view.get<TransformComponent>(eneity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>(entty));
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif 
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity: view)
			{
				auto& [transformCom , cameraCom] = view.get<TransformComponent, CameraComponent>(entity);
				// 若为主相机
				if (cameraCom.Primary)
				{
					mainCamera = &cameraCom.camera;
					cameraTransform = &transformCom.Transform;
					break;
				}
			}
		}
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transformCom, spriteColor] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transformCom.Transform, spriteColor.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewPortResize(uint32_t width, uint32_t height)
	{
		m_ViewPortWidth = width;
		m_ViewPorthHeigh = height;

		// 修正ViewPort尺寸
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectionRatio)
				cameraComponent.camera.SetViewPortSize(width, height);
		}
	}
}