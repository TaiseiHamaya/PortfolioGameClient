# Game Client for Portfolio

[![ReleaseBuild](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/ReleaseBuild.yml/badge.svg)](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/ReleaseBuild.yml)
[![DevelopBuild](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/DevelopBuild.yml/badge.svg)](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/DevelopBuild.yml)
[![DebugBuild](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/DebugBuild.yml/badge.svg)](https://github.com/TaiseiHamaya/PortfolioGameClient/actions/workflows/DebugBuild.yml)

This is multiplayer action game client developed with C++20 and 'SyzygyEngine'.

## Overview

This repository contains the client-side implementation for a networked action game portfolio project.

It includes:
- Login scene with player name input.
- Main in-game scene with player control, enemy/effect systems, and UI managers.
- TCP-based connection flow to a game server using Asio.
- CI pipelines for `Debug`, `Develop`, and `Release` builds.

## Tech Stack

- Language: C++20
- Build System: MSBuild / Visual Studio solution (`Portfolio.sln`)
- Platform: Windows x64
- Networking: Asio (standalone)
- Serialization: Protocol Buffers (with Abseil)
- Graphics: DirectX12
- Tools: DirectXTex, Dear ImGui, Assimp

## Requirements

- Windows 11
- Visual Studio 2022 with C++ desktop development workload
- Git LFS
- Submodule support (`--recurse-submodules`)

## Getting Started

### 1. Clone

```bash
git clone --recurse-submodules https://github.com/TaiseiHamaya/PortfolioGameClient.git
cd PortfolioGameClient
git lfs pull
```

### 2. Build (Visual Studio)

1. Open `Portfolio.sln` in Visual Studio 2022.
2. Select one configuration from: `Debug | x64`, `Develop | x64`, `Release | x64`.
3. Build the solution.

### 3. Build (MSBuild)

```powershell
msbuild Portfolio.sln /p:Platform=x64,Configuration=Develop
```

You can replace `Develop` with `Debug` or `Release`.

## Run

After build, executable output is expected at:

`../generated/outputs/x64/<Configuration>/Portfolio.exe`

You can also use the helper script:

```powershell
pwsh ./Game/DevTool/launch-bot.ps1 1 -BuildType Develop
```

Notes:
- The script can launch multiple bot clients with `--develop-bot-enable`.
- Default server address is currently hardcoded in `Game/Scripts/Network/GameServer/GameServerConnectionManager.cpp`.

## Screenshots

### Gameplay Preview

![Gameplay Preview GIF](docs/images/gameplay.gif)

### Chat Preview

![Chat Preview GIF](docs/images/chat.gif)

## Controls

### Title / Login Scene

- Move selection: `W` / `S`, `Up` / `Down`, or gamepad left stick
- Confirm/select: `Enter`, `Space`, or gamepad `A`
- Name input: alphabetic characters (up to 12), `Backspace` to delete

### In-Game Scene

- Move: `WASD` or gamepad left stick
- Action 1: key `1` or gamepad `A`

## Project Structure

- `Game/` : game-specific code, assets, gameplay scripts, networking logic
- `SyzygyEngine/` : engine source and third-party integrations
- `.github/workflows/` : CI build pipelines for each configuration
- `Portfolio.sln` : root Visual Studio solution

## CI

GitHub Actions validates builds on every push to `master` for:
- `Debug`
- `Develop`
- `Release`

Each workflow checks out submodules and LFS assets before running MSBuild on Windows runners.
