#pragma once

#include "Core/EngineAPI.h"
#include "Core/Core.h"
#include "Core/UUID.h"
#include "InputDefinitions.h"

struct GLFWwindow;

namespace NL
{
	typedef std::function<void()> InputCallback;

	// Specifies the source of an input, like keyboard, mouse or controller.
	enum class InputType
	{
		KEYBOARD,
		MOUSE,
		CONTROLLER
	};

	// Defines an input event.
	struct InputAction
	{
		InputAction()
				: m_Key(), m_Action(), m_Type(), m_Trigger(), m_Id()
		{
		}

		InputAction(int key, int action, const InputType& type, InputCallback trigger)
				: m_Key(key), m_Action(action), m_Type(type), m_Trigger(std::move(trigger)), m_Id(UUID().GetUUID())
		{
		}

		// Returns the key / button that has been assigned to the input action.
		NL_NODISCARD int GetKey() const
		{
			return m_Key;
		}

		// Returns whether the key / button is pressed, released or being held down.
		NL_NODISCARD int GetAction() const
		{
			return m_Action;
		}

		NL_NODISCARD InputType GetInputType() const
		{
			return m_Type;
		}

		// Returns the callback function to be fired.
		NL_NODISCARD InputCallback GetCallback() const
		{
			return m_Trigger;
		}

		NL_NODISCARD uint64_t GetId() const
		{
			return m_Id;
		}

		// Returns whether the key / button is being held down.
		// Only works if action is set to NL_HOLD.
		NL_NODISCARD bool IsHeldDown() const
		{
			return m_HeldDown;
		}

		bool operator==(const InputAction& action) const
		{
			return action.m_Key == m_Key && action.m_Action == m_Action;
		}

	private:
		int m_Key;
		int m_Action;
		InputType m_Type;
		InputCallback m_Trigger;

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

		// Fires callbacks for keys / buttons that are being held down.
		static void Update();

		// Triggers a function when an action is performed with a key.
		// Action must be either NL_PRESS, NL_RELEASE or NL_HOLD.
		// Returns the generated InputAction object.
		static InputAction BindKey(int key, int action, const InputCallback& trigger);

		// Triggers a function when an action is performed with a mouse button.
		// Action must be either NL_PRESS, NL_RELEASE or NL_HOLD.
		// Returns the generated InputAction object.
		static InputAction BindMouse(int key, int action, const InputCallback& trigger);

		// Triggers a function when an action is performed with a controller button.
		// Action must be either NL_PRESS, NL_RELEASE or NL_HOLD.
		// Returns the generated InputAction object.
		static InputAction BindController(int key, int action, const InputCallback& trigger);

		// Triggers a function when an action is performed with a key / button.
		// Action must be either NL_PRESS, NL_RELEASE or NL_HOLD.
		// Returns the generated InputAction object.
		static InputAction BindInput(const InputAction& inputAction);

		// Removes the input binding from the registry.
		// Returns whether the operation was successful.
		static bool RemoveBinding(const InputAction& inputAction);

		// Removes the input binding from the registry.
		// The id can be obtained using InputAction.GetId().
		// Returns whether the operation was successful.
		static bool RemoveBinding(uint64_t id);

	private:
		// Returns true when an action is either NL_PRESS, NL_RELEASE or NL_HOLD.
		static bool IsValidAction(int action)
		{
			return action == NL_PRESS || action == NL_RELEASE || action == NL_HOLD;
		}

		// Triggers the callback of the input action.
		static void FireCallback(const InputAction& inputAction);

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		static inline std::vector<InputAction> m_InputBinds;
		static inline std::vector<InputAction> m_InputHoldBinds;

		friend class Window;
	};
}