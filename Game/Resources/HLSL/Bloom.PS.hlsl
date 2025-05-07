struct VertexShaderOutput {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

struct BloomInfo {
	float weight;
};

ConstantBuffer<BloomInfo> gBloomInfo : register(b0);
Texture2D<float4> gTexture : register(t0);
Texture2D<float4> gBlured : register(t1);
SamplerState gSampler : register(s0);

float4 main(VertexShaderOutput input) : SV_TARGET0 {
	float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
	float4 blurColor = gBlured.Sample(gSampler, input.texcoord);
	return textureColor + blurColor * gBloomInfo.weight;
}