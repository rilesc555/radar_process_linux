import socket
import threading

PROXY_IP = '127.0.0.1'
PROXY_COMMAND_PORT = RADAR_COMMAND_PORT = 23
PROXY_STATUS_PORT = RADAR_STATUS_PORT = 29979
PROXY_TRACK_PORT = RADAR_TRACK_PORT = 29982

# IP address and port of the other device
RADAR_IP = '192.168.1.2'

# IP address and port of the client to send siphoned data
CLIENT_IP = '127.0.0.1'
CLIENT_PORT = 9999


def handle_client(bnet_socket, radar_socket, radar_process_socket):
    wait = [True]

    def forward_data(src_socket, dst_socket):
        if src_socket == radar_socket:
            filename = threading.current_thread().name + "_test.bin"
            file = open(filename, 'wb')
            src_socket.settimeout(5)
        while True:
            try:
                if not wait[0]:
                    break
                data = src_socket.recv(4096)
                if not data:
                    if src_socket == radar_socket:
                        file.close()
                    else:
                        wait[0] = False
                    break
                dst_socket.sendall(data)
                if src_socket == radar_socket:
                    radar_process_socket.sendall(data)
            except socket.timeout:
                continue

    thread1name = f"{threading.current_thread().name} forward to radar"
    thread2name = f"{threading.current_thread().name} return to bnet"

    thread1 = threading.Thread(target=forward_data, name=thread1name, args=(bnet_socket, radar_socket))
    print(thread1name + " created")
    thread2 = threading.Thread(target=forward_data, name=thread2name, args=(radar_socket, bnet_socket))
    print(thread2name + " created")
    print()
    thread1.start()
    thread2.start()

    thread1.join()
    thread2.join()

    bnet_socket.close()
    print(f"Closed bnet socket on {threading.current_thread().name} thread")
    radar_socket.close()
    print(f"Closed radar socket on {threading.current_thread().name} thread")
    radar_process_socket.close()
    print(f"Closed radar process socket on {threading.current_thread().name} thread")


def start_server(portToSpoof, radarPort):
    print("Started " + threading.current_thread().name + " thread")
    proxy_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    proxy_socket.bind((PROXY_IP, portToSpoof))
    proxy_socket.listen(1)

    while True:
        bnet_socket, addr = proxy_socket.accept()
        print(
            f"{threading.current_thread().name} thread accepted connection from {addr} on {proxy_socket.getsockname()}")

        radar_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        radar_socket.connect((RADAR_IP, radarPort))
        print(f"{threading.current_thread().name} socket opened with radar")

        radar_process_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        radar_process_socket.connect((CLIENT_IP, CLIENT_PORT))

        handle_client(bnet_socket, radar_socket)
        # client_thread = threading.Thread(target=handle_client, args=(starter_socket, radar_socket))
        # client_thread.start()


# Usage example
if __name__ == '__main__':
    commandThread = threading.Thread(target=start_server, name="command", args=(PROXY_COMMAND_PORT, RADAR_COMMAND_PORT))
    statusThread = threading.Thread(target=start_server, name="status", args=(PROXY_STATUS_PORT, RADAR_STATUS_PORT))
    trackThread = threading.Thread(target=start_server, name="track", args=(PROXY_TRACK_PORT, RADAR_TRACK_PORT))

    commandThread.start()
    statusThread.start()
    trackThread.start()

    commandThread.join()
    statusThread.join()
    trackThread.join()
