#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct GeometryInput
{
    float4 pos : POSITION0;
    float2 size : SIZE;
    matrix invView : INVVIEW;
    matrix view : VIEW;
    matrix projection : PROJECTION;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float2 maxFrame : MAX_FRAME;
    float2 curFrame : CUR_FRAME;
};

GeometryInput VS(VertexUV input)
{
    GeometryInput output;
	
    output.pos = input.pos;
    output.size = input.uv;
    output.invView = invView;
    output.view = view;
    output.projection = projection;
	
    return output;
}

static const float2 TEXCOORD[4] =
{
    float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)
};

cbuffer SpriteBuffer : register(b10)
{
    float2 maxFrame;
    float2 curFrame;
}

[maxvertexcount(4)]
void GS(point GeometryInput input[1], inout TriangleStream<PixelInput> output)
{
    float3 up = input[0].invView._21_22_23;
    float3 forward = normalize(input[0].pos.xyz - input[0].invView._41_42_43);
    float3 right = normalize(cross(up, forward));
    float2 halfSize = input[0].size * 0.5f;
	
    float4 vertices[4];
    vertices[0] = float4(input[0].pos.xyz - right * halfSize.x - up * halfSize.y, 1.0f);
    vertices[1] = float4(input[0].pos.xyz - right * halfSize.x + up * halfSize.y, 1.0f);
    vertices[2] = float4(input[0].pos.xyz + right * halfSize.x - up * halfSize.y, 1.0f);
    vertices[3] = float4(input[0].pos.xyz + right * halfSize.x + up * halfSize.y, 1.0f);
	
    PixelInput element;
    element.maxFrame = maxFrame;
    element.curFrame = curFrame;    
	
	[unroll(4)]
    for (uint i = 0; i < 4; i++)
    {
        element.pos = mul(vertices[i], input[0].view);
        element.pos = mul(element.pos, input[0].projection);
		
        element.uv = TEXCOORD[i] * (float2(1, 1) / maxFrame);
		
        output.Append(element);
    }
}

float4 PS(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv + (input.curFrame / input.maxFrame);    
	
    return diffuseMap.Sample(samp, uv) * mDiffuse;
}