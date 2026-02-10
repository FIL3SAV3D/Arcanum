#pragma once

// --- other includes ---
#include <ArcEngine/Rendering/Vulkan-1.4/vk_types.h>

//bootstrap library
#include "ArcEngine/Rendering/Vulkan-1.4/VKBootstrap/VkBootstrap.h"

class VulkanEngine
{
public:
    VkInstance _instance;// Vulkan library handle
    VkDebugUtilsMessengerEXT _debug_messenger;// Vulkan debug output handle
    VkPhysicalDevice _chosenGPU;// GPU chosen as the default device
    VkDevice _device; // Vulkan device for commands
    VkSurfaceKHR _surface;// Vulkan window surface

    void Initialize();
    void InitializeVulkan();
    void InitializeSwapchain();
    void InitializeCommands();
    void InitializeSyncStructures();

    bool _IsInitialized = false;
};