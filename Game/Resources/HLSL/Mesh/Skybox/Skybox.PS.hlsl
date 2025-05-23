#include "Skybox.hlsli"

struct Material {
	float3 color;
	uint textureIndex;
};

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
