@echo off

echo Opening python proxy server...

start "" wsl.exe bash -c "sudo python /mnt/c/Users/CUAV/Documents/Projects/radar_process_linux/Scripts/proxy.py; cuav; exec bash" < nul

echo Proxy server running
echo Starting radar processor...

start "" wsl.exe bash -c "cd"

ping -n 1 127.0.0.1 > nul

wsl.exe bash -c "radar_process; u; exec bash" < nul

echo Processor running
echo Connecting to Pi...

start /wait "" wsl.exe bash -c "ssh radarpi@192.168.1.11 << EOF
source gimbal_control_env/bin/activate
cd gimbal
sudo python gimbal_server.py
cuav
EOF
"

echo Connected to Pi. Opening RadarUI...

start "" "C:\Users\CUAV\Documents\Projects\EchoDyne Radar\Radar-UI-v1.3.0\radarui.bat"