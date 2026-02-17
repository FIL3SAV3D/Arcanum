#pragma once

#include <slang.h>
#include <slang-com-ptr.h>

#include <string>

struct AccessPathNode
{
    slang::VariableLayoutReflection* varLayout;
    AccessPathNode* outer;
};

struct AccessPath
{
    AccessPathNode* leaf = nullptr;
    AccessPathNode* deepestConstantBuffer = nullptr;
    AccessPathNode* deepestParameterBlock = nullptr;
};


struct CumulativeOffset
{
    int value; // the actual offset
    int space; // the associated space
};

class ShaderReflectionHelper
{
public:
    void PrintQuotedString(const char* _string);
    void PrintVariable(slang::VariableReflection* variable);
    void PrintType(slang::TypeReflection* type);
    void PrintPossiblyUnbounded(size_t value);
    void PrintResourceShape(SlangResourceShape shape);

    void PrintVarLayout(slang::VariableLayoutReflection* varLayout, AccessPath accessPath);
    void PrintTypeLayout(slang::TypeLayoutReflection* typeLayout, AccessPath accessPath);

    void PrintOffset(slang::VariableLayoutReflection* varLayout, slang::ParameterCategory layoutUnit);
    void PrintOffsets(slang::VariableLayoutReflection* varLayout, AccessPath accessPath);

    void PrintRelativeOffsets(slang::VariableLayoutReflection* varLayout);

    void PrintSize(slang::TypeLayoutReflection* typeLayout, slang::ParameterCategory layoutUnit);
    void PrintSizes(slang::TypeLayoutReflection* typeLayout);

    void PrintScalarType(slang::TypeReflection::ScalarType _type);
    void PrintTypeKind(slang::TypeReflection::Kind _kind);
    void PrintCount(size_t _count);
    void PrintResourceAccess(SlangResourceAccess _access);
    void PrintLayoutUnit(slang::ParameterCategory _unit);

    void printWithDepth(const char* _string);
    void EndLine();

    void PrintProgramLayout(slang::ProgramLayout* programLayout);
    void PrintScope(slang::VariableLayoutReflection* scopeVarLayout, AccessPath accessPath);

    void PrintEntryPointLayout(slang::EntryPointReflection* entryPointLayout);

    CumulativeOffset CalculateCumulativeOffset(slang::ParameterCategory layoutUnit, AccessPath accessPath);

    void PrintCumulativeOffset(slang::VariableLayoutReflection* varLayout, slang::ParameterCategory layoutUnit, AccessPath accessPath);

private:
    int currentIndent = 0;
    int lastIndent = 0;
    std::string indentString;

};