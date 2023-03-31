# Install the keyboard layout DLL's in the system.

[CmdletBinding(SupportsShouldProcess=$true)]
param([switch]$NoPause = $false)

# Must be admin.
$IsAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

# Find architecture directory.
$OSArch = (Get-WmiObject Win32_OperatingSystem).OSArchitecture
$PlatformDir = if ($OSArch -like "*arm*") {"arm64"} elseif ($OSArch -like "*64*") {"x64"} else {"x86"}

# Find keyboard DLL's. First, check in arch\Release (build hierarchy).
$BinDir = "$PSScriptRoot\$PlatformDir\Release"
$Files = if (Test-Path $BinDir) {Get-ChildItem $BinDir -Depth 1 -Include "kbd*.dll"} else {$null}

# If not found, check arch directory (archive hierarchy).
if ($File -eq $null) {
    $BinDir = "$PSScriptRoot\$PlatformDir"
    $Files = if (Test-Path $BinDir) {Get-ChildItem $BinDir -Depth 1 -Include "kbd*.dll"} else {$null}
}

# Install the keyboard Files
if ($Files -eq $null) {
	Write-Output "No keyboard DLL found in $BinDir"
}
elseif ($IsAdmin) {
    # We are admin, we can install.
	$Sys32 = "$($env:SystemRoot)\System32"
	Write-Output "Copying files in $Sys32"
    $Files | ForEach-Object {
    	$Name = $_.Name
    	$Base = $_.BaseName
    	Write-Output "Installing $Name"
    	Copy-Item $_ -Destination $Sys32
        # Search associated registry file.
        $Reg = "$PSScriptRoot\$Base\$Base.reg"
        if (-not (Test-Path $Reg)) {
            $Reg = "$PSScriptRoot\$Base.reg"
        }
        if (-not (Test-Path $Reg)) {
            Write-Output "ERROR: associated registry file $Base.reg not found"
        }
        else {
            reg import "$Reg"
        }
    }
}
else {
    # Recurse the current script in administrator mode (with UAC dialog).
    Write-Output "Must be administrator to continue, trying to restart as administrator ..."
    $cmd = "& '" + $MyInvocation.MyCommand.Path + "'" + $(if ($NoPause) {" -NoPause"} else {""})
    Start-Process -Wait -Verb runas -FilePath PowerShell.exe -ArgumentList @("-ExecutionPolicy", "RemoteSigned", "-Command", $cmd)
}

if (-not $NoPause) {
    pause
}
