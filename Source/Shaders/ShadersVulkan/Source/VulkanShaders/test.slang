// hello-world.slang

RWTexture2D image;

[shader("compute")]
[numthreads(16,16,1)]
void main(uint3 threadId : SV_DispatchThreadID)
{
    uint2 texelCoord = threadId.xy;
    float2 size = float2(0.0, 0.0);
    image.GetDimensions(size.x, size.y);

    if (texelCoord.x < size.x && texelCoord.y < size.y)
    {
        float4 color = float4(0.0, 0.0, 0.0, 1.0);

        if (threadId.x != 0 && threadId.y != 0)
        {
            color.x = float(texelCoord.x) / (size.x);
            color.y = float(texelCoord.y) / (size.y);
        }

        image[texelCoord] = color;
    }
}
