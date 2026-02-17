#pragma once

#include <slang.h>
#include <slang-com-ptr.h>

#include <string>
#include "Debug/ShaderReflectionHelper.h"

class ShaderReflection
{
public:
    void CreateSession();
    void CreateShader();

    void DeconstructParameterPrint(slang::VariableLayoutReflection* _layout);

    void printVariable(slang::VariableReflection* variable);
    void printType(slang::TypeReflection* type);


private:
    ShaderReflectionHelper helper;
    Slang::ComPtr<slang::IGlobalSession> globalSession;
};