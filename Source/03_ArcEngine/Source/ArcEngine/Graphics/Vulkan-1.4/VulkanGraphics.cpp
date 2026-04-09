#include "VulkanGraphics.h"
#include <SDL3/SDL_vulkan.h>
#include <map>

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
    setupDebugMessenger();
    pickPhysicalDevice();
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

bool ArcEngine::VulkanGraphics::isDeviceSuitable(vk::raii::PhysicalDevice const& physicalDevice)
{
    // Check if the physicalDevice supports the Vulkan 1.3 API version
    bool supportsVulkan1_3 = physicalDevice.getProperties().apiVersion >= vk::ApiVersion13;

    // Check if any of the queue families support graphics operations
    auto queueFamilies = physicalDevice.getQueueFamilyProperties();
    bool supportsGraphics = std::ranges::any_of(queueFamilies, [](auto const& qfp) { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });

    // Check if all required physicalDevice extensions are available
    auto availableDeviceExtensions = physicalDevice.enumerateDeviceExtensionProperties();
    bool supportsAllRequiredExtensions =
        std::ranges::all_of(requiredDeviceExtension,
            [&availableDeviceExtensions](auto const& requiredDeviceExtension)
            {
                return std::ranges::any_of(availableDeviceExtensions,
                    [requiredDeviceExtension](auto const& availableDeviceExtension)
                    { return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0; });
            });

    // Check if the physicalDevice supports the required features (dynamic rendering and extended dynamic state)
    auto features =
        physicalDevice
        .template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
    bool supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
        features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

    // Return true if the physicalDevice meets all the criteria
    return supportsVulkan1_3 && supportsGraphics && supportsAllRequiredExtensions && supportsRequiredFeatures;
}

void ArcEngine::VulkanGraphics::pickPhysicalDevice()
{
    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    auto const devIter = std::ranges::find_if(physicalDevices, [&](auto const& physicalDevice) { return isDeviceSuitable(physicalDevice); });
    if (devIter == physicalDevices.end())
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
    physicalDevice = *devIter;
}