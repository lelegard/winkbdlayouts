# Install the keyboard layout DLL's in the system.

[CmdletBinding(SupportsShouldProcess=$true)]
param([switch]$NoPause = $false)

# Find architecture directory.
$OSArch = (Get-WmiObject Win32_OperatingSystem).OSArchitecture
$Arch = if ($OSArch -like "*arm*") {"arm64"} elseif ($OSArch -like "*64*") {"x64"} else {"x86"}

if (Test-Path "$PSScriptRoot\$Arch\Release\kbdadmin.exe") {
    & "$PSScriptRoot\$Arch\Release\kbdadmin.exe" -i "$PSScriptRoot\$Arch\Release"
}
elseif (Test-Path "$PSScriptRoot\$Arch\setup.exe") {
    & "$PSScriptRoot\$Arch\setup.exe"
}
else {
    Write-Output "No installation program found"
}

if (-not $NoPause) {
    pause
}
