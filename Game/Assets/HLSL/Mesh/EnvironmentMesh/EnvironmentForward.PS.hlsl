#include "Forward/Forward.hlsli"

#include "Tools/Lighing/DirectionalLighting.hlsli"

struct Material {
	float3 color;
	uint lightingType;
	float shininess;
	uint textureIndex;
	float3x3 uvTransform;
};

struct Camera {
	float3 position;
};

StructuredBuffer<Material> gMaterial : register(t0, space0);
ConstantBuffer<Camera> gCamera : register(b0, space1);
StructuredBuffer<DirectionalLightBuffer> gDirectionalLights : register(t0, space2);
SamplerState gSampler : register(s0);

TextureCube<float4> gEnvTexture : register(t0, space3);

float4 main(VertexShaderOutput input) : SV_Target0 {
	float4 output = float4(0, 0, 0, 0);
	
	Material material = gMaterial[input.instance];
	const Texture2D<float4> texture = ResourceDescriptorHeap[material.textureIndex];
	
	float3 transformedUV = mul(float3(input.texcoord, 1.0f), material.uvTransform);
	float4 textureColor = texture.Sample(gSampler, transformedUV.xy);
	
	float alpha = 1.0f;
	
	Pixel pixel;
	pixel.color = textureColor.rgb * material.color.rgb;
	pixel.normal = input.normal;
	pixel.shininess = material.shininess;
	pixel.world = input.world;
	
	uint numStructs;
	uint stride;

	gDirectionalLights.GetDimensions(numStructs, stride);
	for (uint i = 0; i < numStructs; i++) {
		LightingData lightingData = CalcLightingData(pixel, gCamera.position, gDirectionalLights[i]);
		float3 cameraToPixel = -lightingData.surface.toCamera;
		float3 reflectedVector = reflect(cameraToPixel, normalize(lightingData.surface.normal));
		float4 envColor = gEnvTexture.Sample(gSampler, reflectedVector);
		output.rgb = BlinnPhongSpecular(lightingData) + HalfLambertDiffuse(lightingData) + envColor.rgb * 0.5f;
		output.a = 1.0f;
	}
	
	// 透明の場合は出力せず終了
	// サンプリングによっては0にならない場合があるためバッファを取る
	if (output.a <= 0.05f) {
		discard;
	}
	
	return output;
}