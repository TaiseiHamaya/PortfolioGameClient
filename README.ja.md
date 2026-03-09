# Game Client for Portfolio

[![ReleaseBuild](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/ReleaseBuild.yml/badge.svg)](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/ReleaseBuild.yml)
[![DevelopBuild](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/DevelopBuild.yml/badge.svg)](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/DevelopBuild.yml)
[![DebugBuild](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/DebugBuild.yml/badge.svg)](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/DebugBuild.yml)

`SyzygyEngine` 上で構築された、C++20 ベースのマルチプレイヤーアクションゲームクライアントです。

## 概要

このリポジトリは、ネットワーク対応アクションゲームのポートフォリオ作品におけるクライアント実装です。

主な内容:
- プレイヤー名入力を含むログインシーン
- プレイヤー操作、敵・エフェクト、UI 管理を含むメインゲームシーン
- Asio を用いたゲームサーバーへの TCP 接続フロー
- `Debug` / `Develop` / `Release` の CI ビルド

## 技術スタック

- 言語: C++20
- ビルド: MSBuild / Visual Studio ソリューション (`Portfolio.sln`)
- プラットフォーム: Windows x64
- ネットワーク: Asio (standalone)
- シリアライズ: Protocol Buffers (Abseil 併用)
- グラフィックス: DirectX12
- ツール: DirectXTex, Dear ImGui, Assimp

## 動作要件

- Windows 11
- Visual Studio 2022 (C++ によるデスクトップ開発ワークロード)
- Git LFS
- Submodule 対応 (`--recurse-submodules`)

## セットアップ

### 1. クローン

```bash
git clone --recurse-submodules https://github.com/TaiseiHamaya/PortfolioGameClient.git
cd PortfolioGameClient
git lfs pull
```

### 2. ビルド (Visual Studio)

1. Visual Studio 2022 で `Portfolio.sln` を開きます。
2. `Debug | x64` / `Develop | x64` / `Release | x64` から構成を選びます。
3. ソリューションをビルドします。

### 3. ビルド (MSBuild)

```powershell
msbuild Portfolio.sln /p:Platform=x64,Configuration=Develop
```

`Develop` は `Debug` または `Release` に変更可能です。

## 実行

ビルド後の実行ファイルは次の場所に出力されます:

`../generated/outputs/x64/<Configuration>/Portfolio.exe`

補助スクリプトでも起動できます:

```powershell
pwsh ./Game/DevTool/launch-bot.ps1 1 -BuildType Develop
```

補足:
- スクリプトは `--develop-bot-enable` を使って複数のボットクライアントを起動できます。
- デフォルトのサーバーアドレスは `Game/Scripts/Network/GameServer/GameServerConnectionManager.cpp` にハードコードされています。

## スクリーンショット

### Gameplay Preview

![Gameplay Preview GIF](docs/images/gameplay.gif)

### Chat Preview

![Chat Preview GIF](docs/images/chat.gif)

## 操作方法

### タイトル / ログインシーン

- 選択移動: `W` / `S`、`Up` / `Down`、またはゲームパッド左スティック
- 決定/選択: `Enter`、`Space`、またはゲームパッド `A`
- 名前入力: 英字 (最大 12 文字)、削除は `Backspace`

### ゲームシーン

- 移動: `WASD` またはゲームパッド左スティック
- アクション 1: キー `1` またはゲームパッド `A`

## ディレクトリ構成

- `Game/` : ゲーム固有コード、アセット、ゲームプレイスクリプト、ネットワーク処理
- `SyzygyEngine/` : エンジン本体とサードパーティ統合
- `.github/workflows/` : 各構成向け CI ビルド定義
- `Portfolio.sln` : ルート Visual Studio ソリューション

## CI

GitHub Actions により、`master` への push ごとに以下をビルド検証します:
- `Debug`
- `Develop`
- `Release`

各ワークフローでは Submodule と LFS アセットを取得した後、Windows ランナー上で MSBuild を実行します。