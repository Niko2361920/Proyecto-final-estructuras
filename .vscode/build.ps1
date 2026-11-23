$ErrorActionPreference = 'Stop'

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere)) {
    throw 'No se encontro el instalador de Visual Studio.'
}

$installation = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $installation) {
    throw 'No se encontraron las herramientas de compilacion de C++ de Visual Studio.'
}

$projectRoot = Split-Path -Parent $PSScriptRoot
$env:URGENCIAS_VC_ENV = Join-Path $installation 'VC\Auxiliary\Build\vcvars64.bat'
$sources = Get-ChildItem -LiteralPath $projectRoot -Filter '*.cpp' -File
$sourceArguments = ($sources | ForEach-Object { '"..\' + $_.Name + '"' }) -join ' '

Push-Location -LiteralPath $projectRoot
try {
    New-Item -ItemType Directory -Path 'build' -Force | Out-Null
    Set-Location -LiteralPath 'build'
    $compile = 'call "%URGENCIAS_VC_ENV%" >nul && cl.exe /nologo /std:c++17 /EHsc /W3 /Zi /Fe:Urgencias.exe ' + $sourceArguments + ' /link /DEBUG'
    & $env:ComSpec /d /c $compile
    exit $LASTEXITCODE
} finally {
    Pop-Location
}
