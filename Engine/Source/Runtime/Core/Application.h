#pragma once

#include "Core.h"

namespace Nightly
{
	class NL_API Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		virtual void Launch() = 0;

	protected:
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnQuit() = 0;
	};
}