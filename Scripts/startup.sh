#!/bin/bash

wsl.exe /bin/bash -c "
    sudo python /mnt/c/Users/CUAV/Documents/Projects/radar_process_linux/Scripts/proxy.py & <<EOF
cuav
EOF
"

wsl
wsl.exe /bin/bash -c "
    ssh radarpi@192.168.1.11 << ENDSSH
    source gimbal_control_env/bin/activate
    cd gimbal
    sudo python gimbal_server.py
ENDSSH
"