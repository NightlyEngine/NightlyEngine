#pragma once

#include "Core/EngineAPI.h"
#include "Core/Core.h"
#include "Core/UUID.h"
#include "KeyDefinitions.h"

struct GLFWwindow;

namespace Nightly
{
	typedef std::function<void()> InputCallback;

	// Defines an input event.
	struct KeyAction
	{
		KeyAction()
				: m_Key(), m_Action(), m_Trigger(), m_Id()
		{
		}

		KeyAction(int key, int action, InputCallback trigger)
				: m_Key(key), m_Action(action), m_Trigger(std::move(trigger)), m_Id(UUID().GetUUID())
		{
		}

		int m_Key;
		int m_Action;
		InputCallback m_Trigger;

		NL_NODISCARD uint64_t GetId() const
		{
			return m_Id;
		}

		// Returns whether the key is being held down.
		// Only works if action is set to NL_REPEAT.
		NL_NODISCARD bool IsHeldDown() const
		{
			return m_HeldDown;
		}

		bool operator==(const KeyAction& keyAction) const
		{
			return keyAction.m_Key == m_Key && keyAction.m_Action == m_Action;
		}

	private:
		uint64_t m_Id;
		bool m_HeldDown = false;

		friend class InputSystem;
	};

	// Handles input events.
	class NL_API InputSystem
	{
	public:
		InputSystem() = default;
		~InputSystem() = default;

		// Registers key callbacks for the specified window.
		static void Initialize(GLFWwindow* window);

		// Fires callbacks for keys that are being held down.
		static void Update();

		// Triggers function when an action is performed with a key.
		// Action must be either NL_PRESS, NL_RELEASE or NL_REPEAT.
		// Returns the generated KeyAction object.
		static KeyAction BindKey(int key, int action, const InputCallback& trigger);

		// Triggers function when an action is performed with a key.
		// Action must be either NL_PRESS, NL_RELEASE or NL_REPEAT.
		// Returns the generated KeyAction object.
		static KeyAction BindKey(const KeyAction& keyAction);

		// Removes the key binding from the registry.
		// Returns whether the operation was successful.
		static bool RemoveBinding(const KeyAction& keyAction);

		// Removes the key binding from the registry.
		// The id can be obtained using KeyAction.GetId().
		// Returns whether the operation was successful.
		static bool RemoveBinding(uint64_t id);

	private:
		// Returns true when an action is either NL_PRESS, NL_RELEASE or NL_REPEAT.
		static bool IsValidAction(int action)
		{
			return action == NL_PRESS || action == NL_RELEASE || action == NL_REPEAT;
		}

		// Triggers the callback of keyAction.
		static void FireCallback(const KeyAction& keyAction);

		// GLFW key callback
		static void OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);

		static inline std::vector<KeyAction> m_KeyPressBinds;
		static inline std::vector<KeyAction> m_KeyHoldBinds;
	};
}