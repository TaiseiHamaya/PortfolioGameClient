#pragma once

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

class GaussianBlurNode : public SingleRenderTargetNode {
public:
	struct GaussianBlurInfo {
		float dispersion;
		float length;
		uint32_t sampleCount;
	};

public:
	GaussianBlurNode() = default;
	~GaussianBlurNode() noexcept = default;

	GaussianBlurNode(const GaussianBlurNode&) = delete;
	GaussianBlurNode& operator=(const GaussianBlurNode&) = delete;
	GaussianBlurNode(GaussianBlurNode&&) = default;
	GaussianBlurNode& operator=(GaussianBlurNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void draw() const;

	void set_base_texture(Reference<RenderTexture> baseTexture_);

public:
	void set_parameters(r32 dispersion, r32 length, uint32_t sampleCount);

private:
	void create_pipeline_state();

#ifdef DEBUG_FEATURES_ENABLE
public:
	void debug_gui();
#endif // DEBUG_FEATURES_ENABLE

private:
	Reference<RenderTexture> baseTexture;

	ConstantBuffer<GaussianBlurInfo> blurInfo;
};
