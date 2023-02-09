//	Vertex Shader for Parallel Lighting. 
float4x4	m44fViewProj;	// View x Proj Matrix

struct VS_INPUT
{
	float4 f4Pos : POSITION;	// Vertex position
	float2 f2UV : TEXCOORD0;	// UV
};

struct VS_OUTPUT
{
	float4 f4Pos : POSITION;	// Vertex position in screen-space
	float2 f2UV : TEXCOORD0;	// UV
};

VS_OUTPUT	main(const VS_INPUT vtxIn)
{
	VS_OUTPUT vout = (VS_OUTPUT)0;
	
	// Transform to screen-space
	vout.f4Pos = mul(vtxIn.f4Pos, m44fViewProj);
	
	vout.f2UV = vtxIn.f2UV;
	return vout;
}
