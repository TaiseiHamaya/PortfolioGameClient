#include "FullscreenShader.hlsli"

struct ExtractionInfo {
	float luminanceIntensity;
};

ConstantBuffer<ExtractionInfo> gExtractionInfo : register(b0);
Texture2D<float4> gTexture : register(t0);

static const float3 LuminanceCoefficient = float3(0.2125f, 0.7154f, 0.0721f);

float4 main(VertexShaderOutput input) : SV_TARGET0 {
	float4 textureColor = gTexture.Load(int3(input.position.xy, 0));
	float luminance = dot(textureColor.xyz, LuminanceCoefficient);
	
	clip(luminance - gExtractionInfo.luminanceIntensity);
	
	return textureColor;
}