#include "ShaderReflection.h"
#include <string>

#include "fmt/core.h"

#include <slang-cpp-types-core.h>
#include <vector>
#include <vulkan/vulkan.h>


void ShaderReflection::CreateSession()
{
    SlangGlobalSessionDesc desc = {};

    assert(SLANG_FAILED(createGlobalSession(&desc, globalSession.writeRef())) && "Failed to Create Global Session");
}

void ShaderReflection::CreateShader(VulkanAPI& api, const char* _ShaderName)
{
    // First we need to create slang global session with work with the Slang API.
    Slang::ComPtr<slang::IGlobalSession> slangGlobalSession;
    assert(SLANG_SUCCEEDED(slang::createGlobalSession(slangGlobalSession.writeRef())) && "Failed to Create Global Session");

    // Next we create a compilation session to generate SPIRV code from Slang source.
    slang::SessionDesc sessionDesc = {};
    slang::TargetDesc targetDesc = {};
    targetDesc.format = SLANG_SPIRV;
    targetDesc.profile = slangGlobalSession->findProfile("spirv_1_5");
    targetDesc.flags = 0;

    sessionDesc.targets = &targetDesc;
    sessionDesc.targetCount = 1;
    sessionDesc.compilerOptionEntryCount = 0;

    const char* searchPaths[] = { "../Shaders/ShadersVulkan/Source/VulkanShaders/" };
    sessionDesc.searchPaths = searchPaths;
    sessionDesc.searchPathCount = 1;

    slang::PreprocessorMacroDesc fancyFlag = { "ENABLE_FANCY_FEATURE", "1" };
    sessionDesc.preprocessorMacros = &fancyFlag;
    sessionDesc.preprocessorMacroCount = 1;

    Slang::ComPtr<slang::ISession> session;
    assert(SLANG_SUCCEEDED(slangGlobalSession->createSession(sessionDesc, session.writeRef())) && "Failed to Create Session");

    // Once the session has been obtained, we can start loading code into it.
    //
    // The simplest way to load code is by calling `loadModule` with the name of a Slang
    // module. A call to `loadModule("hello-world")` will behave more or less as if you
    // wrote:
    //
    //      import hello_world;
    //
    // In a Slang shader file. The compiler will use its search paths to try to locate
    // `hello-world.slang`, then compile and load that file. If a matching module had
    // already been loaded previously, that would be used directly.

    Slang::ComPtr<slang::IBlob> diagnostics;
    Slang::ComPtr<slang::IModule> module;
    module = session->loadModule(_ShaderName, diagnostics.writeRef());
    diagnoseIfNeeded(diagnostics);
    if (!module)
        return;

    std::vector<slang::IComponentType*> componentsToLink;

    // ### Variable decls
    //
    helper.Key("global constants");
    for (auto decl : module->getModuleReflection()->getChildren())
    {
        if (auto varDecl = decl->asVariable(); varDecl &&
            varDecl->findModifier(slang::Modifier::Const) &&
            varDecl->findModifier(slang::Modifier::Static))
        {
            helper.Element();
            helper.PrintVariable(varDecl);
        }
    }

    // ### Finding Entry Points
    //

    helper.Key("defined entry points");
    int definedEntryPointCount = module->getDefinedEntryPointCount();
    for (int i = 0; i < definedEntryPointCount; i++)
    {
        Slang::ComPtr<slang::IEntryPoint> entryPoint;
        SLANG_SUCCEEDED(module->getDefinedEntryPoint(i, entryPoint.writeRef()));

        helper.Element();
        helper.Key("name");
        helper.PrintQuotedString(entryPoint->getFunctionReflection()->getName());

        componentsToLink.push_back(entryPoint.get());
    }

    // Actually creating the composite component type is a single operation
    // on the Slang session, but the operation could potentially fail if
    // something about the composite was invalid (e.g., you are trying to
    // combine multiple copies of the same module), so we need to deal
    // with the possibility of diagnostic output.
    //
    Slang::ComPtr<slang::IComponentType> composed;
    session->createCompositeComponentType(
        (slang::IComponentType**)componentsToLink.data(),
        componentsToLink.size(),
        composed.writeRef(),
        diagnostics.writeRef());
    diagnoseIfNeeded(diagnostics);


    Slang::ComPtr<slang::IComponentType> program;
    composed->link(program.writeRef(), diagnostics.writeRef());
    diagnoseIfNeeded(diagnostics);

    helper.Key("layouts");
    for (int targetIndex = 0; targetIndex < 1; ++targetIndex)
    {
        helper.Element();

        // ### Getting the Program Layout
        //
        slang::ProgramLayout* programLayoutitr =
            program->getLayout(targetIndex, diagnostics.writeRef());
        diagnoseIfNeeded(diagnostics);
        if (!programLayoutitr)
        {
            continue;
        }

        helper.CollectEntryPointMetadata(program, targetIndex, definedEntryPointCount);

        helper.SetProgramLayout(programLayoutitr);
        helper.PrintProgramLayout(programLayoutitr, SLANG_SPIRV);
    }

    // Now we can call `composedProgram->getEntryPointCode()` to retrieve the
    // compiled SPIRV code that we will use to create a vulkan compute pipeline.
    // This will trigger the final Slang compilation and spirv code generation.
    Slang::ComPtr<slang::IBlob> spirvCode;
    {
        Slang::ComPtr<slang::IBlob> diagnosticsBlob;
        SlangResult result = program->getEntryPointCode(
            0,
            0,
            spirvCode.writeRef(),
            diagnosticsBlob.writeRef());
        CheckDiagnostics(diagnosticsBlob);
        assert(SLANG_SUCCEEDED(result) && "Failed to Compile Program");

        //if (isTestMode())
        //{
        //    printEntrypointHashes(1, 1, composedProgram);
        //}
    }

    // First we need to create a descriptor set layout and a pipeline layout.
    // In this example, the pipeline layout is simple: we have a single descriptor
    // set with three buffer descriptors for our input/output storage buffers.
    // General applications typically has much more complicated pipeline layouts,
    // and should consider using Slang's reflection API to learn about the shader
    // parameter layout of a shader program. However, Slang's reflection API is
    // out of scope of this example.
    //VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo = {
    //    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
    //descSetLayoutCreateInfo.bindingCount = 3;
    //VkDescriptorSetLayoutBinding bindings[3];

    //for (int i = 0; i < 3; i++)
    //{
    //    auto& binding = bindings[i];
    //    binding.binding = i;
    //    binding.descriptorCount = 1;
    //    binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    //    binding.stageFlags = VK_SHADER_STAGE_ALL;
    //    binding.pImmutableSamplers = nullptr;
    //}

    //descSetLayoutCreateInfo.pBindings = bindings;

    //api.vkCreateDescriptorSetLayout(
    //    api.device,
    //    &descSetLayoutCreateInfo,
    //    nullptr,
    //    &descriptorSetLayout);

    //VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
    //    VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
    //pipelineLayoutCreateInfo.setLayoutCount = 1;
    //pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;

    //api.vkCreatePipelineLayout(
    //    api.device,
    //    &pipelineLayoutCreateInfo,
    //    nullptr,
    //    &pipelineLayout);


    //// Next we create a shader module from the compiled SPIRV code.
    //VkShaderModuleCreateInfo shaderCreateInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    //shaderCreateInfo.codeSize = spirvCode->getBufferSize();
    //shaderCreateInfo.pCode = static_cast<const uint32_t*>(spirvCode->getBufferPointer());
    //VkShaderModule vkShaderModule;
    //api.vkCreateShaderModule(api.device, &shaderCreateInfo, nullptr, &vkShaderModule);

    //// Now we have all we need to create a compute pipeline.
    //VkComputePipelineCreateInfo pipelineCreateInfo = {
    //    VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };
    //pipelineCreateInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    //pipelineCreateInfo.stage.module = vkShaderModule;
    //pipelineCreateInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    //pipelineCreateInfo.stage.pName = "main";
    //pipelineCreateInfo.layout = pipelineLayout;
    //api.vkCreateComputePipelines(
    //    api.device,
    //    VK_NULL_HANDLE,
    //    1,
    //    &pipelineCreateInfo,
    //    nullptr,
    //    &pipeline);

    //// We can destroy shader module now since it will no longer be used.
    //api.vkDestroyShaderModule(api.device, vkShaderModule, nullptr);
}

void ShaderReflection::AddAutomaticallyIntroducedUniformBuffer(
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