#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct GeometryInput
{
    float3 pos : POSITION0;
    float2 size : SIZE;
    matrix invView : INVVIEW;
    matrix view : VIEW;
    matrix projection : PROJECTION;
    float diatance : DISTANCE;
    float4 color : COLOR;
    float3 velocity : VELOCITY;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float4 color : COLOR;
};

cbuffer RainBuffer : register(b10)
{
    float3 velocity;
    float drawDistance;
	
    float4 color;
	
    float3 origin;
    float time;
	
    float3 size;
    float tubulence;
}

GeometryInput VS(VertexUV input)
{
    GeometryInput output;
	
    float3 v = velocity;
    v.xz /= input.uv.y * tubulence;
    output.velocity = v;
	
    float3 displace = time * v;
    input.pos.xyz = origin + (size + (input.pos.xyz + displace) % size) % size - (size * 0.5f);
	
    float alpha = cos(time + (input.pos.x + input.pos.z));
    alpha = saturate(1.5f + alpha / drawDistance * 2.0f);
	
    output.diatance = drawDistance;
    output.color = color;
    output.color.a = alpha;
	
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

[maxvertexcount(4)]
void GS(point GeometryInput input[1], inout TriangleStream<PixelInput> output)
{
    float3 up = normalize(-input[0].velocity);
    float3 forward = normalize(input[0].pos.xyz - input[0].invView._41_42_43);
    float3 right = normalize(cross(up, forward));
	
    float2 halfSize = input[0].size * 0.5f;
	
    float4 vertices[4];
    vertices[0] = float4(input[0].pos.xyz - right * halfSize.x - up * halfSize.y, 1.0f);
    vertices[1] = float4(input[0].pos.xyz - right * halfSize.x + up * halfSize.y, 1.0f);
    vertices[2] = float4(input[0].pos.xyz + right * halfSize.x - up * halfSize.y, 1.0f);
    vertices[3] = float4(input[0].pos.xyz + right * halfSize.x + up * halfSize.y, 1.0f);
	
    PixelInput element;
    element.color = input[0].color;
	
	[unroll(4)]
    for (uint i = 0; i < 4; i++)
    {
        element.pos = mul(vertices[i], input[0].view);
        element.pos = mul(element.pos, input[0].projection);
		
        element.uv = TEXCOORD[i];
		
        element.color.a = 0.2f * saturate(1 - element.pos.z / input[0].diatance)
			* input[0].color.a;
		
        output.Append(element);
    }
}

float4 PS(PixelInput input) : SV_TARGET
{
    return diffuseMap.Sample(samp, input.uv) * input.color;
}
