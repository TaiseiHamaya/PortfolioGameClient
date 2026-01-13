#pragma once

#include <Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h>

#include <Engine/Assets/Texture/TextureAsset.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h>
#include <Engine/Assets/PrimitiveGeometry/PrimitiveGeometryAsset.h>
#include <Library/Math/ColorRGB.h>

namespace szg {

class CameraInstance;

}

/// <summary>
/// キューブマップ描画ノード
/// </summary>
class CubemapNode final : public szg::BaseRenderPipeline {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override;

	void set_cubemap_texture(const std::string& name);
	void set_camera(Reference<const szg::CameraInstance> camera_);

	void write_position(const Vector3& position);

private:
	void create_pipeline_state();

private:
	struct Material {
		ColorRGB color{ CColorRGB::WHITE };
		u32 texture;
	};

private:
	std::shared_ptr<const szg::PrimitiveGeometryAsset> indexBuffer;
	Reference<const szg::CameraInstance> camera;
	std::shared_ptr<const szg::TextureAsset> cubemapTexture;
	szg::ConstantBuffer<Vector3> vsBuffer;
	szg::ConstantBuffer<Material> materialBuffer;
};
