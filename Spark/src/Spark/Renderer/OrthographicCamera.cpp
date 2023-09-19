#include "spkpch.h"
#include "Spark/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Spark {

	/// <summary>
	/// 设置摄像机视野，1.0为标准，越大屏幕内容纳东西越多
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="top"></param>
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f,1.0f)),m_ViewMatrix(1.0f)
	{
		SPK_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	/// <summary>
	/// 设置投影矩阵，即摄像机容纳画面大小
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="top"></param>
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		SPK_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	void OrthographicCamera::ReCalculateViewMatrix()
	{
		SPK_PROFILE_FUNCTION();

		// glm数学库计算ViewMatrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

}