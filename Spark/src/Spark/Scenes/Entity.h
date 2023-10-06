#pragma once
#include "Spark/Scenes/Scene.h"

#include <entt.hpp>

namespace Spark {

	class Entity 
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T, typename ... Args>
		T& AddComponent(Args&& ... args)
		{
			SPK_CORE_ASSERT(!HasComponent<T>(), "Entity Already Has Component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			SPK_CORE_ASSERT(HasComponent<T>(), "Entity Does NOT Has Component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			SPK_CORE_ASSERT(HasComponent<T>(), "Entity Does NOT Has Component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		// 返回ID从0开始，如果判定无效需要使用entt::null表示无效判定
		operator bool() const { return m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle{ 0 };
		Scene* m_Scene = nullptr;
	};

}
