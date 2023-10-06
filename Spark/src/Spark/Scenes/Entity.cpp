#include "spkpch.h"
#include "Spark/Scenes/Entity.h"

namespace Spark {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {}

}