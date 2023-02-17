//--------------------------------------------------------------------------------------
// vsstage.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.hlsl"
//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.WPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = Tex;

	float4 N = Normal;
	N.w = 0.0f;					// 法線はベクトルなのでＷの値を０にする。

	N = mul(N, World);
	N = normalize(N);

	output.Normal = N;
    
	return output;
}