#include "VulkanEngine.h"

constexpr bool bUseValidationLayers = true;

void VulkanEngine::Initialize()
{
	SDL

	InitializeVulkan();

	InitializeSwapchain();

	InitializeCommands();

	InitializeSyncStructures();

	//everything went fine
	_IsInitialized = true;
}

void VulkanEngine::InitializeVulkan()
{
	vkb::InstanceBuilder builder;

	//make the vulkan instance, with basic debug features
	auto inst_ret = builder.set_app_name("Example Vulkan Application")
		.request_validation_layers(bUseValidationLayers)
		.use_default_debug_messenger()
		.require_api_version(1, 3, 0)
		.build();

	vkb::Instance vkb_inst = inst_ret.value();

	//grab the instance 
	_instance = vkb_inst.instance;
	_debug_messenger = vkb_inst.debug_messenger;

}

void VulkanEngine::InitializeSwapchain()
{
}

void VulkanEngine::InitializeCommands()
{
}

void VulkanEngine::InitializeSyncStructures()
{
}
