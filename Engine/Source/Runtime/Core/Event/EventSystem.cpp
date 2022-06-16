#include "EventSystem.h"

namespace NL
{
	EventSystem::EventPool EventSystem::m_EventPool;

	void EventSystem::Subscribe(const std::string& type, const EventSystem::EventCallback& trigger)
	{
		// Check if event is already in the pool
		if (m_EventPool.find(type) != m_EventPool.end())
		{
			m_EventPool[type].push_back(trigger);
			return;
		}

		std::vector<EventCallback> list = { trigger };
		m_EventPool.insert(std::pair<std::string, std::vector<EventCallback>>(type, list));
	}

	void EventSystem::Dispatch(EventFun fun)
	{
		for (const auto& event : m_EventPool)
		{
			// Check if we found the right event
			if (event.first == fun.GetName())
			{
				// Trigger all callbacks
				for (const auto& callback : event.second)
				{
					callback(fun);
				}
			}
		}
	}
}