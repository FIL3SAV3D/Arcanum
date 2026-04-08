#include "VulkanGraphics.h"
#include <SDL3/SDL_vulkan.h>

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

void ArcEngine::VulkanGraphics::Create(const Window& _Window)
{
    initVulkan();
}

void ArcEngine::VulkanGraphics::Destroy()
{
}

void ArcEngine::VulkanGraphics::Resize(const glm::uvec2& _Size)
{
}

void ArcEngine::VulkanGraphics::UpdateCameraData(const glm::vec4& _Position, const glm::mat4& _View, const glm::mat4& _Projection)
{
}

void ArcEngine::VulkanGraphics::FrameStart(const Window& _Window)
{
}

void ArcEngine::VulkanGraphics::FrameEnd(const Window& _Window)
{
}

void ArcEngine::VulkanGraphics::Blit()
{
}

void ArcEngine::VulkanGraphics::RenderMesh(std::shared_ptr<Model> _Model, const glm::mat4x4& _ObjectToWorld)
{
}

void ArcEngine::VulkanGraphics::RenderMeshInstanced()
{
}

void ArcEngine::VulkanGraphics::RenderMeshIndirect()
{
}

////////////////

void ArcEngine::VulkanGraphics::initVulkan() {
    createInstance();
}

void ArcEngine::VulkanGraphics::mainLoop() {

}

void ArcEngine::VulkanGraphics::cleanup() {

}

void ArcEngine::VulkanGraphics::createInstance()
{
    constexpr vk::ApplicationInfo appInfo{.pApplicationName = "Hello Triangle",
                                          .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                                          .pEngineName = "No Engine",
                                          .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                                          .apiVersion = vk::ApiVersion14};

    // Get the required layers
    std::vector<char const*> requiredLayers;
    if (enableValidationLayers) {
        requiredLayers.assign(validationLayers.begin(), validationLayers.end());
    }

    // Check if the required layers are supported by the Vulkan implementation.
    auto layerProperties = context.enumerateInstanceLayerProperties();
    auto unsupportedLayerIt = std::ranges::find_if(requiredLayers,
        [&layerProperties](auto const& requiredLayer) {
            return std::ranges::none_of(layerProperties,
                [requiredLayer](auto const& layerProperty) { return strcmp(layerProperty.layerName, requiredLayer) == 0; });
        });
    if (unsupportedLayerIt != requiredLayers.end())
    {
        throw std::runtime_error("Required layer not supported: " + std::string(*unsupportedLayerIt));
    }

    // Get the required extensions.
    auto requiredExtensions = getRequiredInstanceExtensions();

    // Check if the required extensions are supported by the Vulkan implementation.
    auto extensionProperties = context.enumerateInstanceExtensionProperties();
    auto unsupportedPropertyIt =
        std::ranges::find_if(requiredExtensions,
            [&extensionProperties](auto const& requiredExtension) {
                return std::ranges::none_of(extensionProperties,
                    [requiredExtension](auto const& extensionProperty) { return strcmp(extensionProperty.extensionName, requiredExtension) == 0; });
            });
    if (unsupportedPropertyIt != requiredExtensions.end())
    {
        throw std::runtime_error("Required extension not supported: " + std::string(*unsupportedPropertyIt));
    }

    vk::InstanceCreateInfo createInfo{
       .pApplicationInfo        = &appInfo,
       .enabledLayerCount       = static_cast<uint32_t>(requiredLayers.size()),
       .ppEnabledLayerNames     = requiredLayers.data(),
       .enabledExtensionCount   = static_cast<uint32_t>(requiredExtensions.size()),
       .ppEnabledExtensionNames = requiredExtensions.data()};
    instance = vk::raii::Instance(context, createInfo);
}

void ArcEngine::VulkanGraphics::setupDebugMessenger()
{
    if (!enableValidationLayers) return;

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT     messageTypeFlags(
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{.messageSeverity = severityFlags,
                                                                          .messageType = messageTypeFlags,
                                                                          .pfnUserCallback = &debugCallback };
    debugMessenger = instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
}

std::vector<const char*> ArcEngine::VulkanGraphics::getRequiredInstanceExtensions()
{
    uint32_t SDLExtensionCount = 0;
    auto SDLExtensions = SDL_Vulkan_GetInstanceExtensions(&SDLExtensionCount);

    std::vector extensions(SDLExtensions, SDLExtensions + SDLExtensionCount);
    if (enableValidationLayers)
    {
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
    }

    return extensions;
}
