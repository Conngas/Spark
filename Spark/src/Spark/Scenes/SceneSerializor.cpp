#include "spkpch.h"
#include "Spark/Scenes/Entity.h"
#include "Spark/Scenes/Component.h"
#include "Spark/Scenes/SceneSerializor.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		// YAML�ڲ�ת��ʱ�����encode��������Ҫ�ı�˺�������
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Spark {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializor::SceneSerializor(const Ref<Scene>& scene)
		: m_Scene(scene)
	{}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		// ����YAML������������ӳ���
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "12837192831273";	// EntityID to Match

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			// �����Զ���Ĳ��������غ���
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;

			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}
	
	void SceneSerializor::Serialize(const std::wstring& filePath)
	{
		// Create YAML Obj ����YAML����
		YAML::Emitter out;
		// Begin New Map �����µ�YAMLӳ��
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		// Create YAML Sequence ����YAML����
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		// ����Entity��ʹ��eachִ�и���Ļص�������each��������entityID���ڱ���
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;
				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;
		// ������д���ļ���
		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	void SceneSerializor::SerializeRuntime(const std::wstring& filePath)
	{
		// To Be Continue
		SPK_CORE_ASSERT(false);
	}

	bool SceneSerializor::DeSerialize(const std::wstring& filePath)
	{
		// �򿪸���λ�õ��ַ�����rdbuf���ڻ�ȡ���岿��
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();
		// �ַ������ص�Node��
		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		SPK_CORE_TRACE("DeSerializing Scene {0}", sceneName);

		auto entites = data["Entities"];
		if (entites)
		{
			for (auto entity : entites)
			{
				// �ڵ㣨entity��ת��Ϊ�ض���������
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				SPK_CORE_TRACE("DeSerialized Entity With ID = {0}, name = {1}", uuid, name);
				// ʹ�ø��������ݴ�������
				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					auto& cameraProp = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProp["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProp["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProp["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProp["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProp["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProp["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProp["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			}
			return true;
		}
	}

	// ��ʵ��
	bool SceneSerializor::DeSerializeRuntime(const std::wstring& filePath)
	{
		SPK_CORE_ASSERT(false);
		return false;
	}
}
