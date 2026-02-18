#include "ShaderReflectionHelper.h"

#include <fmt/core.h>

using namespace fmt;

void ShaderReflectionHelper::PrintQuotedString(const char* _string)
{
    print("{}{}{}", '"', _string, '"');
}

void ShaderReflectionHelper::PrintVariable(
    slang::VariableReflection* variable)
{
    currentIndent++;
    const char* name = variable->getName();
    slang::TypeReflection* type = variable->getType();

    printWithDepth("name: ");    PrintQuotedString(name);
    printWithDepth("type: ");    PrintType(type);
    currentIndent--;
}

void ShaderReflectionHelper::PrintType(slang::TypeReflection* type)
{
    currentIndent++;
    const char* name = type->getName();
    slang::TypeReflection::Kind kind = type->getKind();

    printWithDepth("name: ");    PrintQuotedString(name);
    printWithDepth("kind: ");    PrintTypeKind(kind);

    // ...

    switch (type->getKind())
    {
    case slang::TypeReflection::Kind::Scalar:
    {
        printWithDepth("scalar type: ");
        PrintScalarType(type->getScalarType());
        break;
    }
    case slang::TypeReflection::Kind::Struct:
    {
        printWithDepth("fields:");
        int fieldCount = type->getFieldCount();
        for (int f = 0; f < fieldCount; f++)
        {
            printWithDepth("- ");
            slang::VariableReflection* field =
                type->getFieldByIndex(f);
            PrintVariable(field);
        }
        break;
    }
    case slang::TypeReflection::Kind::Array:
    {
        printWithDepth("element count: ");
        PrintPossiblyUnbounded(type->getElementCount());

        printWithDepth("element type: ");
        PrintType(type->getElementType());
        break;
    }
    case slang::TypeReflection::Kind::Vector:
    {
        printWithDepth("element count: ");
        PrintCount(type->getElementCount());

        printWithDepth("element type: ");
        PrintType(type->getElementType());
        break;
    }
    case slang::TypeReflection::Kind::Matrix:
    {
        printWithDepth("row count: ");
        PrintCount(type->getRowCount());

        printWithDepth("column count: ");
        PrintCount(type->getColumnCount());

        printWithDepth("element type: ");
        PrintType(type->getElementType());
        break;
    }
    case slang::TypeReflection::Kind::Resource:
    {
        printWithDepth("shape");
        PrintResourceShape(type->getResourceShape());

        printWithDepth("access");
        PrintResourceAccess(type->getResourceAccess());

        printWithDepth("result type");
        PrintType(type->getResourceResultType());
        break;
    }
    case slang::TypeReflection::Kind::ConstantBuffer:
    case slang::TypeReflection::Kind::ParameterBlock:
    case slang::TypeReflection::Kind::TextureBuffer:
    case slang::TypeReflection::Kind::ShaderStorageBuffer:
    {
        printWithDepth("element type");
        PrintType(type->getElementType());
        break;
    }
    default:
        break;
    }
    currentIndent--;
}

void ShaderReflectionHelper::PrintPossiblyUnbounded(size_t value)
{
    if (value == ~size_t(0))
    {
        print("unbounded");
    }
    else
    {
        print("{}", unsigned(value));
    }
}

void ShaderReflectionHelper::PrintResourceShape(SlangResourceShape shape)
{
    printWithDepth("base shape:");
    switch (shape & SLANG_RESOURCE_BASE_SHAPE_MASK)
    {
    case SLANG_TEXTURE_1D: printWithDepth("TEXTURE1D"); break;
    case SLANG_TEXTURE_2D: printWithDepth("TEXTURE2D"); break;
        // ...
    }

    if (shape & SLANG_TEXTURE_ARRAY_FLAG) printWithDepth("ARRAY");
    if (shape & SLANG_TEXTURE_MULTISAMPLE_FLAG) printWithDepth("MULTISAMPLE");
    // ...
}

void ShaderReflectionHelper::PrintVarLayout(slang::VariableLayoutReflection* varLayout, AccessPath accessPath)
{
    printWithDepth("Name: "); 
    PrintQuotedString(varLayout->getName());
    EndLine();

    PrintRelativeOffsets(varLayout);

    printWithDepth("Type Layout: ");
    EndLine();

    PrintTypeLayout(varLayout->getTypeLayout());
    EndLine();

    if (varLayout->getStage() != SLANG_STAGE_NONE)
    {
        print("semantic: ");
        print("name: ");  PrintQuotedString(varLayout->getSemanticName());
        print("index: "); print(varLayout->getSemanticIndex());
    }

    ExtendedAccessPath varAccessPath(accessPath, varLayout);

    print("type layout: ");
    PrintTypeLayout(varLayout->getTypeLayout(), varAccessPath);
}

void ShaderReflectionHelper::PrintOffset(
    slang::VariableLayoutReflection* varLayout,
    slang::ParameterCategory layoutUnit)
{
    currentIndent++;

    size_t offset = varLayout->getOffset(layoutUnit);

    printWithDepth("- Value: ");
    print("{}", offset);
    EndLine();

    printWithDepth("  Unit: ");
    PrintLayoutUnit(layoutUnit);
    EndLine();

    size_t spaceOffset = varLayout->getBindingSpace(layoutUnit);

    switch (layoutUnit)
    {
    case slang::ParameterCategory::ConstantBuffer:
    case slang::ParameterCategory::ShaderResource:
    case slang::ParameterCategory::UnorderedAccess:
    case slang::ParameterCategory::SamplerState:
    case slang::ParameterCategory::DescriptorTableSlot:
        printWithDepth("  Space: ");
        print("{}", spaceOffset);
        EndLine();
    default:
        break;
    }

    currentIndent--;
}

void printOffsets(
    slang::VariableLayoutReflection* varLayout,
    AccessPath accessPath)
{
    // ...

    print("cumulative:");
    for (int i = 0; i < usedLayoutUnitCount; ++i)
    {
        print("- ");
        auto layoutUnit = varLayout->getCategoryByIndex(i);
        printCumulativeOffset(varLayout, layoutUnit, accessPath);
    }
}


void ShaderReflectionHelper::PrintRelativeOffsets(
    slang::VariableLayoutReflection* varLayout)
{
    currentIndent++;

    printWithDepth("Relative Offset: ");
    EndLine();

    int usedLayoutUnitCount = varLayout->getCategoryCount();
    for (int i = 0; i < usedLayoutUnitCount; ++i)
    {
        auto layoutUnit = varLayout->getCategoryByIndex(i);
        PrintOffset(varLayout, layoutUnit);
        EndLine();
    }

    EndLine();

    currentIndent--;
}

void ShaderReflectionHelper::PrintTypeLayout(slang::TypeLayoutReflection* typeLayout, AccessPath accessPath)
{
    currentIndent++;
    printWithDepth("Name: "); 
    PrintQuotedString(typeLayout->getName()); 
    EndLine();

    printWithDepth("Kind: "); 
    PrintTypeKind(typeLayout->getKind()); 
    EndLine();

    PrintSizes(typeLayout);

    // ...

    if (typeLayout->getSize() != 0)
    {
        printWithDepth("alignment in bytes: ");
        println("{}", typeLayout->getAlignment());
        EndLine();

        printWithDepth("stride in bytes: ");
        println("{}", typeLayout->getStride());
        EndLine();
    }

    // ...

    switch (typeLayout->getKind())
    {
    case slang::TypeReflection::Kind::Struct:
    {
        printWithDepth("fields: ");

        int fieldCount = typeLayout->getFieldCount();
        for (int f = 0; f < fieldCount; f++)
        {
            auto field = typeLayout->getFieldByIndex(f);
            PrintVarLayout(field);
        }
    break;
    }
    case slang::TypeReflection::Kind::Array:
    {
        printWithDepth("element count: ");
        PrintPossiblyUnbounded(typeLayout->getElementCount());

        printWithDepth("element type layout: ");
        PrintTypeLayout(typeLayout->getElementTypeLayout());
    break;
    }
    case slang::TypeReflection::Kind::Matrix:
    {
        // ...

        printWithDepth("matrix layout mode: TODO");
        //PrintMatrixLayoutMode(typeLayout->getMatrixLayoutMode());
    break;
    }
    case slang::TypeReflection::Kind::ConstantBuffer:
    case slang::TypeReflection::Kind::ParameterBlock:
    case slang::TypeReflection::Kind::TextureBuffer:
    case slang::TypeReflection::Kind::ShaderStorageBuffer:
    {
       

        printWithDepth("Container: ");
        EndLine();
        printWithDepth("Offset: ");
        EndLine();
        PrintRelativeOffsets(typeLayout->getContainerVarLayout());

        AccumulatedOffsets innerAccessPath = accessPath;
        innerAccessPath.deepestConstantBuffer = innerAccessPath.leaf;

        if (typeLayout->getContainerVarLayout()->getTypeLayout()->getSize(
            slang::ParameterCategory::SubElementRegisterSpace) != 0)
        {
            innerAccessPath.deepestParameterBlock = innerAccessPath.leaf;
        }

        auto elementVarLayout = typeLayout->getElementVarLayout();
        printWithDepth("Element: ");
        EndLine();
        PrintRelativeOffsets(elementVarLayout);

        ExtendedAccessPath elementAccessPath(innerAccessPath, elementVarLayout);

        printWithDepth("Type Layout: ");
        EndLine();
        PrintTypeLayout(elementVarLayout->getTypeLayout(), elementAccessPath);

        

    break;
    }
    default:
        break;
    }
    currentIndent--;
}

void ShaderReflectionHelper::PrintSize(
    slang::TypeLayoutReflection* typeLayout,
    slang::ParameterCategory layoutUnit)
{
    currentIndent++;
    size_t size = typeLayout->getSize(layoutUnit);

    printWithDepth("- Value: ");
    PrintPossiblyUnbounded(size);
    EndLine();

    printWithDepth("  Unit: ");
    PrintLayoutUnit(layoutUnit);
    EndLine();

    currentIndent--;
}

void ShaderReflectionHelper::PrintSizes(slang::TypeLayoutReflection* typeLayout)
{
    currentIndent++;

    printWithDepth("Size: ");
    EndLine();
    int usedLayoutUnitCount = typeLayout->getCategoryCount();
    for (int i = 0; i < usedLayoutUnitCount; ++i)
    {
        auto layoutUnit = typeLayout->getCategoryByIndex(i);
        PrintSize(typeLayout, layoutUnit);
    }

    currentIndent--;
}

void ShaderReflectionHelper::PrintScalarType(slang::TypeReflection::ScalarType _type)
{
    switch (_type)
    {
    case(slang::TypeReflection::ScalarType::None):      { print("None");    break; }
    case(slang::TypeReflection::ScalarType::Void):      { print("Void");    break; }
    case(slang::TypeReflection::ScalarType::Bool):      { print("Bool");    break; }
    case(slang::TypeReflection::ScalarType::Int32):     { print("Int32");   break; }
    case(slang::TypeReflection::ScalarType::UInt32):    { print("UInt32");  break; }
    case(slang::TypeReflection::ScalarType::Int64):     { print("Int64");   break; }
    case(slang::TypeReflection::ScalarType::UInt64):    { print("UInt64");  break; }
    case(slang::TypeReflection::ScalarType::Float16):   { print("Float16"); break; }
    case(slang::TypeReflection::ScalarType::Float32):   { print("Float32"); break; }
    case(slang::TypeReflection::ScalarType::Float64):   { print("Float64"); break; }
    case(slang::TypeReflection::ScalarType::Int8):      { print("Int8");    break; }
    case(slang::TypeReflection::ScalarType::UInt8):     { print("UInt8");   break; }
    case(slang::TypeReflection::ScalarType::Int16):     { print("Int16");   break; }
    case(slang::TypeReflection::ScalarType::UInt16):    { print("UInt16");  break; }
    default:
        break;
    }
}

void ShaderReflectionHelper::PrintTypeKind(slang::TypeReflection::Kind _kind)
{
    switch(_kind)
    {
    case(slang::TypeReflection::Kind::None):                 { print("None");                   break; }
    case(slang::TypeReflection::Kind::Struct):               { print("Struct");                 break; }
    case(slang::TypeReflection::Kind::Array):                { print("Array");                  break; }
    case(slang::TypeReflection::Kind::Matrix):               { print("Matrix");                 break; }
    case(slang::TypeReflection::Kind::Vector):               { print("Vector");                 break; }
    case(slang::TypeReflection::Kind::Scalar):               { print("Scalar");                 break; }
    case(slang::TypeReflection::Kind::ConstantBuffer):       { print("ConstantBuffer");         break; }
    case(slang::TypeReflection::Kind::Resource):             { print("Resource");               break; }
    case(slang::TypeReflection::Kind::SamplerState):         { print("SamplerState");           break; }
    case(slang::TypeReflection::Kind::TextureBuffer):        { print("TextureBuffer");          break; }
    case(slang::TypeReflection::Kind::ShaderStorageBuffer):  { print("ShaderStorageBuffer");    break; }
    case(slang::TypeReflection::Kind::ParameterBlock):       { print("ParameterBlock");         break; }
    case(slang::TypeReflection::Kind::GenericTypeParameter): { print("GenereicTypeParameter");  break; }
    case(slang::TypeReflection::Kind::Interface):            { print("Interface");              break; }
    case(slang::TypeReflection::Kind::OutputStream):         { print("OutputStream");           break; }
    case(slang::TypeReflection::Kind::Specialized):          { print("Specialized");            break; }
    case(slang::TypeReflection::Kind::Feedback):             { print("Feedback");               break; }
    case(slang::TypeReflection::Kind::Pointer):              { print("Pointer");                break; }
    case(slang::TypeReflection::Kind::DynamicResource):      { print("DynamicResource");        break; }
    case(slang::TypeReflection::Kind::MeshOutput):           { print("MeshOutput");             break; }
    case(slang::TypeReflection::Kind::Enum):                 { print("Enum");                   break; }
    }
}

void ShaderReflectionHelper::PrintCount(size_t _count)
{
    println("{}", _count);
}

void ShaderReflectionHelper::PrintResourceAccess(SlangResourceAccess _access)
{
    switch (_access)
    {
    case(SLANG_RESOURCE_ACCESS_NONE):           { print("SLANG_RESOURCE_ACCESS_NONE");              break; }
    case(SLANG_RESOURCE_ACCESS_READ):           { print("SLANG_RESOURCE_ACCESS_READ");              break; }
    case(SLANG_RESOURCE_ACCESS_READ_WRITE):     { print("SLANG_RESOURCE_ACCESS_READ_WRITE");        break; }
    case(SLANG_RESOURCE_ACCESS_RASTER_ORDERED): { print("SLANG_RESOURCE_ACCESS_RASTER_ORDERED");    break; }
    case(SLANG_RESOURCE_ACCESS_APPEND):         { print("SLANG_RESOURCE_ACCESS_APPEND");            break; }
    case(SLANG_RESOURCE_ACCESS_CONSUME):        { print("SLANG_RESOURCE_ACCESS_CONSUME");           break; }
    case(SLANG_RESOURCE_ACCESS_WRITE):          { print("SLANG_RESOURCE_ACCESS_WRITE");             break; }
    case(SLANG_RESOURCE_ACCESS_FEEDBACK):       { print("SLANG_RESOURCE_ACCESS_FEEDBACK");          break; }
    case(SLANG_RESOURCE_ACCESS_UNKNOWN):        { print("SLANG_RESOURCE_ACCESS_UNKNOWN");           break; }
    }
    
}

void ShaderReflectionHelper::PrintLayoutUnit(slang::ParameterCategory _unit)
{
    switch (_unit)
    {
    case(slang::ParameterCategory::None):                           { print("None");                        break; }
    case(slang::ParameterCategory::Mixed):                          { print("Mixed");                       break; }
    case(slang::ParameterCategory::ConstantBuffer):                 { print("ConstantBuffer");              break; }
    case(slang::ParameterCategory::ShaderResource):                 { print("ShaderResource");              break; }
    case(slang::ParameterCategory::UnorderedAccess):                { print("UnorderedAccess");             break; }
    case(slang::ParameterCategory::VaryingInput):                   { print("VaryingInput");                break; }
    case(slang::ParameterCategory::VaryingOutput):                  { print("VaryingOutput");               break; }
    case(slang::ParameterCategory::SamplerState):                   { print("SamplerState");                break; }
    case(slang::ParameterCategory::Uniform):                        { print("Uniform");                     break; }
    case(slang::ParameterCategory::DescriptorTableSlot):            { print("DescriptorTableSlot");         break; }
    case(slang::ParameterCategory::SpecializationConstant):         { print("SpecializationConstant");      break; }
    case(slang::ParameterCategory::PushConstantBuffer):             { print("PushConstantBuffer");          break; }
    case(slang::ParameterCategory::RegisterSpace):                  { print("RegisterSpace");               break; }
    case(slang::ParameterCategory::GenericResource):                { print("GenericResource");             break; }
    case(slang::ParameterCategory::RayPayload):                     { print("RayPayload");                  break; }
    case(slang::ParameterCategory::HitAttributes):                  { print("HitAttributes");               break; }
    case(slang::ParameterCategory::CallablePayload):                { print("CallablePayload");             break; }
    case(slang::ParameterCategory::ShaderRecord):                   { print("ShaderRecord");                break; }
    case(slang::ParameterCategory::ExistentialTypeParam):           { print("ExistentialTypeParam");        break; }
    case(slang::ParameterCategory::ExistentialObjectParam):         { print("ExistentialObjectParam");      break; }
    case(slang::ParameterCategory::SubElementRegisterSpace):        { print("SubElementRegisterSpace");     break; }
    case(slang::ParameterCategory::InputAttachmentIndex):           { print("InputAttachmentIndex");        break; }
    //case(slang::ParameterCategory::MetalBuffer):                    { printWithDepth("MetalBuffer");                 break; }
    //case(slang::ParameterCategory::MetalTexture):                   { printWithDepth("MetalTexture");                break; }
    //case(slang::ParameterCategory::MetalArgumentBufferElement):     { printWithDepth("MetalArgumentBufferElement");  break; }
    //case(slang::ParameterCategory::MetalAttribute):                 { printWithDepth("MetalAttribute");              break; }
    //case(slang::ParameterCategory::MetalPayload):                   { printWithDepth("MetalPayload");                break; }
    //case(slang::ParameterCategory::VertexInput):                    { printWithDepth("VertexInput");                 break; }
    //case(slang::ParameterCategory::FragmentOutput):                 { printWithDepth("FragmentOutput");              break; }
    default:
        break;
    }
    
}

void ShaderReflectionHelper::printWithDepth(const char* _string)
{
    if (currentIndent != lastIndent)
    {
        indentString.clear();
        for (int i = 0; i < currentIndent; i++)
        {
            indentString.append("    ");
        }

        lastIndent = currentIndent;
    }

    print("{}{}", indentString, _string);
}

void ShaderReflectionHelper::EndLine()
{
    print("\n");
}

void ShaderReflectionHelper::PrintProgramLayout(
    slang::ProgramLayout* programLayout)
{
    print("global scope: ");
    PrintScope(programLayout->getGlobalParamsVarLayout());

    print("entry points: ");
    int entryPointCount = programLayout->getEntryPointCount();
    for (int i = 0; i < entryPointCount; ++i)
    {
        print("- ");
        PrintEntryPointLayout(
            programLayout->getEntryPointByIndex(i));
    }
}

void ShaderReflectionHelper::PrintScope(
    slang::VariableLayoutReflection* scopeVarLayout, 
    AccessPath accessPath)
{
    AccessPathNode scopeAccessPath = {};  (accessPath, scopeVarLayout);
    scopeAccessPath.outer = 

    auto scopeTypeLayout = scopeVarLayout->getTypeLayout();
    switch (scopeTypeLayout->getKind())
    {
    case slang::TypeReflection::Kind::Struct:
    {
        print("Parameters: ");

        int paramCount = scopeTypeLayout->getFieldCount();
        for (int i = 0; i < paramCount; i++)
        {
            print("- ");

            auto param = scopeTypeLayout->getFieldByIndex(i);
            PrintVarLayout(param, &scopeOffsets);
        }
        break;
    }
    case slang::TypeReflection::Kind::ConstantBuffer:
    {
        print("automatically-introduced constant buffer: ");

        printOffsets(scopeTypeLayout->getContainerVarLayout());

        PrintScope(scopeTypeLayout->getElementVarLayout());
        break;
    }
    case slang::TypeReflection::Kind::ParameterBlock:
    {
        print("automatically-introduced parameter block: ");

        printOffsets(scopeTypeLayout->getContainerVarLayout());

        PrintScope(scopeTypeLayout->getElementVarLayout());
        break;
    }
    }
}

void ShaderReflectionHelper::PrintEntryPointLayout(slang::EntryPointReflection* entryPointLayout)
{
    print("stage: ");
    PrintStage(entryPointLayout->getStage());

    // ...
    PrintScope(entryPointLayout->getVarLayout());

    auto resultVarLayout = entryPointLayout->getResultVarLayout();
    if (resultVarLayout->getTypeLayout()->getKind() != slang::TypeReflection::Kind::None)
    {
        print("result");
        PrintVarLayout(resultVarLayout);
    }

    switch (entryPointLayout->getStage())
    {
    case SLANG_STAGE_COMPUTE:
    {
        SlangUInt sizes[3];
        entryPointLayout->getComputeThreadGroupSize(3, sizes);

        print("thread group size: ");
        print("x: "); print("{}", sizes[0]);
        print("y: "); print("{}", sizes[1]);
        print("z: "); print("{}", sizes[2]);
    }
    break;


    default:
        break;
    }
}

CumulativeOffset ShaderReflectionHelper::CalculateCumulativeOffset(slang::ParameterCategory layoutUnit, AccessPath accessPath)
{
    CumulativeOffset result = {};

    switch (layoutUnit)
    {
    default:
        for (auto node = accessPath.leaf; node != nullptr; node = node->outer)
        {
            result.offset += node->varLayout->getOffset(layoutUnit);
        }
        break;
    case slang::ParameterCategory::Uniform:
        for (auto node = accessPath.leaf; node != accessPath.deepestConstantBuffer; node = node->outer)
        {
            result.offset += node->varLayout->getOffset(layoutUnit);
        }
        break;
    case slang::ParameterCategory::ConstantBuffer:
    case slang::ParameterCategory::ShaderResource:
    case slang::ParameterCategory::UnorderedAccess:
    case slang::ParameterCategory::SamplerState:
    case slang::ParameterCategory::DescriptorTableSlot:
        for (auto node = accessPath.leaf; node != accessPath.deepestParameterBlock; node = node->outer)
        {
            result.offset += node->varLayout->getOffset(layoutUnit);
            result.space += node->varLayout->getBindingSpace(layoutUnit);
        }
        for (auto node = accessPath.deepestParameterBlock; node != nullptr; node = node->outer)
        {
            result.space += node->varLayout->getOffset(slang::ParameterCategory::SubElementRegisterSpace);
        }


        break;


    }

    return result;

}

void ShaderReflectionHelper::PrintCumulativeOffset(
    slang::VariableLayoutReflection* varLayout,
    slang::ParameterCategory layoutUnit,
    AccessPath accessPath)
{
    CumulativeOffset cumulativeOffset = CalculateCumulativeOffset(layoutUnit, accessPath);

    cumulativeOffset.offset += varLayout->getOffset(layoutUnit);
    cumulativeOffset.space += varLayout->getBindingSpace(layoutUnit);

    PrintOffset(layoutUnit, cumulativeOffset.offset, cumulativeOffset.space);
}
