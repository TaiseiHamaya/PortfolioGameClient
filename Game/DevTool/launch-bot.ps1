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

# 座標を計算する関数（正方形配置）
function Get-SquarePositions {
	param([int]$Count)

	$positions = @()

	# 正方形の一辺のサイズを計算
	$side = [Math]::Ceiling([Math]::Sqrt($Count))
	
	$index = 0
	for ($y = 0; $y -lt $side; $y++) {
		for ($x = 0; $x -lt $side; $x++) {
			if ($index -ge $Count) {
				break
			}

			# (0,0)を始点として座標を計算
			$positions += [PSCustomObject]@{X = $x * 3; Y = $y * 3}
			$index++
		}
		if ($index -ge $Count) {
			break
		}
	}

	return $positions
}

# プログラムパスの設定
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$gameDir = Split-Path -Parent $scriptDir
$solutionDir = Split-Path -Parent $gameDir
$rootDir = Split-Path -Parent $solutionDir
$Path = Join-Path $rootDir "generated\outputs\x64\$BuildType\DirectXGame.exe"

# ビルド処理(-NoBuildが指定されていない限り、デフォルトで自動判定)
if (-not $NoBuild) {
	Write-Host "ソリューションをビルドしています..." -ForegroundColor Cyan
	Write-Host "マクロ定義: ENABLE_DEVELOP_BOT" -ForegroundColor Cyan

	$slnFile = Get-ChildItem -Path $solutionDir -Filter "*.sln" | Select-Object -First 1
	
	if (-not (Test-Path $slnFile)) {
		Write-Host "ソリューションファイルが見つかりません。" -ForegroundColor Red
		exit 1
	}

	$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

	if (-not (Test-Path $msbuild)) {
		$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
	}

	if (-not (Test-Path $msbuild)) {
		$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
	}

	if (-not (Test-Path $msbuild)) {
		Write-Host "MSBuild.exeが見つかりません。" -ForegroundColor Red
		exit 1
	}

	$env:CL = "/D ENABLE_DEVELOP_BOT=1"

	# ビルド引数の構築
	$buildArgs = @(
		$slnFile.FullName,
		"/p:Configuration=$BuildType",
		"/p:Platform=x64",
		"/m"
	)

	& $msbuild $buildArgs

	# 環境変数をクリア
	$env:CL = $null

	if ($LASTEXITCODE -ne 0) {
		Write-Host "ビルドに失敗しました。" -ForegroundColor Red
		exit 1
	}

	Write-Host "ビルドが完了しました。" -ForegroundColor Green
}
else {
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

# 座標を計算
$positions = Get-SquarePositions -Count $Count

Write-Host "一時ディレクトリ: $tempDir" -ForegroundColor Cyan
Write-Host "$Count 回プログラムを起動します...`n" -ForegroundColor Green

# プロセスを格納する配列
$processes = @()

# プログラムを指定回数起動
for ($i = 0; $i -lt $Count; $i++) {
	$pos = $positions[$i]
	$runtimeArgs = "-x $($pos.X) -y $($pos.Y)"
	
	Write-Host "[$($i+1)/$Count] 起動中... (座標: $($pos.X), $($pos.Y))" -ForegroundColor Yellow
	$process = Start-Process -FilePath $tempExePath -ArgumentList $runtimeArgs -WorkingDirectory $tempDir -PassThru
	$processes += $process
	Start-Sleep -Milliseconds 50  # 起動間隔を設定
}

Write-Host "`nすべてのプログラムを起動しました。" -ForegroundColor Green
Write-Host "Enterキーを押すと、すべてのプログラムを終了します..." -ForegroundColor Yellow

# ユーザーの入力を待つ
Read-Host

Write-Host "`nすべてのプログラムを終了しています..." -ForegroundColor Cyan

# すべてのプロセスを終了
foreach ($process in $processes) {
	if (-not $process.HasExited) {
		try {
			$process.Kill()
			Write-Host "プロセス ID $($process.Id) を終了しました。" -ForegroundColor Gray
		}
		catch {
			Write-Host "プロセス ID $($process.Id) の終了に失敗しました: $_" -ForegroundColor Red
		}
	}
	else {
		Write-Host "プロセス ID $($process.Id) は既に終了しています。" -ForegroundColor Gray
	}
}

Write-Host "`nすべてのプロセスを終了しました。" -ForegroundColor Green
