#pragma once

#include "slang-com-ptr.h"
#include "slang.h"

#include <string>
#include <vector>

#include <fmt/core.h>

// Example for parsing parameters
// https://github.com/shader-slang/slang/blob/master/examples/reflection-parameter-blocks/main.cpp

struct CumulativeOffset
{
    size_t value = 0;
    size_t space = 0;
};

struct AccessPathNode
{
    slang::VariableLayoutReflection* variableLayout = nullptr;
    AccessPathNode* outer = nullptr;
};

struct AccessPath
{
    AccessPath() {}

    bool valid = false;
    AccessPathNode* deepestConstantBufer = nullptr;
    AccessPathNode* deepestParameterBlock = nullptr;
    AccessPathNode* leaf = nullptr;
};

struct ExtendedAccessPath : AccessPath
{
    ExtendedAccessPath(AccessPath const& base, slang::VariableLayoutReflection* variableLayout)
        : AccessPath(base)
    {
        if (!valid)
            return;

        element.variableLayout = variableLayout;
        element.outer = leaf;

        leaf = &element;
    }

    AccessPathNode element;
};

typedef unsigned int StageMask;



#define WITH_ARRAY() for (int _i = (BeginArray(), 1); _i; _i = (EndArray(), 0))

#define SCOPED_OBJECT() ScopedObject scopedObject##__COUNTER__(this)

inline void diagnoseIfNeeded(slang::IBlob* diagnosticsBlob)
{
    if (diagnosticsBlob != nullptr)
    {
        fmt::println("{}", (const char*)diagnosticsBlob->getBufferPointer());
    }
}

class ShaderReflectionHelper
{
public:
    void PrintVariable(slang::VariableReflection* variable);

    void PrintType(slang::TypeReflection* type);

    void PrintPossiblyUnbounded(size_t value);

    void PrintCommonTypeInfo(slang::TypeReflection* type);

    void PrintVariableLayout(slang::VariableLayoutReflection* variableLayout, AccessPath accessPath);

    void PrintRelativeOffsets(slang::VariableLayoutReflection* variableLayout);

    void PrintOffset(slang::VariableLayoutReflection* variableLayout, slang::ParameterCategory layoutUnit);

    void PrintOffset(slang::ParameterCategory layoutUnit, size_t offset, size_t spaceOffset);

    void PrintTypeLayout(slang::TypeLayoutReflection* typeLayout, AccessPath accessPath);

    void PrintSizes(slang::TypeLayoutReflection* typeLayout);

    void PrintSize(slang::TypeLayoutReflection* typeLayout, slang::ParameterCategory layoutUnit);

    void PrintSize(slang::ParameterCategory layoutUnit, size_t size);

    void PrintKindSpecificInfo(slang::TypeLayoutReflection* typeLayout, AccessPath accessPath);

    void PrintProgramLayout(slang::ProgramLayout* programLayout, SlangCompileTarget targetFormat);

    void PrintScope(slang::VariableLayoutReflection* scopeVarLayout, AccessPath accessPath);

    void PrintEntryPointLayout(slang::EntryPointReflection* entryPointLayout, AccessPath accessPath);

    void PrintStageSpecificInfo(slang::EntryPointReflection* entryPointLayout);

    void PrintVaryingParameterInfo(slang::VariableLayoutReflection* variableLayout);

    void PrintCumulativeOffsets(slang::VariableLayoutReflection* variableLayout, AccessPath accessPath);

    CumulativeOffset CalculateCumulativeOffset(slang::VariableLayoutReflection* variableLayout, slang::ParameterCategory layoutUnit, AccessPath accessPath);

    void PrintCumulativeOffset(slang::VariableLayoutReflection* variableLayout, slang::ParameterCategory layoutUnit, AccessPath accessPath);

    CumulativeOffset CalculateCumulativeOffset(slang::ParameterCategory layoutUnit, AccessPath accessPath);

    Slang::Result CollectEntryPointMetadata(slang::IComponentType* program, int targetIndex, int entryPointCount);

    StageMask CalculateParameterStageMask(slang::ParameterCategory layoutUnit, CumulativeOffset offset);

    StageMask CalculateStageMask(slang::VariableLayoutReflection* variableLayout, AccessPath accessPath);

    void PrintStageUsage(slang::VariableLayoutReflection* variableLayout, AccessPath accessPath);

    void PrintOffsets(slang::VariableLayoutReflection* variableLayout, AccessPath accessPath);

    void PrintIndentation();

    void BeginObject();

    void EndObject();

    void BeginArray();

    void EndArray();

    void NewLine();

    void Element();

    void Key(char const* key);

    void PrintQuotedString(char const* text);

    void PrintBool(bool value);

    void Print(size_t value);

    void PrintComment(char const* text);

    void PrintTypeKind(slang::TypeReflection::Kind kind);

    void PrintResourceShape(SlangResourceShape shape);

    void PrintResourceAccess(SlangResourceAccess access);

    void PrintLayoutUnit(slang::ParameterCategory layoutUnit);

    void PrintStage(SlangStage stage);

    void PrintTargetFormat(SlangCompileTarget targetFormat);

    void PrintScalarType(slang::TypeReflection::ScalarType scalarType);

    void PrintMatrixLayoutMode(SlangMatrixLayoutMode mode);

    void SetProgramLayout(slang::ProgramLayout* _programLayout) { programLayout = _programLayout; };

private:
    bool afterArrayElement = true;
    slang::ProgramLayout* programLayout = nullptr;

    std::vector<Slang::ComPtr<slang::IMetadata>> metadataForEntryPoints;

    int indentation = 0;
};

struct ScopedObject
{
    ScopedObject(ShaderReflectionHelper* outer)
        : outer(outer)
    {
        outer->BeginObject();
    }

    ~ScopedObject() { outer->EndObject(); }

    ShaderReflectionHelper* outer = nullptr;
};