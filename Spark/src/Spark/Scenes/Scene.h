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
	private:
		entt::registry m_Registry;
		friend class Entity;
	};

}