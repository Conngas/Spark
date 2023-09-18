#include "spkpch.h"
#include "Spark/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Spark {

	/// <summary>
	/// �����������Ұ��1.0Ϊ��׼��Խ����Ļ�����ɶ���Խ��
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
	/// ����ͶӰ���󣬼���������ɻ����С
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

		// glm��ѧ�����ViewMatrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

}