#include "Runtime/Core/Application.h"
#include "Runtime/Core/Engine.h"
#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/WindowManager.h"
#include "Renderer/Renderer.h"
#include "Core/PluginManager.h"
#include "World/World.h"
#include "World/Entity.h"

using namespace Nightly;

namespace NightlyEditor
{
	class EditorApp : public Application
	{
	public:
		EditorApp() = default;

		~EditorApp() override = default;

		void Start() override
		{
			NL_ASSERT(!m_IsRunning, "Nightly Editor is already running. Multiple instances of the application are not allowed!", EDITOR);
			m_IsRunning = true;

			// Create engine app
			m_EngineInstance = std::make_unique<Engine>();
			m_EngineInstance->Start();

			NL_CORE_INFO("Initializing Nightly Editor...", EDITOR);

			WindowManager::Initialize();

			// Create editor window
			WindowProps props;
			props.title = "Nightly Editor";
			props.width = 1280;
			props.height = 720;

			m_EditorWindow = WindowManager::Create(props);

			Renderer::Initialize();

			PluginManager::LoadActivePlugins();

			auto entity = std::make_shared<Entity>("Awesome Entity");
			auto world = std::make_unique<World>();
			world->AddEntity(entity);

			// Update the editor as long as the window is not closed
			while (!m_EditorWindow->ShouldClose())
			{
				Update();
			}

			world->RemoveEntity(entity);

			Quit();
		}

	private:
		void Update() override
		{
			if (!m_EngineInstance) return;

			m_EngineInstance->Update();

			PluginManager::UpdatePlugins();

			m_EditorWindow->SwapBuffers();
			WindowManager::PollEvents();
		}

		void Quit() override
		{
			NL_CORE_INFO("Quitting Nightly Editor...", EDITOR);

			if (m_EngineInstance)
			{
				m_EngineInstance->Quit();
			}

			m_EditorWindow->Destroy();
			PluginManager::UnloadPlugins();
			WindowManager::Terminate();
		}

	private:
		std::unique_ptr<Engine> m_EngineInstance;
		std::unique_ptr<Window> m_EditorWindow;
	};
}

using namespace NightlyEditor;

int main(int argc, char** argv)
{
	std::cout << "\n\n" << "Nightly Engine Copyright (C) 2022 Futureblur\n"
	          << "Welcome to Nightly Engine! Enjoy your time and happy coding :)\n\n";

	auto editor = std::make_unique<EditorApp>();
	editor->Start();
}
