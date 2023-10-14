#pragma once
#include "Spark/Renderer/Camera.h"

namespace Spark {

	// �������
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
		/// ���¼���ͶӰ�õ���ȷ��AspectRatio
		/// </summary>
		void ReculateProjection();	
	private:
		// �������Ŵ�С
		float m_OrthoGraphicSize = 10.0f;
		float m_OrthoGraphicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_AspectRatio = 0.0f;
	};
}

