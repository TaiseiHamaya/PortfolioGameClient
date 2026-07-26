#pragma once

#include <Engine/Application/Framework.h>

/// <summary>
/// ゲームアプリケーションメインクラス
/// ゲーム全体の初期化と実行を管理
/// </summary>
class Portfolio final : public szg::Framework {
public:
	void initialize() override;
};

