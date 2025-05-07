#include "BloomNode.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"


void BloomNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("BloomNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	bloomInfo.get_data()->weight = 0.3f;
}

void BloomNode::draw() const {
	baseTexture->start_read();
	blurTexture->start_read();

	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootConstantBufferView(0, bloomInfo.get_resource()->GetGPUVirtualAddress());
	baseTexture->get_as_srv()->use(1);
	blurTexture->get_as_srv()->use(2);
	command->DrawInstanced(3, 1, 0, 0);
}

void BloomNode::set_base_texture(Reference<RenderTexture> baseTexture_) {
	baseTexture = baseTexture_;
}

void BloomNode::set_blur_texture(Reference<RenderTexture> blurTexture_) {
	blurTexture = blurTexture_;
}

void BloomNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL, 1);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0, 0,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	ShaderBuilder shaderManager;
	shaderManager.initialize(
		"DirectXGame/EngineResources/HLSL/FullscreenShader.VS.hlsl",
		"Game/Resources/HLSL/Bloom.PS.hlsl"
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(shaderManager);
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}

#ifdef _DEBUG
#include <imgui.h>
void BloomNode::debug_gui() {
	ImGui::DragFloat("Weight", &bloomInfo.get_data()->weight, 0.001f, 0.0f, 1.0f, "%.4f");
}
#endif // _DEBUG
