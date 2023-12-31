#pragma once
#include "Spark/Core/Timestep.h"
#include "Spark/Renderer/OrthographicCamera.h"

#include "Spark/Event/ApplicationEvent.h"
#include "Spark/Event/MouseEvent.h"

namespace Spark {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;
		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRotio, bool rotation);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void OnResize(float width, float height);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CaculateView(); }
		void CaculateView();
		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		bool OnMouseScrolled(MouseScrollEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;				// 横纵比
		float m_ZoomLevel = 1.0f;			// 缩放级别
		OrthographicCameraBounds m_Bounds;	// 获取相机边界，第一帧立即响应
		OrthographicCamera m_Camera;		// 相机成员，需要在参数后初始化
		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;		// 设置Transform的参数
		float m_CameraTranslationSpeed = 60.0f, m_CameraRotationSpeed = 60.0f;
	};
}