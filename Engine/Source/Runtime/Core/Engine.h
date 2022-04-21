#pragma once

#include "Application.h"

namespace Nightly
{
	// The entire engine runtime application.
	class NL_API Engine : public Application
	{
	public:
		Engine() = default;
		~Engine() override = default;

		void Start() override
		{
			std::cout << "Hello from Nightly Engine! \n";
		}

		void Update() override
		{
		}

		void Quit() override
		{
		}
	};
}