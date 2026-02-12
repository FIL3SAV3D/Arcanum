#pragma once

#include "ArcEngine/Graphics/Interface/ISwapchain.h"

#include <ArcEngine/Graphics/Vulkan-1.4/vk_types.h>

class VulkanSwapchain : public ISwapchain
{
public:
    void Create(const glm::uvec2& _Size) override;
    void Destroy() override;

	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;
};