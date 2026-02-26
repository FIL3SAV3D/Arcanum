struct Meshlet
{
    uint vertex_offset;
    uint triangle_offset;
    uint vertex_count;
    uint triangle_count;
};

StructuredBuffer<Meshlet> meshlets;
StructuredBuffer<uint> meshlet_vertices;
StructuredBuffer<uint8_t> meshlet_triangles;

struct PerVertexData
{
    float4 position;
}

struct PerPrimitveData
{
    uint index0;
    uint index1;
    uint index2;
}

OutputVertices<PerVertexData, 64> vertices;
OutputPrimitives<PerPrimitveData, 126> primitive;

[Shader("mesh")]
[require(meshshading)]
[numthreads(32)]
void main(uint3 threadId: SV_GroupThreadID, uint LocalInvocationIndex: SV_GroupIndex) {
    Meshlet meshlet = meshlets[threadId.x];
    SetMeshOutputCounts(meshlet.vertex_count, meshlet.triangle_count);

    for (uint i = LocalInvocationIndex; i < meshlet.vertex_count; i += gl_WorkGroupSize.x) {
        uint index = meshlet_vertices[meshlet.vertex_offset + i];
        vertices[i] = world_view_projection * vec4(vertex_positions[index], 1);
        gl_MeshVerticesEXT[i].gl_Position = world_view_projection * vec4(vertex_positions[index], 1);
    }

    for (uint i = LocalInvocationIndex; i < meshlet.triangle_count; i += gl_WorkGroupSize.x) {
        uint offset = meshlet.triangle_offset + i * 3;
        gl_PrimitiveTriangleIndicesEXT[i] = uvec3(
            meshlet_triangles[offset], meshlet_triangles[offset + 1], meshlet_triangles[offset + 2]);
    }
}