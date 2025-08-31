#include "GaussianBlurNode.h"

#include <Engine/GraphicsAPI/DirectX/DxCommand/DxCommand.h>
#include <Engine/GraphicsAPI/DirectX/DxPipelineState/DxPipelineState.h>
#include <Engine/GraphicsAPI/DirectX/DxPipelineState/PSOBuilder/PSOBuilder.h>

void GaussianBlurNode::initialize() {
	create_pipeline_state();
	pipelineState->set_name("GaussianBlur");
	primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	blurInfo.get_data()->dispersion = 0.4f;
	blurInfo.get_data()->length = 10.0f;
	blurInfo.get_data()->sampleCount = 4;
}

void GaussianBlurNode::draw() const {
	baseTexture->start_read();
	auto&& command = DxCommand::GetCommandList();
	command->SetGraphicsRootConstantBufferView(0, blurInfo.get_resource()->GetGPUVirtualAddress());
	baseTexture->get_as_srv()->use(1);
	command->DrawInstanced(3, 1, 0, 0);
}

void GaussianBlurNode::set_base_texture(Reference<RenderTexture> baseTexture_) {
	baseTexture = baseTexture_;
}

void GaussianBlurNode::set_parameters(float dispersion, float length, uint32_t sampleCount) {
	*blurInfo.get_data() = {
		.dispersion = dispersion,
		.length = length,
		.sampleCount = sampleCount
	};
}

void GaussianBlurNode::create_pipeline_state() {
	RootSignatureBuilder rootSignatureBuilder;
	rootSignatureBuilder.add_cbv(D3D12_SHADER_VISIBILITY_PIXEL, 0);
	rootSignatureBuilder.add_texture(D3D12_SHADER_VISIBILITY_PIXEL);
	rootSignatureBuilder.sampler(
		D3D12_SHADER_VISIBILITY_PIXEL,
		0, 0,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	std::unique_ptr<PSOBuilder> psoBuilder = std::make_unique<PSOBuilder>();
	psoBuilder->blendstate();
	psoBuilder->rasterizerstate();
	psoBuilder->rootsignature(rootSignatureBuilder.build());
	psoBuilder->shaders(ShaderType::Vertex, "FullscreenShader.VS.hlsl");
	psoBuilder->shaders(ShaderType::Pixel, "GaussianBlur.PS.hlsl");
	psoBuilder->primitivetopologytype();
	psoBuilder->rendertarget();

	pipelineState = std::make_unique<DxPipelineState>();
	pipelineState->initialize(psoBuilder->get_rootsignature(), psoBuilder->build());
}

#ifdef DEBUG_FEATURES_ENABLE
#include <imgui.h>
void GaussianBlurNode::debug_gui() {
	ImGui::DragFloat("Weight", &blurInfo.get_data()->dispersion, 0.001f, 0.0f, 1.0f, "%.4f");
	ImGui::DragFloat("Length", &blurInfo.get_data()->length, 0.01f);
	static uint32_t min = 1;
	static uint32_t max = 16;
	ImGui::DragScalar("SampleCount", ImGuiDataType_U32, reinterpret_cast<int*>(&blurInfo.get_data()->sampleCount), 0.02f, &min, &max);
}
#endif // DEBUG_FEATURES_ENABLE
