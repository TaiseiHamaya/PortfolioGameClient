#pragma once

#include <Engine/Module/Render/RenderPSO/PostEffectPSO.h>

#include <array>

#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

/// <summary>
/// 4枚のテクスチャを合成するノード
/// </summary>
class MargeTextureNode : public PostEffectPSO {
public:

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void execute_effect_command() override;

	void set_texture_resources(std::array<Reference<RenderTexture>, 4> downSampledTextures_);

private:
	void create_pipeline_state();

private:
	std::array<Reference<RenderTexture>, 4> downSampledTextures;
};
