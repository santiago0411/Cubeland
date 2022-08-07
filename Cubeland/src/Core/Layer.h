#pragma once

#include "Events/Event.h"

namespace Cubeland
{
	class Layer
	{
	public:
		Layer();
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}
	};
}
