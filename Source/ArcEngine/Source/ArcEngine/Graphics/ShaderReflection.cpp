#include "ShaderReflection.h"
#include <string>

#include "fmt/core.h"

void ShaderReflection::CreateSession()
{
    SlangGlobalSessionDesc desc = {};

    if (SLANG_FAILED(createGlobalSession(&desc, globalSession.writeRef())))
    {
        throw;
    }

}

void ShaderReflection::CreateShader()
{

    slang::TargetDesc targetDesc = {};

    targetDesc.format = SLANG_GLSL;
    targetDesc.profile = globalSession->findProfile("glsl_460");

    slang::SessionDesc sessionDesc = {};
    sessionDesc.targets = &targetDesc;
    sessionDesc.targetCount = 1;

    const char* searchPaths[] = { "../Shaders/ShadersVulkan/Source/VulkanShaders/" };
    sessionDesc.searchPaths = searchPaths;
    sessionDesc.searchPathCount = 1;

    slang::PreprocessorMacroDesc fancyFlag = { "ENABLE_FANCY_FEATURE", "1" };
    sessionDesc.preprocessorMacros = &fancyFlag;
    sessionDesc.preprocessorMacroCount = 1;


    Slang::ComPtr<slang::ISession> session;
    globalSession->createSession(sessionDesc, session.writeRef());

    std::string name = "test";

    Slang::ComPtr<slang::IBlob> diagnostics;
    Slang::ComPtr<slang::IModule> module(session->loadModule(name.c_str(), diagnostics.writeRef()));

    std::printf("Module %s Debug Reflection:\n\n", name.c_str());

    if (diagnostics)
    {
        fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
    }

    Slang::ComPtr<slang::IEntryPoint> computeEntryPoint;
    module->findEntryPointByName("main", computeEntryPoint.writeRef());

    // program creation
    slang::IComponentType* components[] = { module/*, entryPoint*/ };
    Slang::ComPtr<slang::IComponentType> program;

    session->createCompositeComponentType(components, 1, program.writeRef(), diagnostics.writeRef());
    if (diagnostics)
    {
        fprintf(stderr, "%s\n", (const char*)diagnostics->getBufferPointer());
    }

    slang::ProgramLayout* layout = program->getLayout();

    unsigned int parameterCount = layout->getParameterCount();
    for (unsigned int i = 0; i < parameterCount; i++)
    {
        DeconstructParameterPrint(layout->getParameterByIndex(i));
    }
}

void ShaderReflection::DeconstructParameterPrint(slang::VariableLayoutReflection* _layout)
{
    helper.PrintVarLayout(_layout);
}