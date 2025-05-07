#include "FullscreenShader.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

float4 main(VertexShaderOutput input) : SV_TARGET {
	float4 output;
	output = gTexture.Sample(gSampler, input.texcoord);
	
	return output;
}
