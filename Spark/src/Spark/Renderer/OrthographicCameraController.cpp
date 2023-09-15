#include "spkpch.h"
#include "Spark/Renderer/OrthographicCameraController.h"

#include "Spark/Core/Input.h"
#include "Spark/Core/KeyCodes.h"

namespace Spark {

	OrthographicCameraController::OrthographicCameraController(float aspectRotio, bool rotation)
		:	m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
			m_Rotation(rotation),
			m_AspectRatio(aspectRotio)			
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		// ts 用于同步时间，游戏速度不受帧率影响
		// WASD
		/* OLD STYLE
		if (Spark::Input::IsKeyPress(SPK_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPress(SPK_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		if (Spark::Input::IsKeyPress(SPK_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (Spark::Input::IsKeyPress(SPK_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		if (m_Rotation)
		{
			if ((Spark::Input::IsKeyPress(SPK_KEY_Q)))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if ((Spark::Input::IsKeyPress(SPK_KEY_E)))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}
		*/
		if (Spark::Input::IsKeyPress(SPK_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Spark::Input::IsKeyPress(SPK_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (Spark::Input::IsKeyPress(SPK_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Spark::Input::IsKeyPress(SPK_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (m_Rotation)
		{
			if ((Spark::Input::IsKeyPress(SPK_KEY_Q)))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if ((Spark::Input::IsKeyPress(SPK_KEY_E)))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		// 移动速度随缩放变换
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrollEvent>(SPK_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SPK_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrollEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset();
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}