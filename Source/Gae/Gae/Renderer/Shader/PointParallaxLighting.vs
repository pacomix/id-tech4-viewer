//	Vertex Shader for Point Lighting + Parallax Mapping.

float4x4	m44fViewProj;	// View x Proj Matrix
float3		g_f3Light;		// Light dir
float3		g_f3ViewDir;	// View dir

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
	float4 f4PixelPos : TEXCOORD1;	// Untransformed Pixel position
	float4 f4LightDir : TEXCOORD2;	// Light dir in texture space
	float3 f3ViewDir : TEXCOORD3;	// View vector in texture space
};

VS_OUTPUT	main(const VS_INPUT vtxIn)
{
	VS_OUTPUT vout = (VS_OUTPUT)0;
	
	// Transform to screen-space
	vout.f4Pos = mul(vtxIn.f4Pos, m44fViewProj);
	
	// Transform light->vertex direction to texture space...
	float3x3 m33fTextureMatrix = transpose(float3x3(vtxIn.f3Tangent, vtxIn.f3Binormal, vtxIn.f3Normal));
	
	// ...storing the attenuation factor in the w component.
	vout.f4LightDir = float4(mul((g_f3Light - vtxIn.f4Pos.xyz), m33fTextureMatrix), saturate(1.0f - distance(g_f3Light, vtxIn.f4Pos.xyz) / 675.0f));
	
	// Transform the view vector into texture space.
	//vout.f3ViewDir = mul(mul(g_f3ViewDir, m44fViewProj), m33fTextureMatrix);	// Transform the view vector to texture space.
	vout.f3ViewDir = mul(g_f3ViewDir, m33fTextureMatrix);	// Transform the view vector to texture space.
	
	// Just pass the value to be linearly interpolated.
	vout.f4PixelPos = vtxIn.f4Pos;
	vout.f2UV = vtxIn.f2UV;
	
	return vout;
}