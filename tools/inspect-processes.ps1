# PowerShell script to run "inspect -p" in administrator mode

[CmdletBinding(SupportsShouldProcess=$true)]
param([switch]$NoPause = $false)

$IsAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

if ($IsAdmin) {
    $OSArch = (Get-WmiObject Win32_OperatingSystem).OSArchitecture
    $PlatformDir = if ($OSArch -like "*arm*") {"arm64"} elseif ($OSArch -like "*64*") {"x64"} else {"x86"}
    & "$PSScriptRoot\..\$PlatformDir\Release\inspect.exe" -p
}
else {
    Write-Output "Must be administrator to continue, trying to restart as administrator ..."
    $cmd = "& '" + $MyInvocation.MyCommand.Path + "'" + $(if ($NoPause) {" -NoPause"} else {""})
    Start-Process -Wait -Verb runas -FilePath PowerShell.exe -ArgumentList @("-ExecutionPolicy", "RemoteSigned", "-Command", $cmd)
}

if (-not $NoPause) {
    pause
}
