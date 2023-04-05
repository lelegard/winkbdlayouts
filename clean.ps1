# PowerShell script to cleanup all generated files

[CmdletBinding(SupportsShouldProcess=$true)]
param([switch]$NoPause = $false)

$CleanupFiles = @("arm64", "x64", "x86", "Win32", "Debug", "Release", ".vs", "*.vcxproj.user", "*.aps", "*.zip", "*.log", "kbdtest.c", "*.tmp", "tmp")

Get-ChildItem $PSScriptRoot -Recurse -Include $CleanupFiles -Force | Remove-Item -Recurse -Force

if (-not $NoPause) {
    pause
}
