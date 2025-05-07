#include "FullscreenShader.hlsli"

Texture2D<float4> gTexture1 : register(t0);
Texture2D<float4> gTexture2 : register(t1);
Texture2D<float4> gTexture3 : register(t2);
Texture2D<float4> gTexture4 : register(t3);

SamplerState gSampler : register(s0);

float4 main(VertexShaderOutput input) : SV_TARGET {
	float4 output = float4(0, 0, 0, 0);
	output += gTexture1.Sample(gSampler, input.texcoord);
	output += gTexture2.Sample(gSampler, input.texcoord);
	output += gTexture3.Sample(gSampler, input.texcoord);
	output += gTexture4.Sample(gSampler, input.texcoord);

	output /= 4;
	output.a = 1.0f;
	
	return output;
}