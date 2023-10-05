#pragma once
#include "Spark/Core/Base.h"
#include "Spark/Core/KeyCodes.h"
#include "Spark/Core/MouseCodes.h"

namespace Spark {
	class SPARK_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
