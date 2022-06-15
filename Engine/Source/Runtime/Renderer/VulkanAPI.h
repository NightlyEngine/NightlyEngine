#pragma once

#include "Core/EngineAPI.h"
#include "Core/Core.h"

#include <vulkan/vulkan.h>

namespace Nightly
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		NL_NODISCARD bool IsComplete() const
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		NL_NODISCARD bool IsSupported() const
		{
			return !formats.empty() && !presentModes.empty();
		}
	};

	class NL_API VulkanAPI
	{
	public:
		VulkanAPI() = default;
		~VulkanAPI() = default;

		static void Initialize();
		static void BeginDraw();
		static void EndDraw(const std::vector<VkCommandBuffer>& m_CmdBuffers);
		static void Terminate();

	public:
		static void CreateInstance();
		static void CreateSurface();
		static void PickPhysicalDevice();
		static void CreateLogicalDevice();
		static void CreateSwapChain();
		static void CreateImageViews();
		static void CreateRenderPass();
		static void CreateGraphicsPipeline();
		static void CreateFramebuffers();
		static void CreateCommandPool();
		static void CreateCommandBuffers();
		static void CreateSyncObjects();

		static void RecordCommandBuffer();

		static std::vector<char> ReadFile(const std::string& fileName);

		static bool CheckValidationLayerSupport();
		static bool IsDeviceSuitable(VkPhysicalDevice device);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availableModes);
		static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		static VkShaderModule CreateShaderModule(const std::vector<char>& code);

		static inline constexpr int MAX_FRAMES_IN_FLIGHT = 2;
		static inline const std::array<const char*, 1> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		static inline const std::array<const char*, 1> m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		static inline VkInstance m_Instance;
		static inline VkDevice m_Device;
		static inline VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		static inline VkQueue m_GraphicsQueue;
		static inline VkQueue m_PresentQueue;
		static inline VkSurfaceKHR m_Surface;

		static inline VkSwapchainKHR m_SwapChain;
		static inline std::vector<VkImage> m_SwapChainImages;
		static inline std::vector<VkImageView> m_SwapChainImageViews;
		static inline std::vector<VkFramebuffer> m_SwapChainFramebuffers;
		static inline VkFormat m_SwapChainImageFormat;
		static inline VkExtent2D m_SwapChainExtent;

		static inline VkRenderPass m_RenderPass;
		static inline VkRenderPass m_UIRenderPass;
		static inline VkPipelineLayout m_PipelineLayout;
		static inline VkPipeline m_GraphicsPipeline;
		static inline VkPipeline m_UIPipeline;

		static inline VkCommandPool m_CommandPool;
		static inline std::vector<VkCommandBuffer> m_CommandBuffers;

		static inline std::vector<VkSemaphore> m_ImageReadySemaphores;
		static inline std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		static inline std::vector<VkFence> m_InFlightFences;

		static inline VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
		static inline uint32_t imageIndex;
		static inline uint32_t currentFrame;
		static inline VkSampler m_TextureSampler;
		static inline VkImage frame;
	};
}
