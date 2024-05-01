@echo off

echo Opening python proxy server...

start "" wsl.exe bash -c "sudo python /mnt/c/Users/CUAV/Documents/Projects/radar_process_linux/Scripts/proxy.py; cuav; exec bash" < nul

echo Proxy server running
echo Starting radar processor...

