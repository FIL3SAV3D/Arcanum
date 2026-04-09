#pragma once

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "ArcEngine/Graphics/Interface/IGraphics.h"

namespace ArcEngine
{
    class VulkanGraphics : public IGraphics {
    private:
        vk::raii::Context context;
        vk::raii::Instance instance = nullptr;
        vk::raii::PhysicalDevice physicalDevice = nullptr;
        vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

        const std::vector<char const*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char*> requiredDeviceExtension = { vk::KHRSwapchainExtensionName };

    public:
        void Run();

        // Inherited via IGraphics
        void Create(const Window& _Window) override;
        void Destroy() override;
        void Resize(const glm::uvec2& _Size) override;
        void UpdateCameraData(const glm::vec4& _Position, const glm::mat4& _View, const glm::mat4& _Projection) override;
        void FrameStart(const Window& _Window) override;
        void FrameEnd(const Window& _Window) override;
        void Blit() override;
        void RenderMesh(std::shared_ptr<Model> _Model, const glm::mat4x4& _ObjectToWorld) override;
        void RenderMeshInstanced() override;
        void RenderMeshIndirect() override;


    private:
        void initVulkan();

        void mainLoop();

        void cleanup();

        void createInstance();

        void setupDebugMessenger();

        void pickPhysicalDevice();

        bool isDeviceSuitable(vk::raii::PhysicalDevice const& physicalDevice);

        std::vector<const char*> getRequiredInstanceExtensions();

        static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT      severity,
                                                              vk::DebugUtilsMessageTypeFlagsEXT             type,
                                                              const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                              void*                                         pUserData)
        {
            std::cerr << "validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;

            return vk::False;
        }
    };
}