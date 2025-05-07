#pragma once

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

class DownSamplingNode final : public SingleRenderTargetNode {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	/// <summary>
	/// 描画
	/// </summary>
	void draw();

	void set_base_texture(Reference<RenderTexture> baseTexture);

private:
	/// <summary>
	/// PSO生成
	/// </summary>
	void create_pipeline_state();

private:
	Reference<RenderTexture> texture;
};
