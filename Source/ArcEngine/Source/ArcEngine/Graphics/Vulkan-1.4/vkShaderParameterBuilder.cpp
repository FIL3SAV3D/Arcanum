#include "vkShaderParameterBuilder.h"

Result ShaderParameterBuilder::FinishBuildingPipelineLayout(
    PipelineLayoutBuilder& builder,
    VkPipelineLayout* outPipelineLayout)
{
    FilterOutEmptyDescriptorSets(builder);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };

    pipelineLayoutInfo.setLayoutCount = builder.descriptorSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = builder.descriptorSetLayouts.data();

    pipelineLayoutInfo.pushConstantRangeCount = builder.pushConstantRanges.size();
    pipelineLayoutInfo.pPushConstantRanges = builder.pushConstantRanges.data();

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    vkAPI->vkCreatePipelineLayout(vkAPI->device, &pipelineLayoutInfo, nullptr, &pipelineLayout);

    *outPipelineLayout = pipelineLayout;
    return SLANG_OK;
}

void ShaderParameterBuilder::FinishBuildingDescriptorSetLayout(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder)
{
    if (descriptorSetLayoutBuilder.descriptorRanges.empty())
        return;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };

    descriptorSetLayoutInfo.bindingCount = descriptorSetLayoutBuilder.descriptorRanges.size();
    descriptorSetLayoutInfo.pBindings = descriptorSetLayoutBuilder.descriptorRanges.data();

    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    vkAPI->vkCreateDescriptorSetLayout(
        vkAPI->device,
        &descriptorSetLayoutInfo,
        nullptr,
        &descriptorSetLayout);

    pipelineLayoutBuilder.descriptorSetLayouts[descriptorSetLayoutBuilder.setIndex] =
        descriptorSetLayout;
}

void ShaderParameterBuilder::AddDescriptorSetForParameterBlock(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    slang::TypeLayoutReflection* parameterBlockTypeLayout)
{
    DescriptorSetLayoutBuilder descriptorSetLayoutBuilder;
    StartBuildingDescriptorSetLayout(pipelineLayoutBuilder, descriptorSetLayoutBuilder);

    AddRangesForParameterBlockElement(
        pipelineLayoutBuilder,
        descriptorSetLayoutBuilder,
        parameterBlockTypeLayout->getElementTypeLayout());

    FinishBuildingDescriptorSetLayout(pipelineLayoutBuilder, descriptorSetLayoutBuilder);
}

void ShaderParameterBuilder::AddRangesForParameterBlockElement(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder,
    slang::TypeLayoutReflection* elementTypeLayout)
{
    if (elementTypeLayout->getSize() > 0)
    {
        AddAutomaticallyIntroducedUniformBuffer(descriptorSetLayoutBuilder);
    }

    // Once we have accounted for the possibility of an implicitly-introduced
    // constant buffer, we can move on and add bindings based on whatever
    // non-ordinary data (textures, buffers, etc.) is in the element type:
    //
    AddRanges(pipelineLayoutBuilder, descriptorSetLayoutBuilder, elementTypeLayout);
}

void ShaderParameterBuilder::AddAutomaticallyIntroducedUniformBuffer(
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder)
{
    auto vulkanBindingIndex = descriptorSetLayoutBuilder.descriptorRanges.size();

    VkDescriptorSetLayoutBinding binding = {};
    binding.stageFlags = VK_SHADER_STAGE_ALL;
    binding.binding = vulkanBindingIndex;
    binding.descriptorCount = 1;
    binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    descriptorSetLayoutBuilder.descriptorRanges.push_back(binding);
}

void ShaderParameterBuilder::StartBuildingDescriptorSetLayout(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder)
{
    descriptorSetLayoutBuilder.setIndex = pipelineLayoutBuilder.descriptorSetLayouts.size();
    pipelineLayoutBuilder.descriptorSetLayouts.push_back(VK_NULL_HANDLE);
}

void ShaderParameterBuilder::FilterOutEmptyDescriptorSets(PipelineLayoutBuilder& builder)
{
    std::vector<VkDescriptorSetLayout> filteredDescriptorSetLayouts;
    for (auto descriptorSetLayout : builder.descriptorSetLayouts)
    {
        if (!descriptorSetLayout)
            continue;
        filteredDescriptorSetLayouts.push_back(descriptorSetLayout);
    }
    std::swap(builder.descriptorSetLayouts, filteredDescriptorSetLayouts);
}

void ShaderParameterBuilder::AddDescriptorRanges(
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder,
    slang::TypeLayoutReflection* typeLayout)
{
    int relativeSetIndex = 0;
    int rangeCount = typeLayout->getDescriptorSetDescriptorRangeCount(relativeSetIndex);

    for (int rangeIndex = 0; rangeIndex < rangeCount; ++rangeIndex)
    {
        AddDescriptorRange(
            descriptorSetLayoutBuilder,
            typeLayout,
            relativeSetIndex,
            rangeIndex);
    }
}

void ShaderParameterBuilder::AddDescriptorRange(
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder,
    slang::TypeLayoutReflection* typeLayout,
    int relativeSetIndex,
    int rangeIndex)
{
    slang::BindingType bindingType =
        typeLayout->getDescriptorSetDescriptorRangeType(relativeSetIndex, rangeIndex);
    auto descriptorCount = typeLayout->getDescriptorSetDescriptorRangeDescriptorCount(
        relativeSetIndex,
        rangeIndex);

    // Some Ranges Need to Be Skipped
    // ------------------------------
    //
    switch (bindingType)
    {
    default:
        break;

    case slang::BindingType::PushConstant:
        return;
    }

    auto bindingIndex = descriptorSetLayoutBuilder.descriptorRanges.size();

    VkDescriptorSetLayoutBinding vulkanBindingRange = {};
    vulkanBindingRange.binding = bindingIndex;
    vulkanBindingRange.descriptorCount = descriptorCount;
    vulkanBindingRange.stageFlags = _currentStageFlags;
    vulkanBindingRange.descriptorType = MapSlangBindingTypeToVulkanDescriptorType(bindingType);

    descriptorSetLayoutBuilder.descriptorRanges.push_back(vulkanBindingRange);
}

VkDescriptorType ShaderParameterBuilder::MapSlangBindingTypeToVulkanDescriptorType(slang::BindingType bindingType)
{
    switch (bindingType)
    {
#define CASE(FROM, TO)             \
    case slang::BindingType::FROM: \
        return VK_DESCRIPTOR_TYPE_##TO

        CASE(Sampler, SAMPLER);
        CASE(CombinedTextureSampler, COMBINED_IMAGE_SAMPLER);
        CASE(Texture, SAMPLED_IMAGE);
        CASE(MutableTexture, STORAGE_IMAGE);
        CASE(TypedBuffer, UNIFORM_TEXEL_BUFFER);
        CASE(MutableTypedBuffer, STORAGE_TEXEL_BUFFER);
        CASE(ConstantBuffer, UNIFORM_BUFFER);
        CASE(RawBuffer, STORAGE_BUFFER);
        CASE(MutableRawBuffer, STORAGE_BUFFER);
        CASE(InputRenderTarget, INPUT_ATTACHMENT);
        CASE(InlineUniformData, INLINE_UNIFORM_BLOCK);
        CASE(RayTracingAccelerationStructure, ACCELERATION_STRUCTURE_KHR);

#undef CASE

    default:
        return VkDescriptorType(-1);
    }
}

void ShaderParameterBuilder::AddRanges(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder,
    slang::TypeLayoutReflection* typeLayout)
{
    AddDescriptorRanges(descriptorSetLayoutBuilder, typeLayout);
    AddSubObjectRanges(pipelineLayoutBuilder, descriptorSetLayoutBuilder, typeLayout);
}

void ShaderParameterBuilder::AddSubObjectRanges(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder,
    slang::TypeLayoutReflection* typeLayout)
{
    int subObjectRangeCount = typeLayout->getSubObjectRangeCount();
    for (int subObjectRangeIndex = 0; subObjectRangeIndex < subObjectRangeCount;
        ++subObjectRangeIndex)
    {
        AddSubObjectRange(
            pipelineLayoutBuilder,
            descriptorSetLayoutBuilder,
            typeLayout,
            subObjectRangeIndex);
    }
}

void ShaderParameterBuilder::AddSubObjectRange(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder,
    slang::TypeLayoutReflection* typeLayout,
    int subObjectRangeIndex)
{
    auto bindingRangeIndex =
        typeLayout->getSubObjectRangeBindingRangeIndex(subObjectRangeIndex);
    auto bindingType = typeLayout->getBindingRangeType(bindingRangeIndex);
    switch (bindingType)
    {
    default:
        return;

        // Nested Parameter Blocks
        // -----------------------

    case slang::BindingType::ParameterBlock:
    {
        auto parameterBlockTypeLayout =
            typeLayout->getBindingRangeLeafTypeLayout(bindingRangeIndex);
        AddDescriptorSetForParameterBlock(pipelineLayoutBuilder, parameterBlockTypeLayout);
    }
    break;

    // Push-Constant Ranges
    // --------------------

    case slang::BindingType::PushConstant:
    {
        auto constantBufferTypeLayout =
            typeLayout->getBindingRangeLeafTypeLayout(bindingRangeIndex);
        AddPushConstantRangeForConstantBuffer(
            pipelineLayoutBuilder,
            constantBufferTypeLayout);
        AddRanges(
            pipelineLayoutBuilder,
            descriptorSetLayoutBuilder,
            constantBufferTypeLayout->getElementTypeLayout());
    }
    break;

    // Nested Constant Buffers
    // -----------------------

    case slang::BindingType::ConstantBuffer:
    {
        auto constantBufferTypeLayout =
            typeLayout->getBindingRangeLeafTypeLayout(bindingRangeIndex);
        AddRangesForParameterBlockElement(
            pipelineLayoutBuilder,
            descriptorSetLayoutBuilder,
            constantBufferTypeLayout->getElementTypeLayout());
    }
    break;
    }
}

void ShaderParameterBuilder::AddPushConstantRangeForConstantBuffer(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    slang::TypeLayoutReflection* pushConstantBufferTypeLayout)
{
    auto elementTypeLayout = pushConstantBufferTypeLayout->getElementTypeLayout();
    auto elementSize = elementTypeLayout->getSize();

    if (elementSize == 0)
        return;

    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = _currentStageFlags;
    pushConstantRange.offset = 0;
    pushConstantRange.size = elementSize;

    pipelineLayoutBuilder.pushConstantRanges.push_back(pushConstantRange);
}

Result ShaderParameterBuilder::CreatePipelineLayout(
    slang::ProgramLayout* programLayout,
    VkPipelineLayout* outPipelineLayout)
{
    PipelineLayoutBuilder pipelineLayoutBuilder;

    DescriptorSetLayoutBuilder defaultDescriptorSetLayoutBuilder;
    StartBuildingDescriptorSetLayout(pipelineLayoutBuilder, defaultDescriptorSetLayoutBuilder);

    AddGlobalScopeParameters(
        pipelineLayoutBuilder,
        defaultDescriptorSetLayoutBuilder,
        programLayout);

    AddEntryPointParameters(
        pipelineLayoutBuilder,
        defaultDescriptorSetLayoutBuilder,
        programLayout);

    FinishBuildingDescriptorSetLayout(pipelineLayoutBuilder, defaultDescriptorSetLayoutBuilder);
    FinishBuildingPipelineLayout(pipelineLayoutBuilder, outPipelineLayout);

    return SLANG_OK;
}

void ShaderParameterBuilder::AddGlobalScopeParameters(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder,
    slang::ProgramLayout* programLayout)
{
    _currentStageFlags = VK_SHADER_STAGE_ALL;
    AddRangesForParameterBlockElement(
        pipelineLayoutBuilder,
        descriptorSetLayoutBuilder,
        programLayout->getGlobalParamsTypeLayout());
}

void ShaderParameterBuilder::AddEntryPointParameters(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder,
    slang::ProgramLayout* programLayout)
{
    int entryPointCount = _slangProgramLayout->getEntryPointCount();
    for (int i = 0; i < entryPointCount; ++i)
    {
        auto entryPointLayout = _slangProgramLayout->getEntryPointByIndex(i);
        AddEntryPointParameters(
            pipelineLayoutBuilder,
            descriptorSetLayoutBuilder,
            entryPointLayout);
    }
}

void ShaderParameterBuilder::AddEntryPointParameters(
    PipelineLayoutBuilder& pipelineLayoutBuilder,
    DescriptorSetLayoutBuilder& descriptorSetLayoutBuilder,
    slang::EntryPointLayout* entryPointLayout)
{
    _currentStageFlags = GetShaderStageFlags(entryPointLayout->getStage());
    AddRangesForParameterBlockElement(
        pipelineLayoutBuilder,
        descriptorSetLayoutBuilder,
        entryPointLayout->getTypeLayout());
}

VkShaderStageFlags ShaderParameterBuilder::GetShaderStageFlags(SlangStage stage)
{
    switch (stage)
    {
#define CASE(FROM, TO)       \
    case SLANG_STAGE_##FROM: \
        return VK_SHADER_STAGE_##TO

        CASE(VERTEX, VERTEX_BIT);
        CASE(HULL, TESSELLATION_CONTROL_BIT);
        CASE(DOMAIN, TESSELLATION_EVALUATION_BIT);
        CASE(GEOMETRY, GEOMETRY_BIT);
        CASE(FRAGMENT, FRAGMENT_BIT);
        CASE(COMPUTE, COMPUTE_BIT);
        CASE(RAY_GENERATION, RAYGEN_BIT_KHR);
        CASE(ANY_HIT, ANY_HIT_BIT_KHR);
        CASE(CLOSEST_HIT, CLOSEST_HIT_BIT_KHR);
        CASE(MISS, MISS_BIT_KHR);
        CASE(INTERSECTION, INTERSECTION_BIT_KHR);
        CASE(CALLABLE, CALLABLE_BIT_KHR);
        CASE(MESH, MESH_BIT_EXT);
        CASE(AMPLIFICATION, TASK_BIT_EXT);

#undef CASE
    default:
        return VK_SHADER_STAGE_ALL;
    }
}

Result ShaderParameterBuilder::ValidatePipelineLayout(VkPipelineLayout pipelineLayout)
{
    VkShaderModuleCreateInfo shaderModuleInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    shaderModuleInfo.pCode = (uint32_t const*)_slangCompiledProgramBlob->getBufferPointer();
    shaderModuleInfo.codeSize = _slangCompiledProgramBlob->getBufferSize();

    VkShaderModule vkShaderModule;
    vkAPI->vkCreateShaderModule(vkAPI->device, &shaderModuleInfo, nullptr, &vkShaderModule);

    VkComputePipelineCreateInfo pipelineInfo = { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineInfo.stage.module = vkShaderModule;
    pipelineInfo.stage.pName = "main";
    pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;

    VkPipeline pipeline;
    vkAPI->vkCreateComputePipelines(
        vkAPI->device,
        VK_NULL_HANDLE,
        1,
        &pipelineInfo,
        nullptr,
        &pipeline);

    vkAPI->vkDestroyPipeline(vkAPI->device, pipeline, nullptr);

    return SLANG_OK;
}

Result ShaderParameterBuilder::CreateAndValidatePipelineLayout(VulkanAPI& _vkAPI)
{
    // Once the setup is dealt with, we can go ahead and
    // create the pipeline layout, before validating that
    // it can be used together with the compiled SPIR-V
    // binary for the program.
    //
    vkAPI = &_vkAPI;

    VkPipelineLayout pipelineLayout;
    SLANG_RETURN_ON_FAIL(CreatePipelineLayout(_slangProgramLayout, &pipelineLayout));
    SLANG_RETURN_ON_FAIL(ValidatePipelineLayout(pipelineLayout));

    vkAPI->vkDestroyPipelineLayout(vkAPI->device, pipelineLayout, nullptr);

    return SLANG_OK;
}