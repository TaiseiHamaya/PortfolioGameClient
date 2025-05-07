#pragma once

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

#include <Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/TextureResource/RenderTexture.h>

class BloomNode : public SingleRenderTargetNode {
public:
	BloomNode() = default;
	~BloomNode() noexcept = default;

	BloomNode(const BloomNode&) = delete;
	BloomNode& operator=(const BloomNode&) = delete;
	BloomNode(BloomNode&&) = default;
	BloomNode& operator=(BloomNode&&) = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

	void draw() const;

	void set_base_texture(Reference<RenderTexture> baseTexture);
	void set_blur_texture(Reference<RenderTexture> blurTexture);

private:
	void create_pipeline_state();

#ifdef _DEBUG
public:
	void debug_gui();
#endif // DEBUG

private:
	Reference<RenderTexture> baseTexture;
	Reference<RenderTexture> blurTexture;

	struct BloomInfo {
		float weight;
	};
	ConstantBuffer<BloomInfo> bloomInfo;
};
