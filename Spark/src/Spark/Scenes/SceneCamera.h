#pragma once
#include "Spark/Renderer/Camera.h"

namespace Spark {

	// 场景相机
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetViewPortSize(uint32_t width, uint32_t height);
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetOrthographicSize(float size) { m_OrthoGraphicSize = size;  ReculateProjection(); }

		float GetOrthographicSize() const { return m_OrthoGraphicSize; }
	private:
		/// <summary>
		/// 重新计算投影得到正确的AspectRatio
		/// </summary>
		void ReculateProjection();	
	private:
		// 设置缩放大小
		float m_OrthoGraphicSize = 10.0f;
		float m_OrthoGraphicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};
}

