//	Vertex Shader for Point Lighting + Normal mapping.

float4x4	m44fViewProj;	// View x Proj Matrix
float3		g_f3LightPos;		// Light pos.
float3		g_f3Radius;		// Light radius. Lighting area. 3Axis independent to allow ellipsoid attenuation.
float3		g_f3ViewPos;	// View position in world.

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
	float3 f3ScaledLightVertexDirection : TEXCOORD1;	// Light dir scaled by the lighting ellipsoid volume
	float3 f3LightVtxDir : TEXCOORD2;	// Light dir in texture space
	float3 f3ViewVtxDir : TEXCOORD3;	// ViewVtx dir in texture space
	float3 f3Normal : TEXCOORD4;	// Triangle's Normal in texture space
};

VS_OUTPUT	main(const VS_INPUT vtxIn)
{
	VS_OUTPUT vout = (VS_OUTPUT)0;
	
	// Transform input vertex to screen-space
	vout.f4Pos = mul(vtxIn.f4Pos, m44fViewProj);
	
	// Transform light->vertex direction to texture space.
	float3x3 m33fTextureMatrix = transpose(float3x3(vtxIn.f3Tangent, vtxIn.f3Binormal, vtxIn.f3Normal));
	float3 f3LightVertex = g_f3LightPos - vtxIn.f4Pos.xyz;
	vout.f3LightVtxDir = mul(f3LightVertex, m33fTextureMatrix);
	vout.f3ScaledLightVertexDirection = f3LightVertex * (1.0f / g_f3Radius);	// Correct attenuation based in arbitrary sphere scale.
	vout.f3ViewVtxDir = mul(g_f3ViewPos - vtxIn.f4Pos.xyz, m33fTextureMatrix);	// Transform the view vector to texture space.
	vout.f3Normal = mul(vtxIn.f3Normal, m33fTextureMatrix);
	
	vout.f2UV = vtxIn.f2UV;
	
	return vout;
}