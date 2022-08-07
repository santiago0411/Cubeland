#pragma once

#include "Core/KeyCodes.h"
#include "Events/Event.h"

namespace Cubeland
{
	class KeyboardEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyboardEvent(const KeyCode key)
			: m_KeyCode(key) {}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyboardEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, const bool repeated)
			: KeyboardEvent(keycode), m_Repeated(repeated) {}

		bool IsRepeated() const { return m_Repeated; }

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		uint16_t m_Repeated;
	};

	class KeyReleasedEvent : public KeyboardEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode)
			: KeyboardEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyboardEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode)
			: KeyboardEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}