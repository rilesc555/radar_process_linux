import socket

SERVER_IP, SERVER_PORT = '127.0.0.1', 9999

def main():
    with open("listenerTest.bin", 'wb') as file:
        listener_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        listener_socket.bind((SERVER_IP, SERVER_PORT))
        listener_socket.listen(1)
        proxy_socket, addr = listener_socket.accept()
        while True:
            data = listener_socket.recv(4096)
            if not data:
                break
            file.write(data)
        listener_socket.close()

if __name__ == "__main__":
    main()