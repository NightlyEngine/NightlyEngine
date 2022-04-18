#include "Runtime/Core/Application.h"
#include "Runtime/Core/Engine.h"

namespace NightlyEditor
{
    class EditorApp : public Nightly::Application
	{
	public:
		EditorApp() = default;

		~EditorApp() override = default;

		void Start() override
		{
			m_EngineInstance = new Nightly::Engine;
			m_EngineInstance->Start();

			std::cout << "Hello from Nightly Editor!\n";
			Update();
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

			delete m_EngineInstance;
		}

	private:
		Nightly::Engine* m_EngineInstance = nullptr;
	};
}

int main(int argc, char** argv)
{
	auto editor = new NightlyEditor::EditorApp;
	editor->Start();
	delete editor;
}
