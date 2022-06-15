// Engine includes
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
#include "Renderer/VulkanAPI.h"

// Editor includes
#include "EditorUI.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"
#include "UIBackend.h"

using namespace Nightly;

namespace NightlyEditor
{
	static ImGui_ImplVulkanH_Window g_MainWindowData;
	static VkAllocationCallbacks* g_Allocator = NULL;
	static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;

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

			VulkanAPI::Initialize();
			UIBackend::Initialize(m_EditorWindow);

			#if 0
			ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

			EditorUI::Initialize(m_EditorWindow, wd);

			wd->Surface = VulkanAPI::m_Surface;

			SwapChainSupportDetails details = VulkanAPI::QuerySwapChainSupport(VulkanAPI::m_PhysicalDevice);
			uint32_t count = details.capabilities.minImageCount;

			wd->SurfaceFormat = VulkanAPI::ChooseSwapSurfaceFormat(details.formats);
			wd->PresentMode = VulkanAPI::ChooseSwapPresentMode(details.presentModes);

			QueueFamilyIndices indices = VulkanAPI::FindQueueFamilies(VulkanAPI::m_PhysicalDevice);
			ImGui_ImplVulkanH_CreateOrResizeWindow(VulkanAPI::m_Instance, VulkanAPI::m_PhysicalDevice, VulkanAPI::m_Device, wd,
												   indices.graphicsFamily.value(), nullptr, 1280, 720, count);

			ImGui_ImplGlfw_InitForVulkan(m_EditorWindow->GetNativeWindow(), true);
			ImGui_ImplVulkan_InitInfo init_info = { };
			init_info.Instance = VulkanAPI::m_Instance;
			init_info.PhysicalDevice = VulkanAPI::m_PhysicalDevice;
			init_info.Device = VulkanAPI::m_Device;
			init_info.QueueFamily = indices.graphicsFamily.value();
			init_info.Queue = VulkanAPI::m_GraphicsQueue;
			init_info.PipelineCache = g_PipelineCache;
			init_info.DescriptorPool = VulkanAPI::m_DescriptorPool;
			init_info.Subpass = 0;
			init_info.MinImageCount = count;
			init_info.ImageCount = wd->ImageCount;
			init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			init_info.Allocator = g_Allocator;
			//init_info.CheckVkResultFn = check_vk_result;
			ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

			// Upload Fonts
			{
				VkResult err;

				// Use any command queue
				VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
				VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

				vkResetCommandPool(VulkanAPI::m_Device, command_pool, 0);
				//check_vk_result(err);
				VkCommandBufferBeginInfo begin_info = { };
				begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				err = vkBeginCommandBuffer(command_buffer, &begin_info);
				//check_vk_result(err);

				ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

				VkSubmitInfo end_info = { };
				end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				end_info.commandBufferCount = 1;
				end_info.pCommandBuffers = &command_buffer;
				err = vkEndCommandBuffer(command_buffer);
				//check_vk_result(err);
				err = vkQueueSubmit(VulkanAPI::m_GraphicsQueue, 1, &end_info, VK_NULL_HANDLE);
				//check_vk_result(err);

				err = vkDeviceWaitIdle(VulkanAPI::m_Device);
				//check_vk_result(err);
				ImGui_ImplVulkan_DestroyFontUploadObjects();
			}

			#endif

			//Renderer::Initialize();
			//Renderer::m_DrawFramebufferToScreen = false;

			//EditorUI::Initialize(m_EditorWindow);

			PluginManager::LoadActivePlugins();

			auto world = WorldManager::CreateWorld("New World");
			WorldManager::LoadWorld(world);

			auto parent = world->CreateEntity("Parent Entity");
			parent->Transform()->Translate(Vec3(0, 0, -1));
			//parent->AddComponent<MeshComponent>(std::make_shared<MeshComponent>());

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
			VulkanAPI::BeginDraw();
			UIBackend::Draw();
			VulkanAPI::EndDraw(
					{ VulkanAPI::m_CommandBuffers[VulkanAPI::currentFrame], UIBackend::m_CommandBuffers[VulkanAPI::currentFrame] });
			//EditorUI::Update();
			PluginManager::UpdatePlugins();

			//m_EditorWindow->SwapBuffers();
			WindowManager::PollEvents();
		}

		void Quit() override
		{
			NL_CORE_INFO("Quitting Nightly Editor...", EDITOR);

			if (m_EngineInstance)
			{
				m_EngineInstance->Quit();
			}

			//EditorUI::Terminate();
			m_EditorWindow->Destroy();
			VulkanAPI::Terminate();
			PluginManager::UnloadPlugins();
			//Renderer::Cleanup();
			WindowManager::Terminate();
		}

	private:
		std::unique_ptr<Engine> m_EngineInstance;
		std::shared_ptr<Window> m_EditorWindow;
	};
}

using namespace NightlyEditor;

int main(int argc, char** argv)
{
	std::cout << "\n\n" << "Nightly Engine Copyright (C) 2022 Futureblur\n"
	          << "Welcome to Nightly Engine! Enjoy your time and happy coding :)\n\n";

	if (argv[0])
	{
		std::string path = argv[0];
		std::size_t slashPos = path.find_last_of('/');
		path = path.substr(0, slashPos);

		if (std::filesystem::exists(path))
		{
			std::filesystem::current_path(path);
		}
	}

	auto editor = std::make_unique<EditorApp>();
	editor->Start();
}
