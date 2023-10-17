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
		// ����lambda��������ÿ��entity������[&]�����Ӿ�
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
		// ��ʼ��TreeNodeFlags�����ж�ѡ��״̬����������Ϊת����������
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		if (opened)
		{
			ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_OpenOnArrow;
			// Ӳ������Ϊ�ڵ�
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
			// ��������������
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
			// ����Ψһ���ڵ�������ڴ��б�
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
			// �����IDתΪHashCode�����ýڵ�Ϊ�۵�����״̬����������ΪCamera
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"));
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.camera;
				ImGui::Checkbox("Primary", &cameraComponent.Primary);
				const char* projectionTypeString[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];
				// ʹ��ѡ�����������ȡ��͸�ӷ���
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					// �������Combo����
					for (int i = 0; i < 2; i++)
					{
						bool isselected = currentProjectionTypeString == projectionTypeString[i];
						if (ImGui::Selectable(projectionTypeString[i], &isselected))
						{
							currentProjectionTypeString = projectionTypeString[i];
							// �������Ͷ������
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isselected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &verticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

					float perspNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Plane", &perspNear))
						camera.SetPerspectiveNearClip(perspNear);

					float perspFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Plane", &perspFar))
						camera.SetPerspectiveFarClip(perspFar);					
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Orthographic Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Plane", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Plane", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectionRatio);
				}
				ImGui::TreePop();
			}
		}
	}
}
