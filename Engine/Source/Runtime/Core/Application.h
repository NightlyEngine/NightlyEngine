#pragma once

#include "EngineAPI.h"

#include "ITrackable.h"

namespace NL
{
	// This is a base class for all application types.
	class NL_API Application : public ITrackable<Application>
	{
	public:
		Application() = default;
		~Application() override = default;

		NL_DEFINE_TRACKABLE(Application);

		// Launches the application and initializes necessary variables.
		// Throws an error if this is being called more than once.
		virtual void Start() = 0;

		// This is being called every frame as long as the application is running.
		virtual void Update() = 0;

		// Quits the application and deletes heap allocated variables.
		virtual void Quit() = 0;

	protected:
		bool m_IsRunning = false;
	};
}