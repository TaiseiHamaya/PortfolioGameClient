#pragma once

#include <Library/Utility/Tools/ConstructorMacro.h>

#include <Engine/Module/Render/RenderNode/SingleRenderTargetNode.h>

class EnvironmentMeshNode final : public SingleRenderTargetNode {
public:
	EnvironmentMeshNode();
	~EnvironmentMeshNode() noexcept;

	__CLASS_NON_COPYABLE(EnvironmentMeshNode)

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
