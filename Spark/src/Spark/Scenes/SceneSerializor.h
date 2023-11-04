#pragma once
#include "Spark/Scenes/Scene.h"

#include <optional>

namespace Spark {

	class SceneSerializor
	{
	public:
		SceneSerializor(const Ref<Scene>& scene);

		void Serialize(const std::optional<std::wstring>& filePath);
		void SerializeRuntime(const std::optional<std::wstring>& filePath);

		bool DeSerialize(const std::optional<std::wstring>& filePath);
		bool DeSerializeRuntime(const std::optional<std::wstring>& filePath);
	private:
		Ref<Scene> m_Scene;
	};

}
