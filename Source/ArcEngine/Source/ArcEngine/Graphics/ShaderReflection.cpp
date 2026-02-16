#include "ShaderReflection.h"

#include <slang.h>
#include "slang-com-ptr.h"

// https://shader-slang.org/slang/user-guide/reflection.html

void ShaderReflection::CreateShader()
{
    Slang::ComPtr<slang::IGlobalSession> globalSession;
    SlangGlobalSessionDesc desc = {};
    createGlobalSession(&desc, globalSession.writeRef());

    slang::IComponentType* program = ...;
    slang::ProgramLayout* programLayout = program->getLayout(targetIndex);
    programLayout->getGlobalParamsVarLayout();
}