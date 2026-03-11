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
    Slang::ComPtr<slang::IBlob> diagnostics;
    Result result = SLANG_OK;

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
    helper.NewLine();

    // Once the program has been compiled succcessfully, we can
        // go ahead and grab reflection data from the program.
        //
    int targetIndex = 0;
    slang::ProgramLayout* programLayout =
        program->getLayout(targetIndex, diagnostics.writeRef());
    diagnoseIfNeeded(diagnostics);
    if (!programLayout)
    {
        return;
    }

    // The compiled program can also have binary code (either
    // for individual entry points, or the entire program)
    // generated for it.
    //
    Slang::ComPtr<slang::IBlob> programBinary;
    program->getEntryPointCode(0, 0, programBinary.writeRef(), diagnostics.writeRef());
    diagnoseIfNeeded(diagnostics);

    // Building Shader
    builder._slangProgramLayout = programLayout;
    builder._slangCompiledProgramBlob = programBinary;
    builder.CreateAndValidatePipelineLayout(api);
}