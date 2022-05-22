#pragma once

#include "Core/EngineAPI.h"
#include "Core/Core.h"
#include "Event.h"
#include "InputDefinitions.h"

struct GLFWwindow;

namespace Nightly::Events
{
	struct NL_API InputEvent : public Event
	{
		explicit InputEvent(GLFWwindow* window, int key, int action)
				: m_Window(window), m_Key(key), m_Action(action)
		{
		}

		~InputEvent() override = default;

		NL_NODISCARD virtual GLFWwindow* GetNativeWindow()
		{
			return m_Window;
		}

		NL_NODISCARD int GetKey() const
		{
			return m_Key;
		}

		NL_NODISCARD int GetAction() const
		{
			return m_Action;
		}

	private:
		GLFWwindow* m_Window;
		int m_Key;
		int m_Action;
	};

	class NL_API OnKeyPress : public InputEvent
	{
	public:
		NL_DEFINE_EVENT(OnKeyPress);

		OnKeyPress(GLFWwindow* window, int key)
				: InputEvent(window, key, NL_PRESS)
		{
		}

		~OnKeyPress() override = default;
	};

	class NL_API OnKeyRelease : public InputEvent
	{
	public:
		NL_DEFINE_EVENT(OnKeyRelease);

		OnKeyRelease(GLFWwindow* window, int key)
				: InputEvent(window, key, NL_RELEASE)
		{
		}

		~OnKeyRelease() override = default;
	};

	class NL_API OnKeyHold : public InputEvent
	{
	public:
		NL_DEFINE_EVENT(OnKeyHold);

		OnKeyHold(GLFWwindow* window, int key)
				: InputEvent(window, key, NL_HOLD)
		{
		}

		~OnKeyHold() override = default;
	};

	class NL_API OnMouseButtonPress : public InputEvent
	{
	public:
		NL_DEFINE_EVENT(OnMouseButtonPress);

		OnMouseButtonPress(GLFWwindow* window, int button)
				: InputEvent(window, button, NL_PRESS)
		{
		}

		~OnMouseButtonPress() override = default;
	};

	class NL_API OnMouseButtonRelease : public InputEvent
	{
	public:
		NL_DEFINE_EVENT(OnMouseButtonRelease);

		OnMouseButtonRelease(GLFWwindow* window, int button)
				: InputEvent(window, button, NL_RELEASE)
		{
		}

		~OnMouseButtonRelease() override = default;
	};

	class NL_API OnMouseButtonHold : public InputEvent
	{
	public:
		NL_DEFINE_EVENT(OnMouseButtonHold);

		OnMouseButtonHold(GLFWwindow* window, int button)
				: InputEvent(window, button, NL_HOLD)
		{
		}

		~OnMouseButtonHold() override = default;
	};
}
