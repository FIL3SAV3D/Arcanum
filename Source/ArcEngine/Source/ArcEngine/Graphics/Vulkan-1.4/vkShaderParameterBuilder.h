#pragma once

#include <vulkan/vulkan.h>

#include <slang.h>
#include <slang-com-ptr.h>
#include "vulkan-api.h"

#include <vector>

typedef SlangResult Result;

struct PipelineLayoutBuilder
{
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    std::vector<VkPushConstantRange> pushConstantRanges;
};

struct DescriptorSetLayoutBuilder
{
    std::vector<VkDescriptorSetLayoutBinding> descriptorRanges;

    int setIndex = -1;
};

class ShaderParameterBuilder
{
public:
    Result FinishBuildingPipelineLayout(PipelineLayoutBuilder& builder, VkPipelineLayout* outPipelineLayout);
    void FinishBuildingDescriptorSetLayout(PipelineLayoutBuilder& pipelineLayoutBuilder, DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder);
    void AddDescriptorSetForParameterBlock(PipelineLayoutBuilder& pipelineLayoutBuilder, slang::TypeLayoutReflection* parameterBlockTypeLayout);
    void AddRangesForParameterBlockElement(PipelineLayoutBuilder& pipelineLayoutBuilder, DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder, slang::TypeLayoutReflection* elementTypeLayout);
    void AddAutomaticallyIntroducedUniformBuffer(DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder);
    void StartBuildingDescriptorSetLayout(PipelineLayoutBuilder& pipelineLayoutBuilder, DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder);
    void FilterOutEmptyDescriptorSets(PipelineLayoutBuilder& builder);
    void AddDescriptorRanges(DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder, slang::TypeLayoutReflection* typeLayout);
    void AddDescriptorRange(DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder, slang::TypeLayoutReflection* typeLayout, int relativeSetIndex, int rangeIndex);
    VkDescriptorType MapSlangBindingTypeToVulkanDescriptorType(slang::BindingType bindingType);
    void AddRanges(PipelineLayoutBuilder& pipelineLayoutBuilder, DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder, slang::TypeLayoutReflection* typeLayout);
    void AddSubObjectRanges(PipelineLayoutBuilder& pipelineLayoutBuilder, DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder, slang::TypeLayoutReflection* typeLayout);
    void AddSubObjectRange(PipelineLayoutBuilder& pipelineLayoutBuilder, DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder, slang::TypeLayoutReflection* typeLayout, int subObjectRangeIndex);
    void AddPushConstantRangeForConstantBuffer(PipelineLayoutBuilder& pipelineLayoutBuilder, slang::TypeLayoutReflection* pushConstantBufferTypeLayout);
    Result CreatePipelineLayout(slang::ProgramLayout* programLayout, VkPipelineLayout* outPipelineLayout);
    void AddGlobalScopeParameters(PipelineLayoutBuilder& pipelineLayoutBuilder, DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder, slang::ProgramLayout* programLayout);
    void AddEntryPointParameters(PipelineLayoutBuilder& pipelineLayoutBuilder, DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder, slang::ProgramLayout* programLayout);
    void AddEntryPointParameters(PipelineLayoutBuilder& pipelineLayoutBuilder, DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder, slang::EntryPointLayout* entryPointLayout);
    VkShaderStageFlags GetShaderStageFlags(SlangStage stage);
    Result ValidatePipelineLayout(VkPipelineLayout pipelineLayout);
    Result CreateAndValidatePipelineLayout(VulkanAPI& vkAPI);

public:
    VkShaderStageFlags _currentStageFlags = VK_SHADER_STAGE_ALL;
    slang::ProgramLayout* _slangProgramLayout = nullptr;
    slang::IBlob* _slangCompiledProgramBlob = nullptr;

private:
    VulkanAPI* vkAPI;

};