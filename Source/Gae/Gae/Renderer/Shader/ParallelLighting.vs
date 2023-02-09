//	Vertex Shader for Parallel Lighting. 
float4x4	m44fViewProj;	// View x Proj Matrix
float3	g_f3Light;	// Light dir

struct VS_INPUT
{
	float4 f4Pos : POSITION;	// Vertex position
	float4 f4Color : COLOR;	// Vertex position
	float3 f3Normal : NORMAL;	// Vertex Normal
	float2 f2UV : TEXCOORD0;	// UV
};

struct VS_OUTPUT
{
	float4 f4Pos : POSITION;	// Vertex position in screen-space
	float4 f4Color : COLOR;	// Vertex position in screen-space
	float2 f2UV : TEXCOORD0;	// UV
	float3 f3LightDir : TEXCOORD1;	// Light direction in screen-space
	float3 f3Normal : TEXCOORD2;	// Normal in screen-space
};

VS_OUTPUT	main(const VS_INPUT vtxIn)
{
	VS_OUTPUT vout = (VS_OUTPUT)0;
	
	// Transform to screen-space
	vout.f3LightDir = normalize(mul(g_f3Light, m44fViewProj));
	vout.f4Pos = mul(vtxIn.f4Pos, m44fViewProj);
	vout.f3Normal = mul(vtxIn.f3Normal, m44fViewProj);
	vout.f4Color = vtxIn.f4Color;
	
	vout.f2UV = vtxIn.f2UV;
	return vout;
}