New-Item -Name 'build' -ItemType 'Directory' -Force
Set-Location -Path 'build'

Start-Process -FilePath 'cmake' -ArgumentList '..', '-G "Visual Studio 17 2022"', '-A Win32' -NoNewWindow -Wait
Start-Process -FilePath 'cmake' -ArgumentList '--build', '.' -NoNewWindow -Wait