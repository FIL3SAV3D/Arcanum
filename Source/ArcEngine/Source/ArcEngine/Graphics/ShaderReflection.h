#pragma once

#include <slang.h>
#include <slang-com-ptr.h>

#include <string>
#include "Debug/ShaderReflectionHelper.h"

#include <ArcEngine/Graphics/Vulkan-1.4/vulkan-api.h>

class ShaderReflection
{
public:
    void CreateSession();
    void CreateShader(VulkanAPI& api, const char* _ShaderName);

    void AddAutomaticallyIntroducedUniformBuffer(DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder);

    //void DeconstructParameterPrint(slang::VariableLayoutReflection* _layout);
    
    void CheckDiagnostics(Slang::ComPtr<slang::IBlob> _Diagnostics);

private:
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;

    ShaderReflectionHelper helper;
    Slang::ComPtr<slang::IGlobalSession> globalSession;
};