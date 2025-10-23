#pragma once

#include <Engine/Module/DrawExecutor/BaseDrawExecutor.h>

#include <Engine/Assets/PolygonMesh/PolygonMesh.h>
#include <Engine/GraphicsAPI/DirectX/DxResource/StructuredBuffer/StructuredBuffer.h>
#include <Engine/Module/World/Mesh/StaticMeshInstance.h>

class DirectionalLightingExecutor;
class Camera3D;
class TextureAsset;

/// <summary>
/// 環境マップ付きメッシュ描画実行クラス
/// </summary>
class EnvironmentMeshExecutor final : public BaseDrawExecutor<StaticMeshInstance> {
public:
	EnvironmentMeshExecutor() = default;
	~EnvironmentMeshExecutor() noexcept = default;

	EnvironmentMeshExecutor(std::string meshName, u32 maxInstance, std::string environmentTextureName);

	__CLASS_NON_COPYABLE(EnvironmentMeshExecutor)

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="mesh">メッシュ名</param>
	/// <param name="maxInstance">最大インスタンス数</param>
	/// <param name="environmentTexture_">環境マップテクスチャ</param>
	void reinitialize(std::shared_ptr<const PolygonMesh> mesh, u32 maxInstance, std::shared_ptr<const TextureAsset> environmentTexture_);
	
	/// <summary>
	/// ライトとカメラの設定
	/// </summary>
	/// <param name="directionalLightingExecutor_"></param>
	/// <param name="camera_"></param>
	void setup(Reference<const DirectionalLightingExecutor> directionalLightingExecutor_, Reference<const Camera3D> camera_);
	
	/// <summary>
	/// コマンドを積む
	/// </summary>
	void draw_command() const override;
	
	/// <summary>
	/// GPUバッファに書き込み
	/// </summary>
	/// <param name="instance"></param>
	void write_to_buffer(Reference<const StaticMeshInstance> instance) override;

private:
	std::shared_ptr<const PolygonMesh> asset;
	std::shared_ptr<const TextureAsset> environmentTexture;

	StructuredBuffer<TransformMatrixDataBuffer> matrices;
	std::vector<StructuredBuffer<MaterialDataBuffer3>> materials;

	Reference<const DirectionalLightingExecutor> directionalLightingExecutor;
	Reference<const Camera3D> camera;
};
