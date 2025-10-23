#pragma once

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

#include <Engine/Assets/Texture/TextureAsset.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/ConstantBuffer/ConstantBuffer.h>
#include <Engine/Assets/PrimitiveGeometry/PrimitiveGeometryAsset.h>
#include <Library/Math/Color3.h>

class Camera3D;

/// <summary>
/// キューブマップ描画ノード
/// </summary>
class CubemapNode final : public SingleRenderTargetNode {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override;

	void set_cubemap_texture(const std::string& name);
	void set_camera(Reference<const Camera3D> camera_);

	void write_position(const Vector3& position);

private:
	void create_pipeline_state();

private:
	struct Material {
		Color3 color{ CColor3::WHITE };
		u32 texture;
	};

private:
	std::shared_ptr<const PrimitiveGeometryAsset> indexBuffer;
	Reference<const Camera3D> camera;
	std::shared_ptr<const TextureAsset> cubemapTexture;
	ConstantBuffer<Vector3> vsBuffer;
	ConstantBuffer<Material> materialBuffer;
};
