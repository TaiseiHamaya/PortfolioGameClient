#pragma once

#include <Engine/Runtime/Scene/Scene.h>

/// <summary>
/// 切断シーン
/// サーバーとの接続が失われた時に表示される各指示を提供
/// </summary>
class DisconnectedScene final : public szg::Scene {
public:
	DisconnectedScene() noexcept;
	~DisconnectedScene() noexcept override = default;

	SZG_CLASS_MOVE_ONLY(DisconnectedScene)

public:
	void custom_setup() override;

private:
};
