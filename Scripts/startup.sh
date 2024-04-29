#!/bin/bash

echo "Opening python proxy server..."
wsl.exe bash -c "
sudo python /mnt/c/Users/CUAV/Documents/Projects/radar_process_linux/Scripts/proxy.py <<EOF
cuav
EOF;
exec bash
" &

echo "Proxy server running\nStarting radar processor..."

wsl.exe bash -c "
radar_process & <<EOF
u
EOF
exec bash
" &

echo "Processor running\nConnecting to Pi..."

wsl.exe bash -c "
    ssh radarpi@192.168.1.11 << EOF
source gimbal_control_env/bin/activate
cd gimbal
sudo python gimbal_server.py
exec bash
EOF
" &

echo "Connected to Pi. Opening RadarUI..."

"C:\Users\CUAV\Documents\Projects\EchoDyne Radar\Radar-UI-v1.3.0\radarui.bat"