#pragma once
#include "spkpch.h"
#include "Spark/Scenes/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Spark {

	SceneCamera::SceneCamera()
	{
		ReculateProjection();
	}

	void SceneCamera::SetViewPortSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		ReculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthoGraphicSize = size;
		m_OrthoGraphicNear = nearClip;
		m_OrthographicFar = farClip;
		ReculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		ReculateProjection();
	}

	void SceneCamera::ReculateProjection()
	{
		// È·¶¨×Ýºá±È
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float orthoLeft = -m_OrthoGraphicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthoGraphicSize * m_AspectRatio * 0.5f;
			float orthoButton = -m_OrthoGraphicSize * 0.5f;
			float orthoTop = m_OrthoGraphicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoButton, orthoTop, m_OrthoGraphicNear, m_OrthographicFar);
		}
	}
}
