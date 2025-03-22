#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexUV input)// : SV_POSITION//SV->RS
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.uv = input.uv;
    
    return output;
}

cbuffer ValueBuffer : register(b10)
{
    float scale;
}

float4 PS(PixelInput input) : SV_TARGET //->SV->OM
{
    float2 uv = floor(input.uv * scale) / scale;
	
    return diffuseMap.Sample(samp, uv);
}