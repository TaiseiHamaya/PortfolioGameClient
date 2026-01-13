#pragma once

#include <Engine/Module/DrawExecutor/BaseDrawExecutor.h>

#include <Engine/Assets/PolygonMesh/PolygonMesh.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>

namespace szg {

class DirectionalLightingExecutor;
class CameraInstance;
class TextureAsset;

}

/// <summary>
/// 環境マップ付きメッシュ描画実行クラス
/// </summary>
class EnvironmentMeshExecutor final : public szg::BaseDrawExecutor<szg::StaticMeshInstance> {
public:
	EnvironmentMeshExecutor() = default;
	~EnvironmentMeshExecutor() noexcept = default;

	EnvironmentMeshExecutor(const std::string& meshName, u32 maxInstance, const std::string& environmentTextureName);

	SZG_CLASS_MOVE_ONLY(EnvironmentMeshExecutor)

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="mesh">メッシュ名</param>
	/// <param name="maxInstance">最大インスタンス数</param>
	/// <param name="environmentTexture_">環境マップテクスチャ</param>
	void reinitialize(std::shared_ptr<const szg::PolygonMesh> mesh, u32 maxInstance, std::shared_ptr<const szg::TextureAsset> environmentTexture_);
	
	/// <summary>
	/// ライトとカメラの設定
	/// </summary>
	/// <param name="directionalLightingExecutor_"></param>
	/// <param name="camera_"></param>
	void setup(Reference<const szg::DirectionalLightingExecutor> directionalLightingExecutor_, Reference<const szg::CameraInstance> camera_);
	
	/// <summary>
	/// コマンドを積む
	/// </summary>
	void draw_command() const override;
	
	/// <summary>
	/// GPUバッファに書き込み
	/// </summary>
	/// <param name="instance"></param>
	void write_to_buffer(Reference<const szg::StaticMeshInstance> instance) override;

private:
	std::shared_ptr<const szg::PolygonMesh> asset;
	std::shared_ptr<const szg::TextureAsset> environmentTexture;

	szg::StructuredBuffer<szg::TransformMatrixDataBuffer> matrices;
	std::vector<szg::StructuredBuffer<szg::MaterialDataBufferRGB>> materials;

	Reference<const szg::DirectionalLightingExecutor> directionalLightingExecutor;
	Reference<const szg::CameraInstance> camera;
};
