#include "EditorUI.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
//#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "Platform/PlatformDetection.h"
#include "Core/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/VulkanAPI.h"

#include <vulkan/vulkan.h>

using namespace Nightly;

namespace NightlyEditor
{
	EditorUI::~EditorUI() = default;

	void EditorUI::Initialize(const std::shared_ptr<Window>& editorWindow, ImGui_ImplVulkanH_Window* window)
	{
		NL_ASSERT(glfwVulkanSupported(), "Vulkan is not supported!", EDITOR);

		m_Window = window;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.ConfigInputTextCursorBlink = true;
		io.ConfigDockingTransparentPayload = true;

		#ifdef NL_PLATFORM_OSX
		io.ConfigMacOSXBehaviors = true;
		#endif

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0;

		//ImGui_ImplGlfw_InitForOpenGL(editorWindow->GetNativeWindow(), true);

		io.Fonts->AddFontFromFileTTF("../../Source/ThirdParty/imgui/misc/fonts/Roboto-Medium.ttf", 16.0f);
	}

	void EditorUI::BeginFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void EditorUI::Update()
	{
		BeginFrame();

		#if 0
		// Dock space
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		ImGui::End();
		#endif

		ImGui::ShowDemoWindow();

		ImGui::Begin("Test");

		ImGui::Text("Test");
		ImGui::End();

		#if 0
		// Test window
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		float width = (float) Renderer::GetFramebuffer().GetProps().GetWidth();
		float height = (float) Renderer::GetFramebuffer().GetProps().GetHeight();

		/*ImGui::GetWindowDrawList()->AddImage((void*) Renderer::GetFramebuffer().GetColorBuffer(),
											 ImVec2(ImGui::GetCursorScreenPos()),
											 ImVec2(ImGui::GetCursorScreenPos().x + width,
													ImGui::GetCursorScreenPos().y + height),
											 ImVec2(0, 1),
											 ImVec2(1, 0));*/

		ImVec2 size = ImGui::GetContentRegionAvail();

		Renderer::SetViewportSize(Vec2(size.x, size.y));

		ImGui::Image((void*) (Renderer::GetFramebuffer().GetColorBuffer()), size);

		ImGui::End();
		ImGui::PopStyleVar();
		#endif

		EndFrame();
	}

	void EditorUI::Render()
	{
	}

	void EditorUI::EndFrame()
	{
		ImGui::Render();

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), VulkanAPI::m_CommandBuffers[VulkanAPI::currentFrame]);

		//ImDrawData* data = ImGui::GetDrawData();

		//Renderer::Update();

		/*ImGui_ImplVulkanH_Frame* fd = &m_Window->Frames[m_Window->FrameIndex];
		ImGui_ImplVulkan_RenderDrawData(data, fd->CommandBuffer);
*/
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();


		//FramePresent();

		/*glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);*/

		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/*GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);*/
	}

	void EditorUI::Terminate()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorUI::FrameRender(struct ImDrawData* drawData)
	{
		VkResult err;

		VkSemaphore image_acquired_semaphore = m_Window->FrameSemaphores[m_Window->SemaphoreIndex].ImageAcquiredSemaphore;
		VkSemaphore render_complete_semaphore = m_Window->FrameSemaphores[m_Window->SemaphoreIndex].RenderCompleteSemaphore;
		err = vkAcquireNextImageKHR(VulkanAPI::m_Device, m_Window->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE,
		                            &m_Window->FrameIndex);
		//check_vk_result(err);

		ImGui_ImplVulkanH_Frame* fd = &m_Window->Frames[m_Window->FrameIndex];
		{
			err = vkWaitForFences(VulkanAPI::m_Device, 1, &fd->Fence, VK_TRUE,
			                      UINT64_MAX);    // wait indefinitely instead of periodically checking
			//check_vk_result(err);

			err = vkResetFences(VulkanAPI::m_Device, 1, &fd->Fence);
			//check_vk_result(err);
		}
		{
			err = vkResetCommandPool(VulkanAPI::m_Device, fd->CommandPool, 0);
			//check_vk_result(err);
			VkCommandBufferBeginInfo info = { };
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
			//check_vk_result(err);
		}
		{
			VkRenderPassBeginInfo info = { };
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = m_Window->RenderPass;
			info.framebuffer = fd->Framebuffer;
			info.renderArea.extent.width = m_Window->Width;
			info.renderArea.extent.height = m_Window->Height;
			info.clearValueCount = 1;
			info.pClearValues = &m_Window->ClearValue;
			vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		}

		// Record dear imgui primitives into command buffer
		ImGui_ImplVulkan_RenderDrawData(drawData, fd->CommandBuffer);

		// Submit command buffer
		vkCmdEndRenderPass(fd->CommandBuffer);
		{
			VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			VkSubmitInfo info = { };
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.waitSemaphoreCount = 1;
			info.pWaitSemaphores = &image_acquired_semaphore;
			info.pWaitDstStageMask = &wait_stage;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &fd->CommandBuffer;
			info.signalSemaphoreCount = 1;
			info.pSignalSemaphores = &render_complete_semaphore;

			err = vkEndCommandBuffer(fd->CommandBuffer);
			//check_vk_result(err);
			err = vkQueueSubmit(VulkanAPI::m_GraphicsQueue, 1, &info, fd->Fence);
			//check_vk_result(err);
		}
	}

	void EditorUI::FramePresent()
	{
		VkSemaphore render_complete_semaphore = m_Window->FrameSemaphores[m_Window->SemaphoreIndex].RenderCompleteSemaphore;
		VkPresentInfoKHR info = { };
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &render_complete_semaphore;
		info.swapchainCount = 1;
		info.pSwapchains = &m_Window->Swapchain;
		info.pImageIndices = &m_Window->FrameIndex;
		VkResult err = vkQueuePresentKHR(VulkanAPI::m_PresentQueue, &info);
		//check_vk_result(err);
		m_Window->SemaphoreIndex = (m_Window->SemaphoreIndex + 1) % m_Window->ImageCount;
	}
}