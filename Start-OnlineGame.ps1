<#
.SYNOPSIS
    Start online Plants vs. Zombies.
.DESCRIPTION
    Start Plants vs. Zombies with dynamic-link library injection.
    Two online players defend and attack as the plant side and zombie side respectively..
.PARAMETER Role
    The player's role.
.EXAMPLE
    PS> .\Startup.ps1 -Role 'Plant'
.LINK
    https://github.com/Zhuagenborn/Plants-vs.-Zombies-Online-Battle
.NOTES
    Author: Chen Zhenshuo
    GitHub: https://github.com/czs108

    Author: Liu Guowen
    GitHub: https://github.com/lgw1995
#>

[CmdletBinding()]
param (
    [Parameter(Mandatory)]
    [ValidateSet('Plant', 'Zombie')]
    [String]$Role
)

try {
    Start-Process -FilePath 'Dll-Injector' -ArgumentList '-f', 'PlantsVsZombies.exe', "$($Role.ToLower()).dll"
} catch {
    Write-Host "Exception: $PSItem" -ForegroundColor Red
}