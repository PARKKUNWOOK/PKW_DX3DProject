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

float4 PS(PixelInput input) : SV_TARGET //->SV->OM
{
    float4 baseColor = diffuseMap.Sample(samp, input.uv) * mDiffuse;
    
    //float scale = (baseColor.r + baseColor.g + baseColor.b) / 3;
    //float3 grayColor;
    //grayColor.r = baseColor.r * 0.3f;
    //grayColor.g = baseColor.g * 0.59f;
    //grayColor.b = baseColor.b * 0.11f;
    //
    //float scale = grayColor.r + grayColor.g + grayColor.b;
    float scale = dot(baseColor.rgb, float3(0.3f, 0.59f, 0.11f));
    
    return float4(scale.xxx, baseColor.a);
}