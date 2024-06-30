<#
.SYNOPSIS
    Start an online Plants vs. Zombies battle.
.DESCRIPTION
    Start Plants vs. Zombies with dynamic-link library injection.
    Two online players defend and attack as the plant side and zombie side respectively..
.PARAMETER Role
    The player's role, which can be "Plant" or "Zombie".
.EXAMPLE
    PS> .\Startup.ps1 -Role 'Plant'
.LINK
    https://github.com/Zhuagenborn/Plants-vs.-Zombies-Online-Battle
#>

[CmdletBinding()]
param (
    [Parameter(Mandatory)]
    [ValidateSet('Plant', 'Zombie')]
    [string]$Role
)

try {
    Start-Process -FilePath 'Dll-Injector' -ArgumentList '-f', 'PlantsVsZombies.exe', "$($Role.ToLower()).dll"
} catch {
    Write-Host $_ -ForegroundColor Red
}