#include "Runtime/Core/Application.h"
#include "Runtime/Core/Engine.h"
#include "Core/Log.h"
#include "Core/WindowManager.h"

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

			NL_CORE_INFO("Initializing Nightly Editor...", LogSource::EDITOR);

			// Create editor window
			WindowProps props;
			props.title = "Nightly Editor";
			props.width = 1280;
			props.height = 720;

			m_EditorWindow = WindowManager::Create(props);

			// Update the editor as long as the window is not closed
			while (!m_EditorWindow->ShouldClose())
			{
				Update();
			}

			Quit();
		}

	private:
		void Update() override
		{
			if (!m_EngineInstance) return;

			m_EngineInstance->Update();

			m_EditorWindow->SwapBuffers();
			WindowManager::PollEvents();
		}

		void Quit() override
		{
			NL_CORE_INFO("Quitting Nightly Editor...", LogSource::EDITOR);

			if (m_EngineInstance)
			{
				m_EngineInstance->Quit();
			}

			m_EditorWindow->Destroy();
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
	std::unique_ptr<EditorApp> editor = std::make_unique<EditorApp>();
	editor->Start();
}
