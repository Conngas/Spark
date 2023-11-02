#pragma once

#include "Spark/Scenes/Scene.h"

namespace Spark {

	class SceneSerializor
	{
	public:
		SceneSerializor(const Ref<Scene>& scene);

		void Serialize(const std::string& filePath);
		void SerializeRuntime(const std::string& filePath);

		bool DeSerialize(const std::string& filePath);
		bool DeSerializeRuntime(const std::string& filePath);
	private:
		Ref<Scene> m_Scene;
	};

}
