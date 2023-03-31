# Install the keyboard layout DLL's in the system.

[CmdletBinding(SupportsShouldProcess=$true)]
param([switch]$NoPause = $false)

# Must be admin.
$IsAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

# Find architecture directory.
$OSArch = (Get-WmiObject Win32_OperatingSystem).OSArchitecture
$PlatformDir = if ($OSArch -like "*arm*") {"arm64"} elseif ($OSArch -like "*64*") {"x64"} else {"x86"}

# Find keyboard DLL's.
$BinDir = "$PSScriptRoot\$PlatformDir\Release"
$Files = if (Test-Path $BinDir) {Get-ChildItem $BinDir -Depth 1 -Include "kbd*.dll"} else {$null}

# Install the keyboard Files
if ($Files -eq $null) {
	Write-Output "No keyboard DLL found in $BinDir"
}
elseif ($IsAdmin) {
	$Sys32 = "$($env:SystemRoot)\System32"
	Write-Output "Copying files in $Sys32"
	Copy-Item -Verbose $Files -Destination $Sys32
}
else {
    # Recurse the current script in administrator mode.
    Write-Output "Must be administrator to continue, trying to restart as administrator ..."
    $cmd = "& '" + $MyInvocation.MyCommand.Path + "'"
    if ($NoPause) {
        $cmd += " -NoPause"
    }
    Start-Process -Wait -Verb runas -FilePath PowerShell.exe -ArgumentList @("-ExecutionPolicy", "RemoteSigned", "-Command", $cmd)
}

if (-not $NoPause) {
    pause
}
