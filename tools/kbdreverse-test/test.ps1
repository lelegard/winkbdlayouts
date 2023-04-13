# Full test of a reversed keyboard layout.

[CmdletBinding(SupportsShouldProcess=$true)]
param([switch]$NoPause = $false)

# A function to exit this script.
function Exit-Script([string]$Message = "")
{
    if ($Message -ne "") {
        Write-Host "ERROR: $Message"
    }
    if (-not $NoPause) {
        pause
    }
    exit
}

$ProjectSolutionFile = "$PSScriptRoot\..\..\winkbdlayouts.sln"
$TestSolutionFile = "$PSScriptRoot\kbdtest.sln"
$TestSourceFile = "$PSScriptRoot\kbdtest.c"

# Current architecture.
$OSArch = (Get-WmiObject Win32_OperatingSystem).OSArchitecture
$Arch = if ($OSArch -like "*arm*") {"arm64"} elseif ($OSArch -like "*64*") {"x64"} else {"x86"}

# Find MSBuild
Write-Output "Searching MSBuild..."
$MSRoots = @("C:\Program Files*\MSBuild", "C:\Program Files*\Microsoft Visual Studio")
$MSBuild = Get-ChildItem $MSRoots -Recurse -Include MSBuild.exe -ErrorAction Ignore | ForEach-Object { $_.FullName} | Select-Object -First 1
if ($MSBuild -eq $null) {
    Exit-Script "MSBuild not found"
}
Write-Output "MSBuild: $MSBuild"

# Build the project for each architecture.
& $MSBuild $ProjectSolutionFile /nologo /property:Configuration=Release /property:Platform=$Arch /target:kbdreverse

# Run the generated reverse.exe on the "fr" keyboard from the system.
Remove-Item $TestSourceFile -Force -ErrorAction SilentlyContinue
. "$PSScriptRoot\..\..\$Arch\Release\kbdreverse.exe" fr | Out-File $TestSourceFile -Encoding ascii
& $MSBuild $TestSolutionFile /nologo /property:Configuration=Release /property:Platform=$Arch

Exit-Script
