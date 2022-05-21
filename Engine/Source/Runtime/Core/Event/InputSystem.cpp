#include "InputSystem.h"
#include "Core/Log.h"

#include <GLFW/glfw3.h>

namespace Nightly
{
	void InputSystem::Initialize(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, OnKeyPress);
	}

	KeyAction InputSystem::BindKey(int key, int action, const InputCallback& trigger)
	{
		return BindKey(KeyAction(key, action, trigger));
	}

	KeyAction InputSystem::BindKey(const KeyAction& keyAction)
	{
		if (!IsValidAction(keyAction.m_Action)) return { };

		// Place in the appropriate registry
		if (keyAction.m_Action == NL_REPEAT)
		{
			m_KeyHoldBinds.push_back(keyAction);
		}
		else
		{
			m_KeyPressBinds.push_back(keyAction);
		}

		return keyAction;
	}

	bool InputSystem::RemoveBinding(const KeyAction& keyAction)
	{
		return RemoveBinding(keyAction.GetId());
	}

	bool InputSystem::RemoveBinding(uint64_t id)
	{
		bool successful = false;

		// Remove from key hold binds
		{
			auto it = std::remove_if(m_KeyHoldBinds.begin(), m_KeyHoldBinds.end(),
			                         [id](const KeyAction& key)
			                         {
				                         return key.GetId() == id;
			                         });

			if (it != m_KeyHoldBinds.end()) successful = true;

			m_KeyHoldBinds.erase(it, m_KeyHoldBinds.end());
		}

		// Remove from key press binds
		{
			auto it = std::remove_if(m_KeyPressBinds.begin(), m_KeyPressBinds.end(),
			                         [id](const KeyAction& key)
			                         {
				                         return key.GetId() == id;
			                         });

			if (it != m_KeyPressBinds.end()) successful = true;

			m_KeyPressBinds.erase(it, m_KeyPressBinds.end());
		}

		return successful;
	}

	void InputSystem::Update()
	{
		for (const auto& k : m_KeyHoldBinds)
		{
			if (k.IsHeldDown())
			{
				FireCallback(k);
			}
		}
	}

	void InputSystem::FireCallback(const KeyAction& keyAction)
	{
		try
		{
			keyAction.m_Trigger();
		} catch (std::bad_function_call& bfc)
		{
			NL_CORE_ERROR("Failed to fire input callback function!", ENGINE);
		}
	}

	void InputSystem::OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// Register input change if NL_REPEAT
		for (auto& k : m_KeyHoldBinds)
		{
			if (k.m_Key == key)
			{
				if (action == NL_PRESS)
				{
					k.m_HeldDown = true;
				}
				else if (action == NL_RELEASE)
				{
					k.m_HeldDown = false;
				}
			}
		}

		// Fire callbacks on NL_PRESS or NL_RELEASE actions
		for (const auto& k : m_KeyPressBinds)
		{
			if (k.m_Key == key && k.m_Action == action)
			{
				FireCallback(k);
			}
		}
	}
}