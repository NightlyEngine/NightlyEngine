#include "InputSystem.h"

#include <GLFW/glfw3.h>

#include "EventSystem.h"
#include "InputEvents.h"

#include "Core/Log.h"
#include "Core/Window.h"
#include "Core/WindowManager.h"

namespace NL
{
	void InputSystem::Update()
	{
		for (const auto& binding : m_InputHoldBinds)
		{
			if (binding.IsHeldDown())
			{
				FireCallback(binding);
			}
		}
	}

	InputAction InputSystem::BindKey(int key, int action, const InputCallback& trigger)
	{
		return BindInput(InputAction(key, action, InputType::KEYBOARD, trigger));
	}

	InputAction InputSystem::BindMouse(int key, int action, const InputCallback& trigger)
	{
		return BindInput(InputAction(key, action, InputType::MOUSE, trigger));
	}

	InputAction InputSystem::BindController(int key, int action, const InputCallback& trigger)
	{
		return BindInput(InputAction(key, action, InputType::CONTROLLER, trigger));
	}

	InputAction InputSystem::BindInput(const InputAction& inputAction)
	{
		if (!IsValidAction(inputAction.m_Action)) return { };

		// Place in the appropriate registry
		if (inputAction.m_Action == NL_HOLD)
		{
			m_InputHoldBinds.push_back(inputAction);
		}
		else
		{
			m_InputBinds.push_back(inputAction);
		}

		return inputAction;
	}

	bool InputSystem::RemoveBinding(const InputAction& inputAction)
	{
		return RemoveBinding(inputAction.GetId());
	}

	bool InputSystem::RemoveBinding(uint64_t id)
	{
		bool successful = false;

		// Remove from input hold binds
		{
			auto it = std::remove_if(m_InputHoldBinds.begin(), m_InputHoldBinds.end(),
			                         [id](const InputAction& key)
			                         {
				                         return key.GetId() == id;
			                         });

			if (it != m_InputHoldBinds.end()) successful = true;

			m_InputHoldBinds.erase(it, m_InputHoldBinds.end());
		}

		// Remove from input binds
		{
			auto it = std::remove_if(m_InputBinds.begin(), m_InputBinds.end(),
			                         [id](const InputAction& key)
			                         {
				                         return key.GetId() == id;
			                         });

			if (it != m_InputBinds.end()) successful = true;

			m_InputBinds.erase(it, m_InputBinds.end());
		}

		return successful;
	}

	bool InputSystem::IsKeyPressed(int key)
	{
		int state = glfwGetKey(WindowManager::GetCurrentWindow()->GetNativeWindow(), key);
		return state == NL_PRESS;
	}

	bool InputSystem::IsMouseButtonPressed(int button)
	{
		int state = glfwGetMouseButton(WindowManager::GetCurrentWindow()->GetNativeWindow(), button);
		return state == NL_PRESS;
	}

	Vec2 InputSystem::GetMousePosition()
	{
		double x, y;
		glfwGetCursorPos(WindowManager::GetCurrentWindow()->GetNativeWindow(), &x, &y);

		return { x, y };
	}

	void InputSystem::FireCallback(const InputAction& inputAction)
	{
		try
		{
			inputAction.m_Trigger();
		} catch (std::bad_function_call& bfc)
		{
			NL_CORE_ERROR("Failed to fire input callback function!", ENGINE);
		}
	}

	void InputSystem::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// Discard GLFW_REPEAT since we implemented our own solution for holding keys / buttons
		if (action == NL_HOLD) return;

		// Dispatch events
		if (action == NL_PRESS)
		{
			Events::OnKeyPress event(window, key);
			EventSystem::Dispatch(event);
		}
		else if (action == NL_RELEASE)
		{
			Events::OnKeyRelease event(window, key);
			EventSystem::Dispatch(event);
		}

		// Register input change if NL_HOLD
		for (auto& binding : m_InputHoldBinds)
		{
			if (binding.m_Key == key)
			{
				if (binding.IsHeldDown())
				{
					Events::OnKeyHold event(window, key);
					EventSystem::Dispatch(event);
				}

				if (action == NL_PRESS)
				{
					binding.m_HeldDown = true;
				}
				else if (action == NL_RELEASE)
				{
					binding.m_HeldDown = false;
				}
			}
		}

		// Fire callbacks on NL_PRESS or NL_RELEASE actions
		for (const auto& binding : m_InputBinds)
		{
			if (binding.m_Key == key && binding.m_Action == action)
			{
				FireCallback(binding);
			}
		}
	}

	void InputSystem::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		// Discard GLFW_REPEAT since we implemented our own solution for holding keys / buttons
		if (action == NL_HOLD) return;

		// Dispatch events
		if (action == NL_PRESS)
		{
			Events::OnMouseButtonPress event(window, button);
			EventSystem::Dispatch(event);
		}
		else if (action == NL_RELEASE)
		{
			Events::OnMouseButtonRelease event(window, button);
			EventSystem::Dispatch(event);
		}

		// Register input change if NL_HOLD
		for (auto& binding : m_InputHoldBinds)
		{
			if (binding.m_Key == button)
			{
				if (binding.IsHeldDown())
				{
					Events::OnMouseButtonHold event(window, button);
					EventSystem::Dispatch(event);
				}

				if (action == NL_PRESS)
				{
					binding.m_HeldDown = true;
				}
				else if (action == NL_RELEASE)
				{
					binding.m_HeldDown = false;
				}
			}
		}

		// Fire callbacks on NL_PRESS or NL_RELEASE actions
		for (const auto& binding : m_InputBinds)
		{
			if (binding.m_Key == button && binding.m_Action == action)
			{
				FireCallback(binding);
			}
		}
	}
}