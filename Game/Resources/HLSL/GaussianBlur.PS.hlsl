#include "FullscreenShader.hlsli"

struct BlurInfo {
	float dispersion;
	float length;
	uint sampleCount;
};

ConstantBuffer<BlurInfo> gBlurInfo : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

float Gaussian(float distance, float dispersion) {
	return exp(-pow(distance, (2 * dispersion * dispersion)));
}

static const float2 UVTexelSize = float2(1.0f / 1280.0f, 1.0f / 720.0f);

float4 main(VertexShaderOutput input) : SV_TARGET0 {
	float4 outputColor = float4(0, 0, 0, 0);
	
	float baseStep = gBlurInfo.length / gBlurInfo.sampleCount;
	float2 uvStepSize = UVTexelSize * baseStep;
	
	float2 topLeft = uvStepSize * -(gBlurInfo.sampleCount * 0.5f);
	float2 beginTexel = input.texcoord + topLeft;
	
	float waight[16][16];
	float totalWaight = 0;
	int x = 0;
	
	// ウェイト算出
	for (x = 0; x < gBlurInfo.sampleCount; ++x) {
		for (int y = 0; y < gBlurInfo.sampleCount; ++y) {
			float distance = length(topLeft + float2(x, y) * uvStepSize);
			waight[x][y] = Gaussian(distance, gBlurInfo.dispersion);
			totalWaight += waight[x][y];
		}
	}
	
	// 正規化
	for (x = 0; x < gBlurInfo.sampleCount; ++x) {
		for (int y = 0; y < gBlurInfo.sampleCount; ++y) {
			float distance = dot(UVTexelSize, float2(x, y));
			waight[x][y] = Gaussian(distance, gBlurInfo.dispersion);
			waight[x][y] /= totalWaight;
		}
	}
	
	// ブラーかける
	for (x = 0; x < gBlurInfo.sampleCount; ++x) {
		for (int y = 0; y < gBlurInfo.sampleCount; ++y) {
			float2 sampleTexel = beginTexel + float2(x, y) * uvStepSize;
			outputColor += gTexture.Sample(gSampler, sampleTexel) * waight[x][y];
		}
	}
	
	outputColor.a = 1.0f;
	return outputColor;
}