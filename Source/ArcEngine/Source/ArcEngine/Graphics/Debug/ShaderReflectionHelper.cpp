#include "ShaderReflectionHelper.h"


void ShaderReflectionHelper::PrintVariable(slang::VariableReflection* variable)
{
    SCOPED_OBJECT();

    const char* name = variable->getName();
    slang::TypeReflection* type = variable->getType();

    Key("name");
    PrintQuotedString(name);
    Key("type");
    PrintType(type);

    int64_t value;
    if (SLANG_SUCCEEDED(variable->getDefaultValueInt(&value)))
    {
        Key("value");
        printf("%" PRId64, value);
    }
}

void ShaderReflectionHelper::PrintType(slang::TypeReflection* type)
{
    SCOPED_OBJECT();

    const char* name = type->getName();
    slang::TypeReflection::Kind kind = type->getKind();

    Key("name");
    PrintQuotedString(name);
    Key("kind");
    PrintTypeKind(kind);

    // There is information that we would like to
    // Print for both types and type layouts, so
    // we will factor the common logic into a
    // subroutine so that we can share the code.
    //
    PrintCommonTypeInfo(type);

    switch (type->getKind())
    {
    default:
        break;

        // #### Structure Types
        //
    case slang::TypeReflection::Kind::Struct:
    {
        Key("fields");
        int fieldCount = type->getFieldCount();

        WITH_ARRAY();
        for (int f = 0; f < fieldCount; f++)
        {
            Element();
            auto field = type->getFieldByIndex(f);

            PrintVariable(field);
        }
    }
    break;

    // #### Array Types
    // #### Vector Types
    // #### Matrix Types
    //
    case slang::TypeReflection::Kind::Array:
    case slang::TypeReflection::Kind::Vector:
    case slang::TypeReflection::Kind::Matrix:
    {
        Key("element type");
        PrintType(type->getElementType());
    }
    break;

    // #### Resource Types
    //
    case slang::TypeReflection::Kind::Resource:
    {
        Key("result type");
        PrintType(type->getResourceResultType());
    }
    break;

    // #### Single-Element Container Types
    //
    case slang::TypeReflection::Kind::ConstantBuffer:
    case slang::TypeReflection::Kind::ParameterBlock:
    case slang::TypeReflection::Kind::TextureBuffer:
    case slang::TypeReflection::Kind::ShaderStorageBuffer:
    {
        Key("element type");
        PrintType(type->getElementType());
    }
    break;
    }
}

void ShaderReflectionHelper::PrintPossiblyUnbounded(size_t value)
{
    if (value == ~size_t(0))
    {
        printf("unbounded");
    }
    else
    {
        printf("%u", unsigned(value));
    }
}

void ShaderReflectionHelper::PrintCommonTypeInfo(slang::TypeReflection* type)
{
    switch (type->getKind())
    {
        // #### Scalar Types
        //
    case slang::TypeReflection::Kind::Scalar:
    {
        Key("scalar type");
        PrintScalarType(type->getScalarType());
    }
    break;

    // #### Array Types
    //
    case slang::TypeReflection::Kind::Array:
    {
        Key("element count");
        PrintPossiblyUnbounded(type->getElementCount());
    }
    break;

    // #### Vector Types
    //
    case slang::TypeReflection::Kind::Vector:
    {
        Key("element count");
        Print(type->getElementCount());
    }
    break;

    // #### Matrix Types
    //
    case slang::TypeReflection::Kind::Matrix:
    {
        Key("row count");
        Print(type->getRowCount());

        Key("column count");
        Print(type->getColumnCount());
    }
    break;

    // #### Resource Types
    //
    case slang::TypeReflection::Kind::Resource:
    {
        Key("shape");
        PrintResourceShape(type->getResourceShape());

        Key("access");
        PrintResourceAccess(type->getResourceAccess());
    }
    break;

    default:
        break;
    }
}

void ShaderReflectionHelper::PrintVariableLayout(slang::VariableLayoutReflection* variableLayout, AccessPath accessPath)
{
    SCOPED_OBJECT();

    Key("name");
    PrintQuotedString(variableLayout->getName());

    PrintOffsets(variableLayout, accessPath);

    PrintVaryingParameterInfo(variableLayout);

    ExtendedAccessPath variablePath(accessPath, variableLayout);

    Key("type layout");
    PrintTypeLayout(variableLayout->getTypeLayout(), variablePath);
}

void ShaderReflectionHelper::PrintRelativeOffsets(slang::VariableLayoutReflection* variableLayout)
{
    Key("relative");
    int usedLayoutUnitCount = variableLayout->getCategoryCount();
    WITH_ARRAY();
    for (int i = 0; i < usedLayoutUnitCount; ++i)
    {
        Element();

        auto layoutUnit = variableLayout->getCategoryByIndex(i);
        PrintOffset(variableLayout, layoutUnit);
    }
}

void ShaderReflectionHelper::PrintOffset(
    slang::VariableLayoutReflection* variableLayout,
    slang::ParameterCategory layoutUnit)
{
    PrintOffset(
        layoutUnit,
        variableLayout->getOffset(layoutUnit),
        variableLayout->getBindingSpace(layoutUnit));
}

void ShaderReflectionHelper::PrintOffset(slang::ParameterCategory layoutUnit, size_t offset, size_t spaceOffset)
{
    SCOPED_OBJECT();

    Key("value");
    Print(offset);
    Key("unit");
    PrintLayoutUnit(layoutUnit);

    // #### Spaces / Sets

    switch (layoutUnit)
    {
    default:
        break;

    case slang::ParameterCategory::ConstantBuffer:
    case slang::ParameterCategory::ShaderResource:
    case slang::ParameterCategory::UnorderedAccess:
    case slang::ParameterCategory::SamplerState:
    case slang::ParameterCategory::DescriptorTableSlot:
        Key("space");
        Print(spaceOffset);
        break;
    }
}

void ShaderReflectionHelper::PrintTypeLayout(slang::TypeLayoutReflection* typeLayout, AccessPath accessPath)
{
    SCOPED_OBJECT();

    Key("name");
    PrintQuotedString(typeLayout->getName());
    Key("kind");
    PrintTypeKind(typeLayout->getKind());
    PrintCommonTypeInfo(typeLayout->getType());

    PrintSizes(typeLayout);

    PrintKindSpecificInfo(typeLayout, accessPath);
}

void ShaderReflectionHelper::PrintSizes(slang::TypeLayoutReflection* typeLayout)
{
    Key("size");

    int usedLayoutUnitCount = typeLayout->getCategoryCount();
    WITH_ARRAY()
        for (int i = 0; i < usedLayoutUnitCount; ++i)
        {
            Element();

            auto layoutUnit = typeLayout->getCategoryByIndex(i);
            PrintSize(typeLayout, layoutUnit);
        }

    // #### Alignment and Stride
    if (typeLayout->getSize() != 0)
    {
        Key("alignment in bytes");
        Print(typeLayout->getAlignment());

        Key("stride in bytes");
        Print(typeLayout->getStride());
    }
}

void ShaderReflectionHelper::PrintSize(slang::TypeLayoutReflection* typeLayout, slang::ParameterCategory layoutUnit)
{
    PrintSize(layoutUnit, typeLayout->getSize(layoutUnit));
}

void ShaderReflectionHelper::PrintSize(slang::ParameterCategory layoutUnit, size_t size)
{
    SCOPED_OBJECT();

    Key("value");
    PrintPossiblyUnbounded(size);
    Key("unit");
    PrintLayoutUnit(layoutUnit);
}

void ShaderReflectionHelper::PrintKindSpecificInfo(slang::TypeLayoutReflection* typeLayout, AccessPath accessPath)
{
    switch (typeLayout->getKind())
    {
        // #### Structure Type Layouts
        //
    case slang::TypeReflection::Kind::Struct:
    {
        Key("fields");

        int fieldCount = typeLayout->getFieldCount();
        WITH_ARRAY()
            for (int f = 0; f < fieldCount; f++)
            {
                Element();

                auto field = typeLayout->getFieldByIndex(f);
                PrintVariableLayout(field, accessPath);
            }
    }
    break;

    // #### Array Type Layouts
    //
    case slang::TypeReflection::Kind::Array:
    {
        Key("element type layout");
        PrintTypeLayout(typeLayout->getElementTypeLayout(), AccessPath());
    }
    break;

    // #### Matrix Type Layouts
    //
    case slang::TypeReflection::Kind::Matrix:
    {
        Key("matrix layout mode");
        PrintMatrixLayoutMode(typeLayout->getMatrixLayoutMode());

        Key("element type layout");
        PrintTypeLayout(typeLayout->getElementTypeLayout(), AccessPath());
    }
    break;

    case slang::TypeReflection::Kind::Vector:
    {
        Key("element type layout");
        PrintTypeLayout(typeLayout->getElementTypeLayout(), AccessPath());
    }
    break;

    // #### Single-Element Containers
    //
    case slang::TypeReflection::Kind::ConstantBuffer:
    case slang::TypeReflection::Kind::ParameterBlock:
    case slang::TypeReflection::Kind::TextureBuffer:
    case slang::TypeReflection::Kind::ShaderStorageBuffer:
    {
        auto containerVarLayout = typeLayout->getContainerVarLayout();
        auto elementVarLayout = typeLayout->getElementVarLayout();

        AccessPath innerOffsets = accessPath;
        innerOffsets.deepestConstantBufer = innerOffsets.leaf;
        if (containerVarLayout->getTypeLayout()->getSize(
            slang::ParameterCategory::SubElementRegisterSpace) != 0)
        {
            innerOffsets.deepestParameterBlock = innerOffsets.leaf;
        }

        Key("container");
        {
            SCOPED_OBJECT();
            PrintOffsets(containerVarLayout, innerOffsets);
        }

        Key("content");
        {
            SCOPED_OBJECT();

            PrintOffsets(elementVarLayout, innerOffsets);

            ExtendedAccessPath elementOffsets(innerOffsets, elementVarLayout);

            Key("type layout");
            PrintTypeLayout(elementVarLayout->getTypeLayout(), elementOffsets);
        }
    }
    break;

    case slang::TypeReflection::Kind::Resource:
    {
        if ((typeLayout->getResourceShape() & SLANG_RESOURCE_BASE_SHAPE_MASK) ==
            SLANG_STRUCTURED_BUFFER)
        {
            Key("element type layout");
            PrintTypeLayout(typeLayout->getElementTypeLayout(), accessPath);
        }
        else
        {
            Key("result type");
            PrintType(typeLayout->getResourceResultType());
        }
    }
    break;

    default:
        break;
    }
}

void ShaderReflectionHelper::PrintProgramLayout(slang::ProgramLayout* programLayout, SlangCompileTarget targetFormat)
{
    SCOPED_OBJECT();

    Key("target");
    PrintTargetFormat(targetFormat);

    AccessPath rootOffsets;
    rootOffsets.valid = true;

    Key("global scope");
    {
        SCOPED_OBJECT();
        PrintScope(programLayout->getGlobalParamsVarLayout(), rootOffsets);
    }

    Key("entry points");
    int entryPointCount = programLayout->getEntryPointCount();
    WITH_ARRAY()
        for (int i = 0; i < entryPointCount; ++i)
        {
            Element();
            PrintEntryPointLayout(programLayout->getEntryPointByIndex(i), rootOffsets);
        }
}

void ShaderReflectionHelper::PrintScope(slang::VariableLayoutReflection* scopeVarLayout, AccessPath accessPath)
{
    ExtendedAccessPath scopeOffsets(accessPath, scopeVarLayout);

    auto scopeTypeLayout = scopeVarLayout->getTypeLayout();
    switch (scopeTypeLayout->getKind())
    {
        // #### Parameters are Grouped Into a Structure
        //
    case slang::TypeReflection::Kind::Struct:
    {
        Key("parameters");

        int paramCount = scopeTypeLayout->getFieldCount();
        for (int i = 0; i < paramCount; i++)
        {
            Element();

            auto param = scopeTypeLayout->getFieldByIndex(i);

            PrintVariableLayout(param, scopeOffsets);
        }
    }
    break;

    // #### Wrapped in a Constant Buffer If Needed
    //
    case slang::TypeReflection::Kind::ConstantBuffer:
        Key("automatically-introduced constant buffer");
        {
            SCOPED_OBJECT();
            PrintOffsets(scopeTypeLayout->getContainerVarLayout(), scopeOffsets);
        }

        PrintScope(scopeTypeLayout->getElementVarLayout(), scopeOffsets);
        break;

        // #### Wrapped in a Parameter Block If Needed
        //
    case slang::TypeReflection::Kind::ParameterBlock:
        Key("automatically-introduced parameter block");
        {
            SCOPED_OBJECT();
            PrintOffsets(scopeTypeLayout->getContainerVarLayout(), scopeOffsets);
        }

        PrintScope(scopeTypeLayout->getElementVarLayout(), scopeOffsets);
        break;

    default:
        // Note that this default case is never expected to
        // arise with the current Slang compiler and reflection
        // API, but we include it here as a kind of failsafe.
        //
        Key("variable layout");
        PrintVariableLayout(scopeVarLayout, accessPath);
        break;
    }
}

void ShaderReflectionHelper::PrintEntryPointLayout(slang::EntryPointReflection* entryPointLayout, AccessPath accessPath)
{
    SCOPED_OBJECT();

    Key("stage");
    PrintStage(entryPointLayout->getStage());

    PrintStageSpecificInfo(entryPointLayout);

    PrintScope(entryPointLayout->getVarLayout(), accessPath);

    auto resultVariableLayout = entryPointLayout->getResultVarLayout();
    if (resultVariableLayout->getTypeLayout()->getKind() != slang::TypeReflection::Kind::None)
    {
        Key("result");
        PrintVariableLayout(resultVariableLayout, accessPath);
    }
}

void ShaderReflectionHelper::PrintStageSpecificInfo(slang::EntryPointReflection* entryPointLayout)
{
    switch (entryPointLayout->getStage())
    {
    default:
        break;

    case SLANG_STAGE_COMPUTE:
    {
        static const int kAxisCount = 3;
        SlangUInt sizes[kAxisCount];
        entryPointLayout->getComputeThreadGroupSize(kAxisCount, sizes);

        Key("thread group size");
        SCOPED_OBJECT();
        Key("x");
        Print(sizes[0]);
        Key("y");
        Print(sizes[1]);
        Key("z");
        Print(sizes[2]);
    }
    break;

    case SLANG_STAGE_FRAGMENT:
        Key("uses any sample-rate inputs");
        PrintBool(entryPointLayout->usesAnySampleRateInput());
        break;
    }
}

void ShaderReflectionHelper::PrintVaryingParameterInfo(slang::VariableLayoutReflection* variableLayout)
{
    if (auto semanticName = variableLayout->getSemanticName())
    {
        Key("semantic");
        SCOPED_OBJECT();
        Key("name");
        PrintQuotedString(semanticName);
        Key("index");
        Print(variableLayout->getSemanticIndex());
    }
}

void ShaderReflectionHelper::PrintCumulativeOffsets(
    slang::VariableLayoutReflection* variableLayout,
    AccessPath accessPath)
{
    Key("cumulative");

    int usedLayoutUnitCount = variableLayout->getCategoryCount();
    WITH_ARRAY();
    for (int i = 0; i < usedLayoutUnitCount; ++i)
    {
        Element();

        auto layoutUnit = variableLayout->getCategoryByIndex(i);
        PrintCumulativeOffset(variableLayout, layoutUnit, accessPath);
    }
}

CumulativeOffset ShaderReflectionHelper::CalculateCumulativeOffset(
    slang::VariableLayoutReflection* variableLayout,
    slang::ParameterCategory layoutUnit,
    AccessPath accessPath)
{
    CumulativeOffset result = CalculateCumulativeOffset(layoutUnit, accessPath);
    result.value += variableLayout->getOffset(layoutUnit);
    result.space += variableLayout->getBindingSpace(layoutUnit);
    return result;
}

void ShaderReflectionHelper::PrintCumulativeOffset(
    slang::VariableLayoutReflection* variableLayout,
    slang::ParameterCategory layoutUnit,
    AccessPath accessPath)
{
    CumulativeOffset cumulativeOffset =
        CalculateCumulativeOffset(variableLayout, layoutUnit, accessPath);

    PrintOffset(layoutUnit, cumulativeOffset.value, cumulativeOffset.space);
}

CumulativeOffset ShaderReflectionHelper::CalculateCumulativeOffset(
    slang::ParameterCategory layoutUnit,
    AccessPath accessPath)
{
    CumulativeOffset result;
    switch (layoutUnit)
    {
        // #### Layout Units That Don't Require Special Handling
        //
    default:
        for (auto node = accessPath.leaf; node != nullptr; node = node->outer)
        {
            result.value += node->variableLayout->getOffset(layoutUnit);
        }
        break;

        // #### Bytes
        //
    case slang::ParameterCategory::Uniform:
        for (auto node = accessPath.leaf; node != accessPath.deepestConstantBufer;
            node = node->outer)
        {
            result.value += node->variableLayout->getOffset(layoutUnit);
        }
        break;

        // #### Layout Units That Care About Spaces
        //
    case slang::ParameterCategory::ConstantBuffer:
    case slang::ParameterCategory::ShaderResource:
    case slang::ParameterCategory::UnorderedAccess:
    case slang::ParameterCategory::SamplerState:
    case slang::ParameterCategory::DescriptorTableSlot:
        for (auto node = accessPath.leaf; node != accessPath.deepestParameterBlock;
            node = node->outer)
        {
            result.value += node->variableLayout->getOffset(layoutUnit);
            result.space += node->variableLayout->getBindingSpace(layoutUnit);
        }
        for (auto node = accessPath.deepestParameterBlock; node != nullptr; node = node->outer)
        {
            result.space += node->variableLayout->getOffset(
                slang::ParameterCategory::SubElementRegisterSpace);
        }
        break;
    }
    return result;
}

// Determining Whether Parameters Are Used
// ---------------------------------------

Slang::Result ShaderReflectionHelper::CollectEntryPointMetadata(
    slang::IComponentType* program,
    int targetIndex,
    int entryPointCount)
{
    metadataForEntryPoints.resize(entryPointCount);
    for (int entryPointIndex = 0; entryPointIndex < entryPointCount; entryPointIndex++)
    {
        Slang::ComPtr<slang::IMetadata> entryPointMetadata;
        Slang::ComPtr<slang::IBlob> diagnostics;
        SLANG_RETURN_ON_FAIL(program->getEntryPointMetadata(
            entryPointIndex,
            targetIndex,
            entryPointMetadata.writeRef(),
            diagnostics.writeRef()));
        diagnoseIfNeeded(diagnostics);

        metadataForEntryPoints[entryPointIndex] = entryPointMetadata;
    }
    return SLANG_OK;
}

StageMask ShaderReflectionHelper::CalculateParameterStageMask(
    slang::ParameterCategory layoutUnit,
    CumulativeOffset offset)
{
    unsigned mask = 0;
    auto entryPointCount = metadataForEntryPoints.size();
    for (int i = 0; i < entryPointCount; ++i)
    {
        bool isUsed = false;
        metadataForEntryPoints[i]->isParameterLocationUsed(
            SlangParameterCategory(layoutUnit),
            offset.space,
            offset.value,
            isUsed);
        if (isUsed)
        {
            auto entryPointStage = programLayout->getEntryPointByIndex(i)->getStage();

            mask |= 1 << unsigned(entryPointStage);
        }
    }
    return mask;
}

StageMask ShaderReflectionHelper::CalculateStageMask(
    slang::VariableLayoutReflection* variableLayout,
    AccessPath accessPath)
{
    StageMask mask = 0;

    int usedLayoutUnitCount = variableLayout->getCategoryCount();
    for (int i = 0; i < usedLayoutUnitCount; ++i)
    {
        auto layoutUnit = variableLayout->getCategoryByIndex(i);
        auto offset = CalculateCumulativeOffset(variableLayout, layoutUnit, accessPath);

        mask |= CalculateParameterStageMask(layoutUnit, offset);
    }

    return mask;
}

void ShaderReflectionHelper::PrintStageUsage(slang::VariableLayoutReflection* variableLayout, AccessPath accessPath)
{
    StageMask stageMask = CalculateStageMask(variableLayout, accessPath);

    Key("used by stages");
    WITH_ARRAY()
        for (int i = 0; i < SLANG_STAGE_COUNT; i++)
        {
            if (stageMask & (1 << i))
            {
                Element();
                PrintStage(SlangStage(i));
            }
        }
    afterArrayElement = false;
}

void ShaderReflectionHelper::PrintOffsets(slang::VariableLayoutReflection* variableLayout, AccessPath accessPath)
{
    Key("offset");
    {
        SCOPED_OBJECT();
        PrintRelativeOffsets(variableLayout);

        if (accessPath.valid)
        {
            PrintCumulativeOffsets(variableLayout, accessPath);
        }
    }


    if (accessPath.valid)
    {
        PrintStageUsage(variableLayout, accessPath);
    }
}

void ShaderReflectionHelper::PrintIndentation()
{
    for (int i = 1; i < indentation; ++i)
    {
        printf("  ");
    }
}

void ShaderReflectionHelper::BeginObject() { indentation++; }

void ShaderReflectionHelper::EndObject() { indentation--; }

void ShaderReflectionHelper::BeginArray() { indentation++; }

void ShaderReflectionHelper::EndArray() { indentation--; }

void ShaderReflectionHelper::NewLine()
{
    printf("\n");
    PrintIndentation();
}

void ShaderReflectionHelper::Element()
{
    NewLine();
    printf("- ");
    afterArrayElement = true;
}

void ShaderReflectionHelper::Key(char const* Key)
{
    if (!afterArrayElement)
    {
        NewLine();
    }
    afterArrayElement = false;

    printf("%s: ", Key);
}

void ShaderReflectionHelper::PrintQuotedString(char const* text)
{
    if (text)
    {
        printf("\"%s\"", text);
    }
    else
    {
        printf("null");
    }
}

void ShaderReflectionHelper::PrintBool(bool value) { printf(value ? "true" : "false"); }

void ShaderReflectionHelper::Print(size_t value) { printf("%u", unsigned(value)); }

void ShaderReflectionHelper::PrintComment(char const* text) { printf("# %s", text); }

void ShaderReflectionHelper::PrintTypeKind(slang::TypeReflection::Kind kind)
{
    switch (kind)
    {
#define CASE(TAG)                          \
    case slang::TypeReflection::Kind::TAG: \
        printf("%s", #TAG);                \
        break

        CASE(None);
        CASE(Struct);
        CASE(Array);
        CASE(Matrix);
        CASE(Vector);
        CASE(Scalar);
        CASE(ConstantBuffer);
        CASE(Resource);
        CASE(SamplerState);
        CASE(TextureBuffer);
        CASE(ShaderStorageBuffer);
        CASE(ParameterBlock);
        CASE(GenericTypeParameter);
        CASE(Interface);
        CASE(OutputStream);
        CASE(Specialized);
        CASE(Feedback);
        CASE(Pointer);
        CASE(DynamicResource);
        CASE(MeshOutput);
#undef CASE

    default:
        printf("%d # unexpected enumerant", int(kind));
        break;
    }
}

void ShaderReflectionHelper::PrintResourceShape(SlangResourceShape shape)
{
    SCOPED_OBJECT();

    Key("base");
    auto baseShape = shape & SLANG_RESOURCE_BASE_SHAPE_MASK;
    switch (baseShape)
    {
#define CASE(TAG)           \
    case SLANG_##TAG:       \
        printf("%s", #TAG); \
        break

        CASE(TEXTURE_1D);
        CASE(TEXTURE_2D);
        CASE(TEXTURE_3D);
        CASE(TEXTURE_CUBE);
        CASE(TEXTURE_BUFFER);
        CASE(STRUCTURED_BUFFER);
        CASE(BYTE_ADDRESS_BUFFER);
        CASE(RESOURCE_UNKNOWN);
        CASE(ACCELERATION_STRUCTURE);
        CASE(TEXTURE_SUBPASS);
#undef CASE

    default:
        printf("%d # unexpected enumerant", int(baseShape));
        break;
    }

#define CASE(TAG)                               \
    do                                          \
    {                                           \
        if (shape & SLANG_TEXTURE_##TAG##_FLAG) \
        {                                       \
            Key(#TAG);                          \
            printf("true");                     \
        }                                       \
    } while (0)

    CASE(FEEDBACK);
    CASE(SHADOW);
    CASE(ARRAY);
    CASE(MULTISAMPLE);
#undef CASE
}

void ShaderReflectionHelper::PrintResourceAccess(SlangResourceAccess access)
{
    switch (access)
    {
#define CASE(TAG)                     \
    case SLANG_RESOURCE_ACCESS_##TAG: \
        printf("%s", #TAG);           \
        break

        CASE(NONE);
        CASE(READ);
        CASE(READ_WRITE);
        CASE(RASTER_ORDERED);
        CASE(APPEND);
        CASE(CONSUME);
        CASE(WRITE);
        CASE(FEEDBACK);
#undef CASE

    default:
        printf("%d # unexpected enumerant", int(access));
        break;
    }
}

void ShaderReflectionHelper::PrintLayoutUnit(slang::ParameterCategory layoutUnit)
{
    switch (layoutUnit)
    {
#define CASE(TAG, DESCRIPTION)                \
    case slang::ParameterCategory::TAG:       \
        printf("%s # %s", #TAG, DESCRIPTION); \
        break

        CASE(ConstantBuffer, "constant buffer slots");
        CASE(ShaderResource, "texture slots");
        CASE(UnorderedAccess, "uav slots");
        CASE(VaryingInput, "varying input slots");
        CASE(VaryingOutput, "varying output slots");
        CASE(SamplerState, "sampler slots");
        CASE(Uniform, "bytes");
        CASE(DescriptorTableSlot, "bindings");
        CASE(SpecializationConstant, "specialization constant ids");
        CASE(PushConstantBuffer, "push-constant buffers");
        CASE(RegisterSpace, "register space offset for a variable");
        CASE(GenericResource, "generic resources");
        CASE(RayPayload, "ray payloads");
        CASE(HitAttributes, "hit attributes");
        CASE(CallablePayload, "callable payloads");
        CASE(ShaderRecord, "shader records");
        CASE(ExistentialTypeParam, "existential type parameters");
        CASE(ExistentialObjectParam, "existential object parameters");
        CASE(SubElementRegisterSpace, "register spaces / descriptor sets");
        CASE(InputAttachmentIndex, "subpass input attachments");
        CASE(MetalArgumentBufferElement, "Metal argument buffer elements");
        CASE(MetalAttribute, "Metal attributes");
        CASE(MetalPayload, "Metal payloads");
#undef CASE

    default:
        printf("%d # unknown enumerant", int(layoutUnit));
        break;
    }
}

void ShaderReflectionHelper::PrintStage(SlangStage stage)
{
    switch (stage)
    {
#define CASE(NAME)           \
    case SLANG_STAGE_##NAME: \
        printf(#NAME);       \
        break

        CASE(NONE);
        CASE(VERTEX);
        CASE(HULL);
        CASE(DOMAIN);
        CASE(GEOMETRY);
        CASE(FRAGMENT);
        CASE(COMPUTE);
        CASE(RAY_GENERATION);
        CASE(INTERSECTION);
        CASE(ANY_HIT);
        CASE(CLOSEST_HIT);
        CASE(MISS);
        CASE(CALLABLE);
        CASE(MESH);
        CASE(AMPLIFICATION);
#undef CASE

    default:
        printf("%d # unexpected enumerant", int(stage));
        break;
    };
}
void ShaderReflectionHelper::PrintTargetFormat(SlangCompileTarget targetFormat)
{
    switch (targetFormat)
    {
#define CASE(TAG)           \
    case SLANG_##TAG:       \
        printf("%s", #TAG); \
        break

        CASE(TARGET_UNKNOWN);
        CASE(TARGET_NONE);
        CASE(GLSL);
        CASE(GLSL_VULKAN_DEPRECATED);
        CASE(GLSL_VULKAN_ONE_DESC_DEPRECATED);
        CASE(HLSL);
        CASE(SPIRV);
        CASE(SPIRV_ASM);
        CASE(DXBC);
        CASE(DXBC_ASM);
        CASE(DXIL);
        CASE(DXIL_ASM);
        CASE(C_SOURCE);
        CASE(CPP_SOURCE);
        CASE(HOST_EXECUTABLE);
        CASE(SHADER_SHARED_LIBRARY);
        CASE(SHADER_HOST_CALLABLE);
        CASE(CUDA_SOURCE);
        CASE(PTX);
        CASE(CUDA_OBJECT_CODE);
        CASE(OBJECT_CODE);
        CASE(HOST_CPP_SOURCE);
        CASE(HOST_HOST_CALLABLE);
        CASE(CPP_PYTORCH_BINDING);
        CASE(METAL);
        CASE(METAL_LIB);
        CASE(METAL_LIB_ASM);
        CASE(HOST_SHARED_LIBRARY);
        CASE(WGSL);
        CASE(WGSL_SPIRV_ASM);
        CASE(WGSL_SPIRV);
#undef CASE

    default:
        printf("%d # unhandled enumerant", int(targetFormat));
    }
}

void ShaderReflectionHelper::PrintScalarType(slang::TypeReflection::ScalarType scalarType)
{
    switch (scalarType)
    {
#define CASE(TAG)                    \
    case slang::TypeReflection::TAG: \
        printf("%s", #TAG);          \
        break

        CASE(None);
        CASE(Void);
        CASE(Bool);
        CASE(Int32);
        CASE(UInt32);
        CASE(Int64);
        CASE(UInt64);
        CASE(Float16);
        CASE(Float32);
        CASE(Float64);
        CASE(Int8);
        CASE(UInt8);
        CASE(Int16);
        CASE(UInt16);
#undef CASE

    default:
        printf("%d # unhandled enumerant", int(scalarType));
    }
}

void ShaderReflectionHelper::PrintMatrixLayoutMode(SlangMatrixLayoutMode mode)
{
    switch (mode)
    {
#define CASE(TAG)                   \
    case SLANG_MATRIX_LAYOUT_##TAG: \
        printf("%s", #TAG);         \
        break

        CASE(MODE_UNKNOWN);
        CASE(ROW_MAJOR);
        CASE(COLUMN_MAJOR);
#undef CASE

    default:
        printf("%d # unhandled enumerant", int(mode));
    }
}