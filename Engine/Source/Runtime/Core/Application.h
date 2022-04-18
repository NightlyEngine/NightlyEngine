#pragma once

#include "Core.h"

namespace Nightly
{
	class NL_API Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;
		
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void Quit() = 0;
	};
}