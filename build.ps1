# PowerShell script to build the keyboard files and binary archive.

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

# Create a new directory (delete previous one), return the directory object.
function Create-NewDirectory([string]$DirName)
{
    Remove-Item $DirName -Recurse -Force -ErrorAction SilentlyContinue
    return New-Item -ItemType Directory -Force $DirName
}

# Get first solution file in the same directory as the script.
$SolutionFile = Get-ChildItem "$PSScriptRoot\*.sln" | Select-Object -First 1
if ($SolutionFile -eq $null) {
    Exit-Script "No solution file found in $PSScriptRoot"
}
$ProjectName = $SolutionFile.BaseName
Write-Output "Solution file: $SolutionFile"

# Find MSBuild
Write-Output "Searching MSBuild..."
$MSRoots = @("C:\Program Files*\MSBuild", "C:\Program Files*\Microsoft Visual Studio")
$MSBuild = Get-ChildItem $MSRoots -Recurse -Include MSBuild.exe -ErrorAction Ignore | ForEach-Object { $_.FullName} | Select-Object -First 1
if ($MSBuild -eq $null) {
    Exit-Script "MSBuild not found"
}
Write-Output "MSBuild: $MSBuild"

# Build the project for each architecture.
foreach ($Arch in ("x86", "x64", "arm64")) {
    Write-Output "Building for $Arch ..."
    & $MSBuild $SolutionFile /nologo /property:Configuration=Release /property:Platform=$Arch
}

# Build archive binaries.
$Archive = "$PSScriptRoot\$ProjectName.zip"
Write-Output "Archive: $Archive"
$TempRoot = Create-NewDirectory "$PSScriptRoot\tmp"
$InstallRoot = Create-NewDirectory "$TempRoot\$ProjectName"
Copy-Item "$PSScriptRoot\install.ps1" -Destination $InstallRoot
foreach ($Arch in ("x86", "x64", "arm64")) {
    $Files = Get-ChildItem "$PSScriptRoot\$Arch\Release\kbd*.dll"
    if ($Files -ne $null) {
        Copy-Item $Files -Destination $(Create-NewDirectory "$InstallRoot\$Arch")
    }
}
$ProgressPreference = "SilentlyContinue"
Compress-Archive $InstallRoot -DestinationPath "$Archive" -Force
Remove-Item $TempRoot -Recurse -Force

Exit-Script
