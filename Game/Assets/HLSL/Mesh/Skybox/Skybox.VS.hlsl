#include "Skybox.hlsli"

static const float4 VertexData[] = {
// 右面。描画インデックスは [0,1,2] [2,1,3]で内側を向く 
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f, 1.0f },
	{ 1.0f, -1.0f, -1.0f, 1.0f },
// 左面。描画インデックスは [4,5,6] [6,5,7] 
	{ -1.0f, 1.0f, -1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f, 1.0f },
// 前面。描画インデックスは [8,9,10] [10,9,11] 
	{ -1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f, 1.0f },
// 背面 [12,13,14] [14,13,15] 
	{ -1.0f, -1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, -1.0f, 1.0f },
	{ -1.0f, 1.0f, -1.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f, 1.0f },
// 上面 [16,17,18] [18,17,19] 
	{ -1.0f, 1.0f, -1.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
// 底面 [20,21,22] [22,21,23] 
	{ -1.0f, -1.0f, 1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, -1.0f, 1.0f }
};

struct World {
	float3 world;
};

struct CameraInfomation {
	float4x4 viewProjection;
	float4x4 view;
};

ConstantBuffer<World> gWorld : register(b0);
ConstantBuffer<CameraInfomation> gCameraMatrix : register(b1);

VertexShaderOutput main(uint vertexId : SV_VertexID) {
	VertexShaderOutput output;
	
	float4 vertex = VertexData[vertexId];
	
	float4 world = vertex + float4(gWorld.world, 0.0f);

	float4x4 vp = gCameraMatrix.viewProjection;

	output.position = mul(world, vp).xyww;
	output.texcoord = vertex.xyz;
	return output;
}
