


float4 psmain(MeshOutput input) : SV_TARGET
{
    return float4(input.Color, 1);
}