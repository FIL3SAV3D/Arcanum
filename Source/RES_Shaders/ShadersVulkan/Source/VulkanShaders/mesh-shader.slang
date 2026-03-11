static const uint THREADGROUP_SIZE_X = 16;
static const uint THREADGROUP_SIZE_Y = THREADGROUP_SIZE_X;

struct PerVertexData
{
    float3 position;
}

struct PerIndexData
{
    uint index0;
    uint index1;
    uint index2;
}

static const float3 verticesPosition[3] = {float3(-1,-1,0), float3(0,1,0), float3(1,-1,0)};

uniform PerVertexData RandomAssName;

OutputVertices<PerVertexData, 3> vertices;
OutputIndices<PerIndexData, 1> indices;

[Shader("mesh")]
[require(meshshading)]
[numthreads(THREADGROUP_SIZE_X, THREADGROUP_SIZE_Y)]
void main(uint3 threadId: SV_DispatchThreadID)
{
    SetMeshOutputCounts(3, 1);

    PerVertexData data;

    data.position = verticesPosition[0];
    OutputVertices<PerVertexData, 3>._setVertex(vertices, 0, data);
    data.position = verticesPosition[1];
    OutputVertices<PerVertexData, 3>._setVertex(vertices, 1, data);
    data.position = verticesPosition[2];
    OutputVertices<PerVertexData, 3>._setVertex(vertices, 2, data);
}

[shader("fragment")]
float4 fragmentMain()
{
    return float4(1.0f);
}