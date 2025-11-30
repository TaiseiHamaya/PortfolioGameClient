#pragma once

#include <Engine/Module/Render/RenderPSO/PostEffectPSO.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

/// <summary>
/// ダウンサンプリング
/// </summary>
class DownSamplingNode final : public PostEffectPSO {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	/// <summary>
	/// 描画
	/// </summary>
	void execute_effect_command() override;

	void set_base_texture(Reference<RenderTexture> baseTexture);

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

private:
	Reference<RenderTexture> texture;
};
