#pragma once
#include "Spark/Core/Timestep.h"
#include "Spark/Scenes/EditorCamera.h"

#include <entt.hpp>

namespace Spark {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity GetPrimaryCameraEntity();
		void DestoryEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewPortResize(uint32_t width, uint32_t height);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewPortWidth, m_ViewPorthHeigh;
		friend class Entity;
		friend class SceneSerializor;
		friend class SceneHierarchyPanel;
	};

}
