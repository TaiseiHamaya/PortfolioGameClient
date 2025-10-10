param(
    [Parameter(Position = 0)]
    [int]$Count,
    
    [switch]$NoBuild,
    [switch]$ForceBuild,
    
    [Parameter(Position = 1)]
    [string]$BuildType = "Release"
)

# 起動回数の取得
if (-not $Count -or $Count -le 0) {
    $Count = Read-Host "起動回数を入力してください"
    $Count = [int]$Count
}

# プログラムパスの設定
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$gameDir = Split-Path -Parent $scriptDir
$solutionDir = Split-Path -Parent $gameDir
$rootDir = Split-Path -Parent $solutionDir
$Path = Join-Path $rootDir "generated\outputs\x64\$BuildType\DirectXGame.exe"

# ビルド処理（-NoBuildが指定されていない限り、デフォルトで自動判定）
if (-not $NoBuild) {
    Write-Host "ソリューションをビルドしています..." -ForegroundColor Cyan
    Write-Host "マクロ定義: ENABLE_AUTOMOVE" -ForegroundColor Cyan
    
    $slnFile = Get-ChildItem -Path $solutionDir -Filter "*.sln" | Select-Object -First 1
    
    if ($slnFile) {
        $msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
        
        if (-not (Test-Path $msbuild)) {
            $msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
        }
        
        if (-not (Test-Path $msbuild)) {
            $msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
        }
        
        if (Test-Path $msbuild) {
            # ビルド引数の構築（ENABLE_AUTOMOVEマクロを追加）
            $buildArgs = @(
                $slnFile.FullName,
                "/p:Configuration=$BuildType",
                "/p:Platform=x64",
                "/p:PreprocessorDefinitions=ENABLE_AUTOMOVE",
                "/m"
            )
            
            & $msbuild $buildArgs
            
            if ($LASTEXITCODE -ne 0) {
                Write-Host "ビルドに失敗しました。" -ForegroundColor Red
                exit 1
            }
            
            Write-Host "ビルドが完了しました。" -ForegroundColor Green
        } else {
            Write-Host "MSBuild.exeが見つかりません。" -ForegroundColor Red
            exit 1
        }
    } else {
        Write-Host "ソリューションファイルが見つかりません。" -ForegroundColor Red
        exit 1
    }
} else {
    Write-Host "ビルドをスキップします。" -ForegroundColor Cyan
}

# 実行ファイルの存在確認
if (-not (Test-Path $Path)) {
    Write-Host "指定されたプログラムが見つかりません: $Path" -ForegroundColor Red
    exit 1
}

# /tempディレクトリへのコピー
$tempDir = Join-Path $scriptDir "temp"
New-Item -ItemType Directory -Path $tempDir -Force | Out-Null

$exeDir = Split-Path -Parent $Path

# 実行ファイルと同じディレクトリのすべてのファイルをコピー
Write-Host "実行ファイルと依存ファイルをコピーしています..." -ForegroundColor Cyan
Copy-Item -Path "$exeDir\*" -Destination $tempDir -Recurse -Force

# DirectXGame のリソースフォルダをコピー
$directXGameDir = Join-Path $solutionDir "DirectXGame"
$engineResourcesSource = Join-Path $directXGameDir "EngineResources"
$editorResourcesSource = Join-Path $directXGameDir "EditorResources"

$directXGameTempDir = Join-Path $tempDir "DirectXGame"
New-Item -ItemType Directory -Path $directXGameTempDir -Force | Out-Null

if (Test-Path $engineResourcesSource) {
    Write-Host "EngineResourcesをコピーしています..." -ForegroundColor Cyan
    Copy-Item -Path $engineResourcesSource -Destination $directXGameTempDir -Recurse -Force
}

if (Test-Path $editorResourcesSource) {
    Write-Host "EditorResourcesをコピーしています..." -ForegroundColor Cyan
    Copy-Item -Path $editorResourcesSource -Destination $directXGameTempDir -Recurse -Force
}

# Game のリソースフォルダをコピー
$coreSource = Join-Path $gameDir "Core"
$debugDataSource = Join-Path $gameDir "DebugData"
$resourcesSource = Join-Path $gameDir "Resources"

$gameTempDir = Join-Path $tempDir "Game"
New-Item -ItemType Directory -Path $gameTempDir -Force | Out-Null

if (Test-Path $coreSource) {
    Write-Host "Coreをコピーしています..." -ForegroundColor Cyan
    Copy-Item -Path $coreSource -Destination $gameTempDir -Recurse -Force
}

if (Test-Path $debugDataSource) {
    Write-Host "DebugDataをコピーしています..." -ForegroundColor Cyan
    Copy-Item -Path $debugDataSource -Destination $gameTempDir -Recurse -Force
}

if (Test-Path $resourcesSource) {
    Write-Host "Resourcesをコピーしています..." -ForegroundColor Cyan
    Copy-Item -Path $resourcesSource -Destination $gameTempDir -Recurse -Force
}

$exeName = Split-Path -Leaf $Path
$tempExePath = Join-Path $tempDir $exeName

Write-Host "一時ディレクトリ: $tempDir" -ForegroundColor Cyan
Write-Host "$Count 回プログラムを起動します..." -ForegroundColor Green

# プログラムを指定回数起動
for ($i = 1; $i -le $Count; $i++) {
    Write-Host "[$i/$Count] 起動中..." -ForegroundColor Yellow
    Start-Process -FilePath $tempExePath -WorkingDirectory $tempDir
    Start-Sleep -Milliseconds 500  # 起動間隔を設定
}

Write-Host "すべてのプログラムを起動しました。" -ForegroundColor Green
Write-Host "一時ディレクトリ: $tempDir" -ForegroundColor Cyan