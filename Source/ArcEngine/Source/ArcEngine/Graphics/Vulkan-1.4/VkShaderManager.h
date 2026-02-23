#pragma once

#include "vulkan-api.h"

#include <vulkan/vulkan.h>

#include <slang.h>
#include <slang-com-ptr.h>

#include <vector>

struct ShaderObject
{
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
};

class VkShaderManager
{
public:
    void Create();
    void Destroy();

    void CreateShader(VulkanAPI& api);
    void DestroyShader(VulkanAPI& api);
private:
    std::vector<ShaderObject> mShaderList;

    Slang::ComPtr<slang::IGlobalSession> mSlangGlobalSession;
};