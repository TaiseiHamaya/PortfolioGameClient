#pragma once

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

class LuminanceExtractionNode : public SingleRenderTargetNode {
public:
	LuminanceExtractionNode() = default;
	~LuminanceExtractionNode() noexcept = default;

	LuminanceExtractionNode(const LuminanceExtractionNode&) = delete;
	LuminanceExtractionNode& operator=(const LuminanceExtractionNode&) = delete;
	LuminanceExtractionNode(LuminanceExtractionNode&&) = default;
	LuminanceExtractionNode& operator=(LuminanceExtractionNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void draw() const;

	void set_texture_resource(Reference<RenderTexture> baseTexture_);

private:
	void create_pipeline_state();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	Reference<RenderTexture> baseTexture;

	struct LuminanceExtractionInfo {
		r32 intensity;
	};
	ConstantBuffer<LuminanceExtractionInfo> luminanceExtractionInfo;
};
