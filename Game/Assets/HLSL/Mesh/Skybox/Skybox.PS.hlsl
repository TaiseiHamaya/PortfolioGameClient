#include "Skybox.hlsli"

/// <summary>
/// スカイボックス用マテリアル
/// 色とテクスチャインデックスを保持
/// </summary>
struct Material {
	float3 color;
	uint textureIndex;
};

/// <summary>
/// Skyboxピクセルシェーダー出力
/// 最終的な色を出力
/// </summary>
struct PixelShaderOutput {
	float4 color : SV_Target0;
};

ConstantBuffer<Material> gMaterial : register(b0, space0);
SamplerState gSampler : register(s0);

[earlydepthstencil]
PixelShaderOutput main(VertexShaderOutput input) {
	Material material = gMaterial;
	
	PixelShaderOutput output;
	// texture color
	const TextureCube<float4> texture = ResourceDescriptorHeap[material.textureIndex];
	
	float4 textureColor = texture.Sample(gSampler, input.texcoord);
		
	// 出力
	output.color.xyz = textureColor.rgb * material.color;
	output.color.a = 1.0f;
	return output;
}
