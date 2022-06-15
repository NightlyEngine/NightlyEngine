#pragma once

#include <vulkan/vulkan.h>

namespace Nightly
{
	class Window;
}

namespace NightlyEditor
{
	class UIBackend
	{
	public:
		UIBackend() = default;
		~UIBackend() = default;

		static void Initialize(std::shared_ptr<Nightly::Window> window);
		static void Draw();

		static inline std::vector<VkCommandBuffer> m_CommandBuffers;

	private:
		static void CreateCommandPool();
		static void CreateCommandBuffers();
		static void CreateImage();
		static void CreateImageViews();
		static void CreateContext();
		static void CreateDescriptorPool();
		static void CreateRenderPass();
		static void CreateFramebuffers();

		static void InsertImageMemoryBarrier(VkCommandBuffer cmdbuffer, VkImage image, VkAccessFlags srcAccessMask,
		                                     VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
		                                     VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
		                                     VkImageSubresourceRange subresourceRange);

		static VkCommandBuffer BeginCommand(const VkCommandPool& commandPool);
		static void EndCommand(const VkCommandBuffer& buffer, const VkCommandPool& pool);

		static uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties);

		static inline VkCommandPool m_CommandPool;
		static inline VkDescriptorPool m_DescriptorPool;

		static inline VkSampler m_TextureSampler;
		static inline std::vector<VkImage> m_Images;
		static inline std::vector<VkImageView> m_ImageViews;
		static inline std::vector<VkDeviceMemory> m_DstImageMemory;

		//static inline VkRenderPass m_RenderPass;
		static inline std::vector<VkFramebuffer> m_Framebuffers;
		//static inline VkPipeline m_Pipeline;

		static inline uint32_t m_QueueFamily;
	};
}
