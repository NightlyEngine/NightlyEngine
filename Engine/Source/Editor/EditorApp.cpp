#include "Core/Application.h"
#include "Core/Engine.h"
#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/Math.h"
#include "Core/PluginManager.h"
#include "Core/WindowManager.h"
#include "Core/Window.h"
#include "Core/Event/InputSystem.h"
#include "Core/Event/EventSystem.h"
#include "Core/Event/InputEvents.h"
#include "World/World.h"
#include "World/Entity.h"
#include "World/WorldManager.h"
#include "World/Components/MeshComponent.h"
#include "Renderer/Renderer.h"

#include "EditorUI.h"

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
			// FIXME: Do we even need this?
			m_EngineInstance = std::make_unique<Engine>();
			m_EngineInstance->Start();

			NL_CORE_INFO("Initializing Nightly Editor...", EDITOR);

			WindowManager::Initialize();

			// Create editor window
			WindowProps props;
			props.title = "Nightly Editor - Pre Alpha";
			props.width = 1280;
			props.height = 720;

			m_EditorWindow = WindowManager::Create(props);

			Renderer::m_DrawFramebufferToScreen = true;
			Renderer::Initialize();

			EditorUI::Initialize(m_EditorWindow->GetNativeWindow());

			PluginManager::LoadActivePlugins();

			auto world = WorldManager::CreateWorld("New World");
			WorldManager::LoadWorld(world);

			auto parent = world->CreateEntity("Parent Entity");
			parent->Transform()->Translate(Vec3(0, 0, -1));
			parent->AddComponent<MeshComponent>(std::make_shared<MeshComponent>());

			// Quit when ESC is pressed
			InputSystem::BindKey(NL_KEY_ESCAPE, NL_PRESS, [this]
			{
				m_EditorWindow->Destroy();
			});

			// Update the editor as long as the window is not closed
			while (!m_EditorWindow->ShouldClose())
			{
				Update();
			}

			world->RemoveEntity(parent);

			Quit();
		}

	private:
		void Update() override
		{
			if (!m_EngineInstance) return;

			m_EngineInstance->Update();

			InputSystem::Update();
			Renderer::Update();
			EditorUI::Draw();
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
			EditorUI::Terminate();
			Renderer::Cleanup();
			WindowManager::Terminate();
		}

	private:
		std::unique_ptr<Engine> m_EngineInstance;
		Ref<Window> m_EditorWindow;
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
