# Install the keyboard layout DLL's in the system.

[CmdletBinding(SupportsShouldProcess=$true)]
param([switch]$NoPause = $false)

$OSArch = (Get-WmiObject Win32_OperatingSystem).OSArchitecture
$Arch = if ($OSArch -like "*arm*") {"arm64"} elseif ($OSArch -like "*64*") {"x64"} else {"x86"}
$EndOpt = if ($NoPause) {"-np"} else {"-p"}

if (Test-Path "$PSScriptRoot\$Arch\Release\kbdadmin.exe") {
    & "$PSScriptRoot\$Arch\Release\kbdadmin.exe" -i "$PSScriptRoot\$Arch\Release" $EndOpt
}
elseif (Test-Path "$PSScriptRoot\$Arch\setup.exe") {
    & "$PSScriptRoot\$Arch\setup.exe" $EndOpt
}
else {
    Write-Output "No installation program found"
    if (-not $NoPause) {pause}
}
