#include "VulkanAPI.h"

#include <GLFW/glfw3.h>
#include <set>

#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/WindowManager.h"
#include "Core/Window.h"

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!

#define CHECK_RESULT(message, func) do { \
VkResult result = func; \
NL_ASSERT(result == VK_SUCCESS, NL_TEXT(message << result), ENGINE); \
} while(0)

#if NDEBUG
static constexpr bool m_EnableValidationLayers = false;
#else
static constexpr bool m_EnableValidationLayers = true;
#endif

namespace Nightly
{
	void VulkanAPI::Initialize()
	{
		CreateInstance();
		CreateSurface();
		PickPhysicalDevice();
		CreateLogicalDevice();

		/*{
			VkDescriptorPoolSize pool_sizes[] =
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
							{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 }
					};
			VkDescriptorPoolCreateInfo pool_info = { };
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
			pool_info.poolSizeCount = (uint32_t) IM_ARRAYSIZE(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;
			CHECK_RESULT("Failed to create descriptor pool:", vkCreateDescriptorPool(m_Device, &pool_info, nullptr, &m_DescriptorPool));
		}*/

		CreateSwapChain();
		CreateImageViews();
		CreateRenderPass();
		CreateGraphicsPipeline();
		CreateFramebuffers();
		CreateCommandPool();
		CreateCommandBuffers();
		CreateSyncObjects();
	}

	void VulkanAPI::BeginDraw()
	{
		vkWaitForFences(m_Device, 1, &m_InFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(m_Device, 1, &m_InFlightFences[currentFrame]);

		vkAcquireNextImageKHR(m_Device, m_SwapChain, UINT64_MAX, m_ImageReadySemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		//vkResetCommandBuffer(m_CommandBuffers[currentFrame], 0);
		RecordCommandBuffer();
	}

	void VulkanAPI::EndDraw(const std::vector<VkCommandBuffer>& m_CmdBuffers)
	{
		VkSubmitInfo submitInfo { };
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageReadySemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = (uint32_t) m_CmdBuffers.size();
		submitInfo.pCommandBuffers = m_CmdBuffers.data();

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		CHECK_RESULT("Failed to submit queue: ", vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[currentFrame]));

		VkPresentInfoKHR presentInfo { };
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(m_PresentQueue, &presentInfo);

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanAPI::Terminate()
	{
		vkDeviceWaitIdle(m_Device);

		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(m_Device, m_ImageReadySemaphores[i], nullptr);
			vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(m_Device, m_InFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

		for (auto& framebuffer : m_SwapChainFramebuffers)
		{
			vkDestroyFramebuffer(m_Device, framebuffer, nullptr);
		}

		vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
		vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

		for (auto& view : m_SwapChainImageViews)
		{
			vkDestroyImageView(m_Device, view, nullptr);
		}

		vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
		vkDestroyDevice(m_Device, nullptr);
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}

	void VulkanAPI::CreateInstance()
	{
		if (m_EnableValidationLayers && !CheckValidationLayerSupport())
		{
			NL_CORE_FATAL("Requested validation layers are not available!", ENGINE);
		}

		// Create application info
		VkApplicationInfo info { };
		info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		info.pApplicationName = "Nightly Engine";
		info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
		info.pEngineName = "Nightly Engine";
		info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
		info.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo { };
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &info;

		// Get extensions
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = (uint32_t) m_ValidationLayers.size();
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		CHECK_RESULT("Failed to create instance: ", vkCreateInstance(&createInfo, nullptr, &m_Instance));

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		NL_CORE_INFO(extensionCount << " available vulkan extensions found: ", ENGINE);
		for (const auto& ext : extensions)
		{
			NL_CORE_INFO(ext.extensionName, ENGINE);
		}
	}

	void VulkanAPI::CreateSurface()
	{
		CHECK_RESULT("Failed to create window surface: ",
		             glfwCreateWindowSurface(m_Instance, WindowManager::GetCurrentWindow()->GetNativeWindow(), nullptr, &m_Surface));
	}

	void VulkanAPI::PickPhysicalDevice()
	{
		uint32_t count = 0;
		vkEnumeratePhysicalDevices(m_Instance, &count, nullptr);

		NL_ASSERT(count != 0, "Failed to find GPUs with Vulkan support!", ENGINE);

		std::vector<VkPhysicalDevice> devices(count);
		vkEnumeratePhysicalDevices(m_Instance, &count, devices.data());

		for (const auto& device : devices)
		{
			if (IsDeviceSuitable(device))
			{
				m_PhysicalDevice = device;
				break;
			}
		}

		NL_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU!", ENGINE);
	}

	void VulkanAPI::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float priority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo { };
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &priority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures { };

		VkDeviceCreateInfo createInfo { };
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = (uint32_t) queueCreateInfos.size();
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = (uint32_t) m_DeviceExtensions.size();
		createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = (uint32_t) m_ValidationLayers.size();
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		CHECK_RESULT("Failed to create logical device: ", vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device));

		vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);
	}

	void VulkanAPI::CreateSwapChain()
	{
		SwapChainSupportDetails details = QuerySwapChainSupport(m_PhysicalDevice);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(details.formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(details.presentModes);
		VkExtent2D extent = ChooseSwapExtent(details.capabilities);

		uint32_t imageCount = details.capabilities.minImageCount + 1;
		if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
		{
			imageCount = details.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo { };
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = details.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		CHECK_RESULT("Failed to create swap chain: ", vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain));

		vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);
		m_SwapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data());

		m_SwapChainImageFormat = surfaceFormat.format;
		m_SwapChainExtent = extent;
	}

	void VulkanAPI::CreateImageViews()
	{
		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); i++)
		{
			VkImageViewCreateInfo info { };
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.image = m_SwapChainImages[i];
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = m_SwapChainImageFormat;

			info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = 1;

			CHECK_RESULT("Failed to create image views: ", vkCreateImageView(m_Device, &info, nullptr, &m_SwapChainImageViews[i]));
		}
	}

	void VulkanAPI::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment { };
		colorAttachment.format = m_SwapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference attachmentRef { };
		attachmentRef.attachment = 0;
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass { };
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &attachmentRef;

		VkSubpassDependency dependency { };
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo info { };
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount = 1;
		info.pAttachments = &colorAttachment;
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 1;
		info.pDependencies = &dependency;

		CHECK_RESULT("Failed to create render pass!", vkCreateRenderPass(m_Device, &info, nullptr, &m_RenderPass));
		CHECK_RESULT("Failed to create render pass!", vkCreateRenderPass(m_Device, &info, nullptr, &m_UIRenderPass));
	}

	void VulkanAPI::CreateGraphicsPipeline()
	{
		auto vert = ReadFile("../../Source/Shaders/Vulkan/UnlitVert.spv");
		auto frag = ReadFile("../../Source/Shaders/Vulkan/UnlitFrag.spv");

		VkShaderModule vertSM = CreateShaderModule(vert);
		VkShaderModule fragSM = CreateShaderModule(frag);

		VkPipelineShaderStageCreateInfo vertInfo { };
		vertInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertInfo.module = vertSM;
		vertInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragInfo { };
		fragInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragInfo.module = fragSM;
		fragInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertInfo, fragInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo { };
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly { };
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport { };
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float) m_SwapChainExtent.width;
		viewport.height = (float) m_SwapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor { };
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState { };
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer { };
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;

		VkPipelineMultisampleStateCreateInfo multisampling { };
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachment { };
		colorBlendAttachment.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending { };
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		const std::array<VkDynamicState, 2> dynamicStates =
				{
						VK_DYNAMIC_STATE_VIEWPORT,
						VK_DYNAMIC_STATE_LINE_WIDTH
				};

		VkPipelineDynamicStateCreateInfo dynamicState { };
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = (uint32_t) dynamicStates.size();
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo { };
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = 0;
		pipelineLayoutCreateInfo.pSetLayouts = nullptr;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		CHECK_RESULT("Failed to create pipeline layout: ",
		             vkCreatePipelineLayout(m_Device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout));

		VkGraphicsPipelineCreateInfo pipelineInfo { };
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;

		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr;

		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		CHECK_RESULT("Failed to create graphics pipeline: ",
		             vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline));

		pipelineInfo.renderPass = m_UIRenderPass;

		CHECK_RESULT("Failed to create graphics pipeline: ",
		             vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_UIPipeline));

		vkDestroyShaderModule(m_Device, vertSM, nullptr);
		vkDestroyShaderModule(m_Device, fragSM, nullptr);
	}

	void VulkanAPI::CreateFramebuffers()
	{
		m_SwapChainFramebuffers.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImageViews.size(); i++)
		{
			VkImageView attachments[] = { m_SwapChainImageViews[i] };

			VkFramebufferCreateInfo info { };
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.renderPass = m_RenderPass;
			info.attachmentCount = 1;
			info.pAttachments = attachments;
			info.width = m_SwapChainExtent.width;
			info.height = m_SwapChainExtent.height;
			info.layers = 1;

			CHECK_RESULT("Failed to create framebuffer: ", vkCreateFramebuffer(m_Device, &info, nullptr, &m_SwapChainFramebuffers[i]));
		}
	}

	void VulkanAPI::CreateCommandPool()
	{
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

		VkCommandPoolCreateInfo info { };
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		info.queueFamilyIndex = indices.graphicsFamily.value();

		CHECK_RESULT("Failed to create command pool: ", vkCreateCommandPool(m_Device, &info, nullptr, &m_CommandPool));
	}

	void VulkanAPI::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo info { };
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = m_CommandPool;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount = (uint32_t) m_CommandBuffers.size();

		CHECK_RESULT("Failed to allocate command buffers: ", vkAllocateCommandBuffers(m_Device, &info, m_CommandBuffers.data()));
	}

	void VulkanAPI::CreateSyncObjects()
	{
		m_ImageReadySemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo { };
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo { };
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			std::string msg = "Failed to create synchronization objects: ";
			CHECK_RESULT(msg, vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_ImageReadySemaphores[i]));
			CHECK_RESULT(msg, vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]));
			CHECK_RESULT(msg, vkCreateFence(m_Device, &fenceInfo, nullptr, &m_InFlightFences[i]));
		}
	}

	void VulkanAPI::RecordCommandBuffer()
	{
		VkCommandBufferBeginInfo beginInfo { };
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		CHECK_RESULT("Failed to begin recording command buffer: ", vkBeginCommandBuffer(m_CommandBuffers[currentFrame], &beginInfo));

		VkRenderPassBeginInfo renderPassInfo { };
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_RenderPass;
		renderPassInfo.framebuffer = m_SwapChainFramebuffers[imageIndex];

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChainExtent;

		VkClearValue clearColor = { { { 27 / 255.0f, 25 / 255.0f, 50 / 255.0f, 1.0f } } };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(m_CommandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(m_CommandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
		vkCmdDraw(m_CommandBuffers[currentFrame], 3, 1, 0, 0);

		vkCmdEndRenderPass(m_CommandBuffers[currentFrame]);
		CHECK_RESULT("Failed to record command buffer: ", vkEndCommandBuffer(m_CommandBuffers[currentFrame]));
	}

	std::vector<char> VulkanAPI::ReadFile(const std::string& fileName)
	{
		std::ifstream file(fileName, std::ios::ate | std::ios::binary);

		NL_ASSERT(file.is_open(), "Failed to open file!", ENGINE);

		size_t size = (size_t) file.tellg();
		std::vector<char> buffer(size);

		file.seekg(0);
		file.read(buffer.data(), size);
		file.close();

		return buffer;
	}

	bool VulkanAPI::CheckValidationLayerSupport()
	{
		uint32_t count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		std::vector<VkLayerProperties> availableLayers(count);
		vkEnumerateInstanceLayerProperties(&count, availableLayers.data());

		for (const auto& layerName : m_ValidationLayers)
		{
			bool found = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				return false;
			}
		}

		return true;
	}

	bool VulkanAPI::IsDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = FindQueueFamilies(device);
		bool extensionsSupported = CheckDeviceExtensionSupport(device);

		bool swapChainSupported = false;
		if (extensionsSupported)
		{
			auto details = QuerySwapChainSupport(device);
			swapChainSupported = details.IsSupported();
		}

		return indices.IsComplete() && extensionsSupported && swapChainSupported;
	}

	bool VulkanAPI::CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t count;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(count);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &count, availableExtensions.data());

		std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	QueueFamilyIndices VulkanAPI::FindQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);

			if (presentSupport)
			{
				indices.presentFamily = i;
			}

			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			if (indices.IsComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}

	SwapChainSupportDetails VulkanAPI::QuerySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities);

		{
			uint32_t count;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &count, nullptr);

			if (count != 0)
			{
				details.formats.resize(count);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &count, details.formats.data());
			}
		}

		{
			uint32_t count;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &count, nullptr);

			if (count != 0)
			{
				details.presentModes.resize(count);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &count, details.presentModes.data());
			}
		}

		return details;
	}

	VkSurfaceFormatKHR VulkanAPI::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& available : availableFormats)
		{
			if (available.format == VK_FORMAT_R8G8B8A8_SRGB && available.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return available;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanAPI::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availableModes)
	{
		for (const auto& mode : availableModes)
		{
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return mode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanAPI::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			int width = WindowManager::GetCurrentWindow()->GetWidth();
			int height = WindowManager::GetCurrentWindow()->GetHeight();

			VkExtent2D extent = { (uint32_t) width, (uint32_t) height };
			extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return extent;
		}
	}

	VkShaderModule VulkanAPI::CreateShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo info { };
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = code.size();
		info.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule module;
		CHECK_RESULT("Failed to create shader module: ", vkCreateShaderModule(m_Device, &info, nullptr, &module));

		return module;
	}
}