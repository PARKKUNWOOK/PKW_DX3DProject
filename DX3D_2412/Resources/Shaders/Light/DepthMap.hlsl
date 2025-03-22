#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
    LightPixelInput output;
    output.pos = mul(input.pos, world);
    output.worldPos = output.pos;
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    output.viewPos = invView._41_42_43;
	
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    float depth = input.pos.z / input.pos.w;
	
    return float4(depth.xxx, 1);
}