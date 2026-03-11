#pragma once

#include "ArcEngine/Graphics/Interface/ISwapchain.h"

#include <ArcEngine/Graphics/Vulkan-1.4/vk_types.h>

class VulkanSwapchain
{
public:
	struct SwapchainParameters
	{
		glm::uvec2 size;
		VkPhysicalDevice chosenGPU;
		VkDevice device;
		VkSurfaceKHR surface;
	};

public:
    void Create (const SwapchainParameters& _parameters);
    void Destroy(const VkDevice& _device);

	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;
};