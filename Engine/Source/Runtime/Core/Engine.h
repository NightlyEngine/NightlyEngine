#pragma once

#include "Application.h"

namespace NL
{
	// The entire engine runtime application.
	class NL_API Engine : public Application
	{
	public:
		Engine() = default;
		~Engine() override = default;

		void Start() override;

		void Update() override
		{
		}

		void Quit() override;
	};
}