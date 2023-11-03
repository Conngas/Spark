#pragma once

#include "Spark/Scenes/Scene.h"

namespace Spark {

	class SceneSerializor
	{
	public:
		SceneSerializor(const Ref<Scene>& scene);

		void Serialize(const std::wstring& filePath);
		void SerializeRuntime(const std::wstring& filePath);

		bool DeSerialize(const std::wstring& filePath);
		bool DeSerializeRuntime(const std::wstring& filePath);
	private:
		Ref<Scene> m_Scene;
	};

}
