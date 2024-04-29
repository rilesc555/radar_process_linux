@echo off

echo Opening python proxy server...

start "" wsl.exe bash -c "sudo python /mnt/c/Users/CUAV/Documents/Projects/radar_process_linux/Scripts/proxy.py" < nul

echo Proxy server running\nStarting radar processor...

start "" wsl.exe bash -c "radar_process" < nul

echo Processor running\nConnecting to Pi...

start "" wsl.exe bash -c "ssh radarpi@192.168.1.11 << EOF
source gimbal_control_env/bin/activate
cd gimbal
sudo python gimbal_server.py
EOF
"

echo Connected to Pi. Opening RadarUI...

start "" "C:\Users\CUAV\Documents\Projects\EchoDyne Radar\Radar-UI-v1.3.0\radarui.bat"