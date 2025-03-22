#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct GeometryInput
{
    float4 pos : POSITION0;
    float2 size : SIZE;
    float3 viewPos : POSITION1;
    matrix view : VIEW;
    matrix projection : PROJECTION;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

GeometryInput VS(VertexUV input)
{
    GeometryInput output;
	
    output.pos = input.pos;
    output.size = input.uv;
    output.viewPos = invView._41_42_43;
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

[maxvertexcount(4)]
void GS(point GeometryInput input[1], inout TriangleStream<PixelInput> output)
{
    float3 up = float3(0, 1, 0);
    float3 forward = input[0].pos.xyz - input[0].viewPos;
    forward.y = 0;
    forward = normalize(forward);
	
    float3 right = normalize(cross(up, forward));
    float2 halfSize = input[0].size * 0.5f;
	
    float4 vertices[4];
    vertices[0] = float4(input[0].pos.xyz - right * halfSize.x - up * halfSize.y, 1.0f);
    vertices[1] = float4(input[0].pos.xyz - right * halfSize.x + up * halfSize.y, 1.0f);
    vertices[2] = float4(input[0].pos.xyz + right * halfSize.x - up * halfSize.y, 1.0f);
    vertices[3] = float4(input[0].pos.xyz + right * halfSize.x + up * halfSize.y, 1.0f);
	
    PixelInput element;
	
	[unroll(4)]
    for (uint i = 0; i < 4; i++)
    {
        element.pos = mul(vertices[i], input[0].view);
        element.pos = mul(element.pos, input[0].projection);
		
        element.uv = TEXCOORD[i];
		
        output.Append(element);
    }
}

float4 PS(PixelInput input) : SV_TARGET
{
    return diffuseMap.Sample(samp, input.uv) * mDiffuse;
}