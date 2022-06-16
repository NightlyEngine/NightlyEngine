#pragma once

#include "Runtime/Core/EngineAPI.h"
#include "Runtime/Core/Event/Event.h"

#define NL_CAST_EVENT(Type, fun) (Events::Type*) &fun

namespace NL
{
	class NL_API EventSystem
	{
	public:
		typedef std::function<
		void(EventFun
		fun)>
		EventCallback;
		typedef std::map<std::string, std::vector<EventCallback>> EventPool;

		EventSystem() = default;
		~EventSystem() = default;

		// Listens for callbacks and executes code when it gets fired.
		static void Subscribe(const std::string& type, const EventCallback& trigger);

		// Triggers callbacks on all subscribed objects.
		static void Dispatch(EventFun fun);

	private:
		static EventPool m_EventPool;
	};
}
