#include "VulkanGraphics.h"
#include <SDL3/SDL_vulkan.h>

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

    auto extensions = context.enumerateInstanceExtensionProperties();

    std::cout << "available extensions:\n";

    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    uint32_t SDLExtensionCount = 0;
    auto SDLExtensions = SDL_Vulkan_GetInstanceExtensions(&SDLExtensionCount);

    auto extensionProperties = context.enumerateInstanceExtensionProperties();
    for (uint32_t i = 0; i < SDLExtensionCount; ++i)
    {
        if (std::ranges::none_of(extensionProperties,
            [SDLExtension = SDLExtensions[i]](auto const& extensionProperty)
            { return strcmp(extensionProperty.extensionName, SDLExtension) == 0; }))
        {
            throw std::runtime_error("Required SDL extension not supported: " + std::string(SDLExtensions[i]));
        }
    }

    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = SDLExtensionCount,
        .ppEnabledExtensionNames = SDLExtensions };

    instance = vk::raii::Instance(context, createInfo);
}
