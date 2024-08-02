import socket

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 8080))  # Bind to localhost and port 8080
    server_socket.listen(1)

    print("Waiting for connection...")
    conn, addr = server_socket.accept()
    print(f"Connected by {addr}")

    # Receive data
    data = conn.recv(1024).decode('utf-8')  # Adjust buffer size as needed
    print(f"Received message: {data}")

    conn.close()
    server_socket.close()

if __name__ == "__main__":
    main()