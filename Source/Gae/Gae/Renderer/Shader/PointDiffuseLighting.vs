//	Vertex Shader for Point Lighting + Diffuse mapping.

float4x4	m44fViewProj;	// View x Proj Matrix
float3		g_f3Light;		// Light pos.
float3		g_f3Radius;		// Light radius. Lighting area.

struct VS_INPUT
{
	float4 f4Pos : POSITION;	// Vertex position
	float3 f3Normal : NORMAL;	// Vertex Normal
	float2 f2UV : TEXCOORD0;	// UV
	float3 f3Tangent : TANGENT0;	// Tangent
	float3 f3Binormal : BINORMAL0;	// Binormal
};

struct VS_OUTPUT
{
	float4 f4Pos : POSITION;	// Vertex position in screen-space
	float2 f2UV : TEXCOORD0;	// UV
	float4 f4ScaledLightVertexDirection : TEXCOORD1;	// Light dir scaled by the lighting ellipsoid volume
};

VS_OUTPUT	main(const VS_INPUT vtxIn)
{
	VS_OUTPUT vout = (VS_OUTPUT)0;
	
	// Transform to screen-space
	vout.f4Pos = mul(vtxIn.f4Pos, m44fViewProj);
	
	//vout.f4LightDir.xyz = float3(g_f3Light - vtxIn.f4Pos.xyz);
	vout.f4ScaledLightVertexDirection.xyz = (g_f3Light - vtxIn.f4Pos.xyz) * (1.0f / g_f3Radius);	// Correct attenuation based in arbitrary sphere scale.
	
	vout.f2UV = vtxIn.f2UV;
	
	return vout;
}