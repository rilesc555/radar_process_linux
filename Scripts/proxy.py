import socket
from multiprocessing import Process, current_process
import signal
import sys
import threading

processes = []
sockets = []

def signal_handler(sig, frame):
    print('You pressed Ctrl+C!')
    for socket in sockets:
        socket.close()
    for process in processes:
        process.terminate()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

PROXY_IP = '127.0.0.1'
PROXY_COMMAND_PORT = 23
PROXY_STATUS_PORT = 29979
PROXY_TRACK_PORT = 29982
PROXY_DETECTIONS_PORT = 29981
PROXY_MEASUREMENTS_PORT = 29984


# IP address and port of the other device
RADAR_IP = '192.168.1.2'

# IP address and port of the client to send siphoned data
CLIENT_IP = '127.0.0.1'
CLIENT_PORT = 60000

def handle_client(starter_socket, radar_socket, process_socket=None):
    close = [False]

    def forward_to_radar(client_socket, radar_socket):
        client_socket.settimeout(3)
        while True:
            try:    
                if close[0]:
                    break
                data = client_socket.recv(4096)
                if not data:
                    close[0] = True
                    break
                radar_socket.sendall(data)
            except socket.timeout:
                continue

    def forward_to_client(radar_socket, client_socket, process_socket=None):
        radar_socket.settimeout(3)  
        while True:
            try:
                if close[0]:
                   break
                data = radar_socket.recv(4096)
                if not data:
                    close[0] = True
                    break
                client_socket.sendall(data)
                if process_socket:
                    process_socket.sendall(data)
            except socket.timeout:
                continue

    thread1name = f"{current_process().name} forward to radar"
    thread2name = f"{current_process().name} return to bnet" 

    Process1 = threading.Thread(target=forward_to_radar, name = thread1name, args=(starter_socket, radar_socket))
    Process2 = threading.Thread(target=forward_to_client, name = thread2name, args=(radar_socket, starter_socket, process_socket))
    Process1.start()
    Process2.start()
    Process1.join()
    Process2.join()

    starter_socket.close()
    print(f"{current_process().name} closed connection to bnet")
    radar_socket.close()
    print(f"{current_process().name} closed connection to radar")
    if process_socket:
        process_socket.close()
        print(f"{current_process().name} closed connection to kalman filter and pi")
    
def start_server(portToSpoof):
    print(f"Started {current_process().name} thread")
    proxy_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sockets.append(proxy_socket)
    proxy_socket.bind((PROXY_IP, portToSpoof))
    proxy_socket.listen(1)
    
    while True:    
        print(f"Proxy server listening on port {portToSpoof}")
        starter_socket, addr = proxy_socket.accept()
        print(f"{current_process().name} process accepted connection from {addr}")

        radar_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sockets.append(radar_socket)
        radar_socket.connect((RADAR_IP, portToSpoof))

        if current_process().name == "track":
            radar_process_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sockets.append(radar_process_socket)
            radar_process_socket.connect((CLIENT_IP, CLIENT_PORT))
            print("Connected to track process socket")
            handle_client(starter_socket, radar_socket, radar_process_socket)
        else:    
            handle_client(starter_socket, radar_socket)

# Usage example
if __name__ == '__main__':
    commandProcess = Process(target=start_server, name="command", args=[PROXY_COMMAND_PORT])
    statusProcess = Process(target=start_server, name="status", args=[PROXY_STATUS_PORT])
    trackProcess = Process(target=start_server, name="track", args=[PROXY_TRACK_PORT])
    detectionsProcess = Process(target=start_server, name="detections", args=[PROXY_DETECTIONS_PORT])
    measurementsProcess = Process(target=start_server, name="measurements", args=[PROXY_MEASUREMENTS_PORT])

    commandProcess.start()
    statusProcess.start()
    trackProcess.start()
    detectionsProcess.start()
    measurementsProcess.start()

    commandProcess.join()
    statusProcess.join()
    trackProcess.join()
    detectionsProcess.join()
    measurementsProcess.join()
    