#include "CubemapNode.h"

#include <Engine/Assets/PrimitiveGeometry/PrimitiveGeometryLibrary.h>
#include <Engine/Assets/Texture/TextureLibrary.h>
#include <Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h>
#include <Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h>
#include <Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h>
#include <Engine/GraphicsAPI/RenderingSystemValues.h>
#include <Engine/Module/World/Camera/Camera3D.h>

void CubemapNode::initialize() {
	depthStencil = RenderingSystemValues::GetDepthStencilTexture();
	create_pipeline_state();
	pipelineState->set_name("CubemapNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	indexBuffer = PrimitiveGeometryLibrary::GetPrimitiveGeometry("Cubemap");
}

void CubemapNode::preprocess() {
	materialBuffer.get_data()->texture = cubemapTexture->index();
	auto& command = DxCommand::GetCommandList();
	command->IASetIndexBuffer(indexBuffer->get_p_ibv());
	command->SetGraphicsRootConstantBufferView(0, vsBuffer.get_resource()->GetGPUVirtualAddress());
	command->SetGraphicsRootConstantBufferView(1, materialBuffer.get_resource()->GetGPUVirtualAddress());
	camera->register_world_projection(2);
	command->DrawIndexedInstanced(indexBuffer->index_size(), 1, 0, 0, 0);
}

void CubemapNode::set_cubemap_texture(const std::string& name) {
	cubemapTexture = TextureLibrary::GetTexture(name);
}

void CubemapNode::set_camera(Reference<const Camera3D> camera_) {
	camera = camera_;
}

void CubemapNode::write_position(const Vector3& position) {
	*vsBuffer.get_data() = position;
}

void CubemapNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 0); // 0 : world
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0); // 1 : material
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_VERTEX, 1); // 2 : camera
	rootSignatureBuilder.sampler(D3D12_SHADER_VISIBILITY_PIXEL, 0, 0);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->depth_state(
		depthStencil->get_as_dsv()->get_format(),
		D3D12_DEPTH_WRITE_MASK_ZERO,
		D3D12_COMPARISON_FUNC_LESS_EQUAL
	);
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "Skybox.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "Skybox.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}
