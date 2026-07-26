#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Engine/Module/Render/RenderPipeline/BaseRenderPipeline.h>

/// <summary>
/// 環境メッシュレンダリングノード
/// 環境マップ用のメッシュ描画パイプラインを実装
/// </summary>
class EnvironmentMeshNode final : public szg::BaseRenderPipeline {
public:
	EnvironmentMeshNode();
	~EnvironmentMeshNode() noexcept;

	SZG_CLASS_MOVE_ONLY(EnvironmentMeshNode)

public:
	static void BeginLoadShader();

	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void preprocess() override {};

private:
	void create_pipeline_state();
};
