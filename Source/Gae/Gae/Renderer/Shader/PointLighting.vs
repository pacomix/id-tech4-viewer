//	Vertex Shader for Point Lighting.

float4x4	m44fViewProj;	// View x Proj Matrix
float4x4	m44fViewProjInvTrans;	// View x Proj Matrix
float3		g_f3Light;		// Light pos

struct VS_INPUT
{
	float4 f4Pos : POSITION;	// Vertex position
	float3 f3Normal : NORMAL;	// Vertex Normal
	float2 f2UV : TEXCOORD0;	// UV
};

struct VS_OUTPUT
{
	float4 f4Pos : POSITION;	// Vertex position in screen-space
	float2 f2UV : TEXCOORD0;	// UV
	float4 f4PixelPos : TEXCOORD1;	// Untransformed Pixel position
	float3 f3Normal : TEXCOORD2;	// Normal in screen-space
};

VS_OUTPUT	main(const VS_INPUT vtxIn)
{
	VS_OUTPUT vout = (VS_OUTPUT)0;
	
	// Transform to screen-space
	vout.f4Pos = mul(vtxIn.f4Pos, m44fViewProj);
	vout.f3Normal = vtxIn.f3Normal;
	
	// Just pass the value to be linearly interpolated.
	vout.f4PixelPos = vtxIn.f4Pos;
	vout.f2UV = vtxIn.f2UV;
	return vout;
}