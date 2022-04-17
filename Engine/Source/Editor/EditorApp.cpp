#include "Runtime/Core/Application.h"

namespace NightlyEditor
{
	class EditorApp : public Nightly::Application
	{
	public:
		EditorApp() = default;

		~EditorApp() override = default;

		void Launch() override
		{
			OnStart();
		}

	private:
		void OnStart() override
		{
			std::cout << "Hello from Nightly Editor!\n";
		}

		void OnUpdate() override
		{

		}

		void OnQuit() override
		{

		}
	};
}

int main(int argc, char** argv)
{
	auto app = new NightlyEditor::EditorApp;
	app->Launch();
	delete app;
}