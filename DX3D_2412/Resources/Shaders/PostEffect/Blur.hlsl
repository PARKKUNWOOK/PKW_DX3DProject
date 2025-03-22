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
    float2 imageSize;
}

static const float2 edges[8] =
{
    float2(-1, -1), float2(0, -1), float2(1, -1),
	float2(-1, 0), float2(1, 0),
	float2(-1, 1), float2(0, 1), float2(1, 1),
};

float4 PS(PixelInput input) : SV_TARGET //->SV->OM
{
    float4 result = 0;
	
    for (int i = 0; i < scale; i++)
    {
        float2 div = (1 + i) / imageSize;

        for (int j = 0; j < 8; j++)
        {
            float2 uv = edges[j] * div + input.uv;
            result += diffuseMap.Sample(samp, uv);
        }
    }
	
    result /= scale * 8;
	
    return result * mDiffuse;
}