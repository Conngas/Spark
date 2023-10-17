#include "SceneHierarchyPanel.h"
#include "Spark/Scenes/Component.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Spark {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		//////////////////////////////////////////////////////////////////////////
		/// Hierarchy
		//////////////////////////////////////////////////////////////////////////
		ImGui::Begin("Scene Hierarchy");
		// 接收lambda函数并对每个entity遍历，[&]捕获子句
		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};
		ImGui::End();

		//////////////////////////////////////////////////////////////////////////
		/// Property
		//////////////////////////////////////////////////////////////////////////
		ImGui::Begin("Properties");
		if (m_SelectionContext)
			DrawComponent(m_SelectionContext);
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		// 初始化TreeNodeFlags用于判定选中状态变量，（）为转换提升作用
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		if (opened)
		{
			ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_OpenOnArrow;
			// 硬编码作为节点
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponent(Entity entity)
	{
		//////////////////////////////////////////////////////////////////////////

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			// 用于命名缓冲区
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		//////////////////////////////////////////////////////////////////////////

		if (entity.HasComponent<TransformComponent>())
		{
			// 设置唯一树节点序号用于打开列表
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				// 
				ImGui::TreePop();
			}
		}

		//////////////////////////////////////////////////////////////////////////

		if (entity.HasComponent<CameraComponent>())
		{

		}
	}
}
