#include "Runtime/Core/Application.h"
#include "Runtime/Core/Engine.h"
#include "Core/Log.h"

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
			if (m_IsRunning)
			{
				NL_CORE_ERROR("Nightly Editor is already running. Multiple instances of the application are not allowed!",
				              LogSource::EDITOR);

				return;
			}

			m_IsRunning = true;

			m_EngineInstance = std::make_unique<Engine>();
			m_EngineInstance->Start();

			NL_CORE_INFO("Initializing Nightly Editor...", LogSource::EDITOR);

			Update();
			Quit();
		}

	private:
		void Update() override
		{
			if (!m_EngineInstance) return;

			m_EngineInstance->Update();
		}

		void Quit() override
		{
			if (m_EngineInstance)
			{
				m_EngineInstance->Quit();
			}
		}

	private:
		std::unique_ptr<Engine> m_EngineInstance;
	};
}

using namespace NightlyEditor;

int main(int argc, char** argv)
{
	std::unique_ptr<EditorApp> editor = std::make_unique<EditorApp>();
	editor->Start();
}
