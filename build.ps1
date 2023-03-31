# PowerShell script to build the MSBuild solution in this directory.

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

# Get first solution file in the same directory as the script.
$SolutionFile = Get-ChildItem "$PSScriptRoot\*.sln" | ForEach-Object { $_.FullName} | Select-Object -First 1
if ($SolutionFile -eq $null) {
    Exit-Script "No solution file found in $PSScriptRoot"
}
Write-Output "Solution file: $SolutionFile"

# Find architecture.
$OSArch = (Get-WmiObject Win32_OperatingSystem).OSArchitecture
$Platform = if ($OSArch -like "*arm*") {"arm64"} elseif ($OSArch -like "*64*") {"x64"} else {"Win32"}
$PlatformDir = $Platform -replace "Win32","x86"
Write-Output "Platform: $Platform"

# Find MSBuild
Write-Output "Searching MSBuild..."
$MSRoots = @("C:\Program Files*\MSBuild", "C:\Program Files*\Microsoft Visual Studio")
$MSBuild = Get-ChildItem $MSRoots -Recurse -Include MSBuild.exe -ErrorAction Ignore | ForEach-Object { $_.FullName} | Select-Object -First 1
if ($MSBuild -eq $null) {
    Exit-Script "MSBuild not found"
}

# Build the project.
Write-Output "MSBuild: $MSBuild"
& $MSBuild $SolutionFile /nologo /property:Configuration=Release /property:Platform=$Platform

# Build archive binaries.
$Archive = (Get-Item $SolutionFile).DirectoryName + "\" + (Get-Item $SolutionFile).BaseName + ".zip"
Write-Output "Archive: $Archive"
$ProgressPreference = "SilentlyContinue"
Get-ChildItem "$PSScriptRoot\$PlatformDir\Release" -Depth 1 -Include @("*.exe", "*.dll", "*.reg") |
    Compress-Archive -Force -DestinationPath "$Archive"

Exit-Script
