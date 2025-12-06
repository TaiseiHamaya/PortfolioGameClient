#include "EnvironmentMeshNode.h"

#include <Engine/Assets/Shader/ShaderLibrary.h>
#include <Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h>
#include <Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h>
#include <Engine/GraphicsAPI/RenderingSystemValues.h>

EnvironmentMeshNode::EnvironmentMeshNode() = default;
EnvironmentMeshNode::~EnvironmentMeshNode() noexcept = default;

void EnvironmentMeshNode::BeginLoadShader() {
	szg::ShaderLibrary::RegisterLoadQue("[[szg]]/Forward/Mesh/StaticMeshForward.VS.hlsl");
	szg::ShaderLibrary::RegisterLoadQue("Mesh/EnvironmentMesh/EnvironmentForward.PS.hlsl");
}

void EnvironmentMeshNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("EnvironmentMeshNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void EnvironmentMeshNode::create_pipeline_state() {
	szg::RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_VERTEX, 0, 1, 0); // 0 : transform(S0T0, V)
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1, 0); // 1 : material(S0T0, P)
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0, 1); // 2 : camera vs(S1B0, V)
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1); // 3 : camera ps(S1B0, P)
	rootSignatureBuilder.add_structured(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1, 2); // 4 : light(S1B0, P)
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0, 1, 3); // 5 : environment texture(S3T0, P)
	rootSignatureBuilder.sampler( // sampler
		D3D12_SHADER_VISIBILITY_PIXEL,
		0, 0,
		D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);

	szg::InputLayoutBuilder inputLayoutBuilder;
	inputLayoutBuilder.add_element("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutBuilder.add_element("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT);
	inputLayoutBuilder.add_element("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT);

	std::unique_ptr<szg::PSOBuilder> psoBuilder = std::make_unique<szg::PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->depth_state(szg::RenderingSystemValues::GetDepthStencilTexture()->get_as_dsv()->get_format());
	psoBuilder->inputlayout(inputLayoutBuilder.build());
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(szg::ShaderType::Vertex, "StaticMeshForward.VS.hlsl");
	psoBuilder->shaders(szg::ShaderType::Pixel, "EnvironmentForward.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<szg::DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
