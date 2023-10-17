#pragma once
#include "Spark/Renderer/Camera.h"

namespace Spark {

	// 场景相机
	class SceneCamera : public Camera
	{
	// 选择SceneCamera类别
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetViewPortSize(uint32_t width, uint32_t height);

		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);
		
		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveFOV = verticalFOV; ReculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; ReculateProjection(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; ReculateProjection(); }

		float GetOrthographicSize() const { return m_OrthoGraphicSize; }
		void SetOrthographicSize(float size) { m_OrthoGraphicSize = size;  ReculateProjection(); }
		float GetOrthographicNearClip() const { return m_OrthoGraphicNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthoGraphicNear = nearClip; ReculateProjection(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; ReculateProjection(); }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type;  ReculateProjection(); }
	private:
		/// <summary>
		/// 重新计算投影得到正确的AspectRatio
		/// </summary>
		void ReculateProjection();	
	private:
		// 设置缩放大小
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_OrthoGraphicSize = 10.0f;
		float m_OrthoGraphicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};
}

