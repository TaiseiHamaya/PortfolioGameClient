#include "LuminanceExtractionNode.h"

#include "Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h"
#include "Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h"

void LuminanceExtractionNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("LuminanceExtractionNode");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	luminanceExtractionInfo.get_data()->intensity = 0.8f;
}

void LuminanceExtractionNode::draw() const {
	baseTexture->start_read();

	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootConstantBufferView(0, luminanceExtractionInfo.get_resource()->GetGPUVirtualAddress());
	baseTexture->get_as_srv()->use(1);
	command->DrawInstanced(3, 1, 0, 0);

}

void LuminanceExtractionNode::set_texture_resource(Reference<RenderTexture> baseTexture_) {
	baseTexture = baseTexture_;
}

void LuminanceExtractionNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "FullscreenShader.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "LuminanceExtraction.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}

#ifdef _DEBUG
#include <imgui.h>
void LuminanceExtractionNode::debug_gui() {
	ImGui::DragFloat("Intensity", &luminanceExtractionInfo.get_data()->intensity, 0.001f, 0.0f, 1.0f, "%.4f");
}
#endif // _DEBUG
