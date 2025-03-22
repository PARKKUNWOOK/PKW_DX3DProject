#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)// : SV_POSITION//SV->RS
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

cbuffer BrushBuffer : register(b10)
{
    int type;
    float3 pickingPos;
	
    float range;
    float3 color;
}

float4 BrushColor(float3 pos)
{
    float2 direction = pos.xz - pickingPos.xz;
	
    float dist = length(direction);
	
    if (dist < range)
        return float4(color, 0);

    return float4(0, 0, 0, 0);
}

Texture2D secondMap : register(t11);
Texture2D thirdMap : register(t12);

float4 PS(LightPixelInput input) : SV_TARGET //->SV->OM
{
    Material material = GetMaterial(input);
    
    float4 second = secondMap.Sample(samp, input.uv);
    float4 third = thirdMap.Sample(samp, input.uv);
    
    material.baseColor = lerp(material.baseColor, second, input.tangent.x);
    material.baseColor = lerp(material.baseColor, third, input.tangent.y);
    
    float4 ambient = CalcAmbient(material);
    
    float4 result = 0;
    
    [unroll(MAX_LIGHT)]
    for (int i = 0; i < lightCount; i++)
    {
        if (!lights[i].isActive)
            continue;

        if (lights[i].type == 0)
            result += CalcDirectional(material, lights[i]);
        else if (lights[i].type == 1)
            result += CalcPoint(material, lights[i]);
        else if (lights[i].type == 2)
            result += CalcSpot(material, lights[i]);
    }
    
    return result + ambient + mEmissive + BrushColor(material.worldPos);
}