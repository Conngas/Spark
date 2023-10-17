#pragma once
#include "Spark/Core/Timestep.h"

#include <entt.hpp>

namespace Spark {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		
		void OnUpdate(Timestep ts);
		void OnViewPortResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewPortWidth, m_ViewPorthHeigh;
		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}
