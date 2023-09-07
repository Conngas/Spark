#pragma once
#include "Spark/Renderer/OrthographicCamera.h"
#include "Spark/Core/Timestep.h"
#include "Spark/Event/ApplicationEvent.h"
#include "Spark/Event/MouseEvent.h"

namespace Spark {
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRotio, bool rotation);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrollEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;				// ���ݱ�
		float m_ZoomLevel = 1.0f;			// ���ż���
		OrthographicCamera m_Camera;		// �����Ա����Ҫ�ڲ������ʼ��
		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;		// ����Transform�Ĳ���
		float m_CameraTranslationSpeed = 60.0f, m_CameraRotationSpeed = 60.0f;
	};
}