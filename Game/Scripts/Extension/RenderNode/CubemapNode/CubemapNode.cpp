#include "CubemapNode.h"

#include <Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h>
#include <Engine/Assets/Texture/TextureLibrary.h>
#include <Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h>
#include <Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h>
#include <Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h>
#include <Engine/GraphicsAPI/RenderingSystemValues.h>
#include <Engine/Module/World/Camera/CameraInstance.h>

void CubemapNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("CubemapNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	indexBuffer = szg::PrimitiveGeometryLibrary::GetPrimitiveGeometry("Cubemap");
}

void CubemapNode::preprocess() {
	if (!indexBuffer) {
		return;
	}
	materialBuffer.data_mut()->texture = cubemapTexture->index();
	auto& command = szg::DxCommand::GetCommandList();
	command->IASetIndexBuffer(indexBuffer->get_p_ibv());
	command->SetGraphicsRootConstantBufferView(0, vsBuffer.get_resource()->GetGPUVirtualAddress());
	command->SetGraphicsRootConstantBufferView(1, materialBuffer.get_resource()->GetGPUVirtualAddress());
	//camera->register_world_projection(2);
	command->DrawIndexedInstanced(indexBuffer->index_size(), 1, 0, 0, 0);
}

void CubemapNode::set_cubemap_texture(const std::string& name) {
	cubemapTexture = szg::TextureLibrary::GetTexture(name);
}

void CubemapNode::set_camera(Reference<const szg::CameraInstance> camera_) {
	camera = camera_;
}

void CubemapNode::write_position(const Vector3& position) {
	*vsBuffer.data_mut() = position;
}

void CubemapNode::create_pipeline_state() {
	szg::RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 0 : world
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 1 : material
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 1); // 2 : camera
	rootSignatureBuilder.sampler(D3D12_SHADER_VISIBILITY_PIXEL, 0, 0);

	std::unique_ptr<szg::PSOBuilder> psoBuilder = std::make_unique<szg::PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->depth_state(
		szg::RenderingSystemValues::GetDepthStencilTexture()->get_as_dsv()->get_format(),
		D3D12_DEPTH_WRITE_MASK_ZERO,
		D3D12_COMPARISON_FUNC_LESS_EQUAL
	);
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(szg::ShaderType::Vertex, "Skybox.VS.hlsl");
	psoBuilder->shaders(szg::ShaderType::Pixel, "Skybox.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<szg::DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
