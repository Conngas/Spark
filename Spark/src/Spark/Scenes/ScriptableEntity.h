#pragma once
#include "Spark/Scenes/Entity.h"

namespace Spark {
	
	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:
		Entity m_Entity;
		friend class Scene;
	};

}
