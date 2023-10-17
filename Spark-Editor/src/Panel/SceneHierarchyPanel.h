#pragma once

#include "Spark/Core/Log.h"
#include "Spark/Core/Base.h"
#include "Spark/Scenes/Scene.h"
#include "Spark/Scenes/Entity.h"

namespace Spark {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);
		
		void SetContext(const Ref<Scene>& context);
		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}
