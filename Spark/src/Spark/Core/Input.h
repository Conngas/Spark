#pragma once
#include "Spark/Core/KeyCodes.h"
#include "Spark/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Spark {
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
