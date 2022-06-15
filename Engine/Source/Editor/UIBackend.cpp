#include "UIBackend.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "Renderer/VulkanAPI.h"
#include "Core/Window.h"

#define CHECK_RESULT(message, func) do { \
VkResult result = func; \
NL_ASSERT(result == VK_SUCCESS, NL_TEXT(message << result), ENGINE); \
} while(0)

using namespace Nightly;

namespace NightlyEditor
{
	void UIBackend::Initialize(std::shared_ptr<Window> window)
	{
		auto details = VulkanAPI::FindQueueFamilies(VulkanAPI::m_PhysicalDevice);
		m_QueueFamily = details.graphicsFamily.value();

		CreateCommandPool();
		CreateCommandBuffers();
		CreateImage();
		CreateImageViews();
		CreateContext();
		CreateDescriptorPool();
		//CreateRenderPass();
		CreateFramebuffers();

		ImGui_ImplGlfw_InitForVulkan(window->GetNativeWindow(), true);

		ImGui_ImplVulkan_InitInfo info { };
		info.Instance = VulkanAPI::m_Instance;
		info.Device = VulkanAPI::m_Device;
		info.PhysicalDevice = VulkanAPI::m_PhysicalDevice;
		info.Allocator = nullptr;
		info.Queue = VulkanAPI::m_GraphicsQueue;
		info.QueueFamily = m_QueueFamily;
		info.DescriptorPool = m_DescriptorPool;
		info.MinImageCount = 2;
		info.ImageCount = 2;
		info.Subpass = 0;
		info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		info.CheckVkResultFn = nullptr;

		ImGui_ImplVulkan_Init(&info, VulkanAPI::m_UIRenderPass);

		{
			auto buffer = BeginCommand(m_CommandPool);
			ImGui_ImplVulkan_CreateFontsTexture(buffer);
			EndCommand(buffer, m_CommandPool);
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}


	}

	void UIBackend::Draw()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		VkCommandBufferBeginInfo info { };
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		CHECK_RESULT("Failed to begin command buffer: ", vkBeginCommandBuffer(m_CommandBuffers[VulkanAPI::imageIndex], &info));

		VkRenderPassBeginInfo rpbInfo { };
		rpbInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		rpbInfo.renderPass = VulkanAPI::m_UIRenderPass;
		rpbInfo.framebuffer = m_Framebuffers[VulkanAPI::imageIndex];
		rpbInfo.renderArea.offset = { 0, 0 };
		rpbInfo.renderArea.extent = VulkanAPI::m_SwapChainExtent;

		auto clearValue = VkClearValue();
		clearValue.color = VkClearColorValue({ { 1, 0, 0, 1 } });

		rpbInfo.clearValueCount = 1;
		rpbInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(m_CommandBuffers[VulkanAPI::currentFrame], &rpbInfo, VK_SUBPASS_CONTENTS_INLINE);

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_CommandBuffers[VulkanAPI::currentFrame]);

		vkCmdEndRenderPass(m_CommandBuffers[VulkanAPI::currentFrame]);
		vkEndCommandBuffer(m_CommandBuffers[VulkanAPI::currentFrame]);
	}

	void UIBackend::CreateImage()
	{
		const size_t size = VulkanAPI::m_SwapChainImages.size();
		m_Images.resize(size);
		m_DstImageMemory.resize(size);

		for (uint32_t i = 0; i < size; i++)
		{
			VkImageCreateInfo info { };
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.imageType = VK_IMAGE_TYPE_2D;

			info.format = VK_FORMAT_B8G8R8A8_SRGB;
			info.extent.width = VulkanAPI::m_SwapChainExtent.width;
			info.extent.height = VulkanAPI::m_SwapChainExtent.height;
			info.extent.depth = 1;
			info.arrayLayers = 1;
			info.mipLevels = 1;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.tiling = VK_IMAGE_TILING_LINEAR;
			info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

			CHECK_RESULT("Failed to create image: ", vkCreateImage(VulkanAPI::m_Device, &info, nullptr, &m_Images[i]));

			VkMemoryRequirements requirements;
			VkMemoryAllocateInfo allocateInfo { };
			allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

			vkGetImageMemoryRequirements(VulkanAPI::m_Device, m_Images[i], &requirements);

			allocateInfo.allocationSize = requirements.size;
			allocateInfo.memoryTypeIndex = FindMemoryType(requirements.memoryTypeBits,
			                                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			CHECK_RESULT("Failed to allocate memory for image: ",
			             vkAllocateMemory(VulkanAPI::m_Device, &allocateInfo, nullptr, &m_DstImageMemory[i]));

			CHECK_RESULT("Failed to bind image memory: ", vkBindImageMemory(VulkanAPI::m_Device, m_Images[i], m_DstImageMemory[i], 0));

			VkCommandBuffer copyCMD = BeginCommand(m_CommandPool);
			InsertImageMemoryBarrier(copyCMD, m_Images[i], VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_MEMORY_READ_BIT,
			                         VK_IMAGE_LAYOUT_UNDEFINED,
			                         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_PIPELINE_STAGE_TRANSFER_BIT,
			                         VK_PIPELINE_STAGE_TRANSFER_BIT,
			                         VkImageSubresourceRange { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

			EndCommand(copyCMD, m_CommandPool);
		}
	}

	void UIBackend::CreateImageViews()
	{
		m_ImageViews.resize(m_Images.size());
		for (auto& m_Image : m_Images)
		{
			VkImageViewCreateInfo info { };
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.image = m_Image;
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = VK_FORMAT_B8G8R8A8_SRGB;

			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = 1;

			VkImageView view;
			CHECK_RESULT("Failed to create image view: ", vkCreateImageView(VulkanAPI::m_Device, &info, nullptr, &view));
		}
	}

	void UIBackend::CreateContext()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
		io.ConfigMacOSXBehaviors = true;
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.ConfigInputTextCursorBlink = true;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}


	void UIBackend::CreateDescriptorPool()
	{
		VkDescriptorPoolSize sizes[] =
				{
						{ VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
						{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
						{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
						{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000 },
						{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000 },
						{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000 },
						{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
						{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
						{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
						{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
						{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 } };

		VkDescriptorPoolCreateInfo info = { };
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		info.maxSets = 1000 * IM_ARRAYSIZE(sizes);
		info.poolSizeCount = (uint32_t) IM_ARRAYSIZE(sizes);
		info.pPoolSizes = sizes;

		CHECK_RESULT("Failed to create descriptor pool: ", vkCreateDescriptorPool(VulkanAPI::m_Device, &info, nullptr, &m_DescriptorPool));
	}

	void UIBackend::CreateRenderPass()
	{
		VkAttachmentDescription attachment { };
		attachment.format = VulkanAPI::m_SwapChainImageFormat;
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference reference { };
		reference.attachment = 0;
		reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass { };
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &reference;
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		VkSubpassDependency dependency = { };
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0; // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo info { };
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount = 1;
		info.pAttachments = &attachment;
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 1;
		info.pDependencies = &dependency;

		CHECK_RESULT("Failed to create render pass: ", vkCreateRenderPass(VulkanAPI::m_Device, &info, nullptr, &VulkanAPI::m_UIRenderPass));
	}

	void UIBackend::CreateCommandPool()
	{
		VkCommandPoolCreateInfo info { };
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		info.queueFamilyIndex = m_QueueFamily;

		CHECK_RESULT("Failed to create command pool: ", vkCreateCommandPool(VulkanAPI::m_Device, &info, nullptr, &m_CommandPool));
	}

	void UIBackend::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(VulkanAPI::m_SwapChainImageViews.size());

		VkCommandBufferAllocateInfo info { };
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = m_CommandPool;
		info.commandBufferCount = (uint32_t) m_CommandBuffers.size();
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		CHECK_RESULT("Failed to create command buffer: ", vkAllocateCommandBuffers(VulkanAPI::m_Device, &info, m_CommandBuffers.data()));
	}

	void UIBackend::CreateFramebuffers()
	{
		m_Framebuffers.resize(VulkanAPI::m_SwapChainImageViews.size());

		VkImageView attachment[1];
		VkFramebufferCreateInfo info { };
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.attachmentCount = 1;
		info.pAttachments = attachment;
		info.renderPass = VulkanAPI::m_UIRenderPass;
		info.width = VulkanAPI::m_SwapChainExtent.width;
		info.height = VulkanAPI::m_SwapChainExtent.height;
		info.layers = 1;

		for (uint32_t i = 0; i < VulkanAPI::m_SwapChainImageViews.size(); i++)
		{
			attachment[0] = VulkanAPI::m_SwapChainImageViews[i];
			CHECK_RESULT("Failed to create framebuffer: ",
			             vkCreateFramebuffer(VulkanAPI::m_Device, &info, nullptr, &m_Framebuffers[i]));
		}

		#if 0
		/*for (int i = 0; i < m_ImageViews.size(); ++i)
		{
			VkImageView attachment[1];

			VkFramebufferCreateInfo info { };
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.width = VulkanAPI::m_SwapChainExtent.width;
			info.height = VulkanAPI::m_SwapChainExtent.height;
			info.renderPass = m_RenderPass;
			info.attachmentCount = 1;
			info.pAttachments = attachment;
			info.layers = 1;

			for (int j = 0; j < VulkanAPI::m_SwapChainImageViews.size(); ++j)
			{

			}
		}*/
		#endif
	}

	void UIBackend::InsertImageMemoryBarrier(VkCommandBuffer cmdbuffer, VkImage image, VkAccessFlags srcAccessMask,
	                                         VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
	                                         VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
	                                         VkImageSubresourceRange subresourceRange)
	{
		VkImageMemoryBarrier imageMemoryBarrier { };
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.srcAccessMask = srcAccessMask;
		imageMemoryBarrier.dstAccessMask = dstAccessMask;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}

	VkCommandBuffer UIBackend::BeginCommand(VkCommandPool const& commandPool)
	{
		VkCommandBufferAllocateInfo allocateInfo = { };
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandPool = commandPool;
		allocateInfo.commandBufferCount = 1;

		VkCommandBuffer buffer;
		CHECK_RESULT("Failed to allocate command buffer: ", vkAllocateCommandBuffers(VulkanAPI::m_Device, &allocateInfo, &buffer));

		VkCommandBufferBeginInfo info { };
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		CHECK_RESULT("Failed to begin command buffer: ", vkBeginCommandBuffer(buffer, &info));

		return buffer;
	}

	void UIBackend::EndCommand(const VkCommandBuffer& buffer, const VkCommandPool& pool)
	{
		CHECK_RESULT("Failed to end command buffer: ", vkEndCommandBuffer(buffer));

		VkSubmitInfo info { };
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &buffer;

		CHECK_RESULT("Failed to submit queue: ", vkQueueSubmit(VulkanAPI::m_GraphicsQueue, 1, &info, nullptr));
		vkQueueWaitIdle(VulkanAPI::m_GraphicsQueue);

		vkFreeCommandBuffers(VulkanAPI::m_Device, pool, 1, &buffer);
	}

	uint32_t UIBackend::FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(VulkanAPI::m_PhysicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}
}