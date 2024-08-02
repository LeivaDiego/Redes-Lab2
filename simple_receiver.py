import socket

def process_message(message):
    # Procesar el mensaje recibido (imprimir o guardar en un archivo, etc.)
    print(f"Received message:\n{message}\n")

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 8080))  # Bind to localhost and port 8080
    server_socket.listen(1)

    print("Waiting for connection...")
    conn, addr = server_socket.accept()
    print(f"Connected by {addr}")

    try:
        while True:
            # Buffer para almacenar datos recibidos
            buffer = ""
            while True:
                # Leer datos del socket
                data = conn.recv(1024).decode('utf-8')  # Ajustar tamaño del buffer si es necesario
                if not data:
                    break
                buffer += data
                # Verificar si se ha recibido un mensaje completo
                if '\n' in buffer:
                    # Separar el mensaje en partes
                    messages = buffer.split('\n')
                    if len(messages) >= 3:
                        original_message = messages[0]
                        encoded_message = messages[1]
                        noisy_message = messages[2]

                        # Procesar el mensaje recibido
                        process_message(f"Original: {original_message}\n Encoded: {encoded_message}\n Noisy: {noisy_message}")

                        # Limpiar el buffer después de procesar el mensaje
                        buffer = "\n".join(messages[3:])

    except KeyboardInterrupt:
        print("Server interrupted.")

    finally:
        conn.close()
        server_socket.close()

if __name__ == "__main__":
    main()
