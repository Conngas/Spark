#pragma once
#include "Spark/Core/Timestep.h"

#include <entt.hpp>

namespace Spark {

	class Scene
	{
	public:
		Scene();
		~Scene();

		//TEMP
		entt::registry& Reg() { return m_Registry; }

		entt::entity CreateEntity();
		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};

}