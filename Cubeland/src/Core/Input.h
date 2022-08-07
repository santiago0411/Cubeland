#pragma once

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

#include <glm/vec2.hpp>

namespace Cubeland
{
	class Input
	{
	public:
		Input() = delete;
		~Input() = delete;

		static bool IsKeyPressed(const KeyCode key);
		static bool IsMouseButtonPressed(const MouseButton button);
		static glm::vec2 GetMousePosition();
	};
}
