import socket

# Función para decodificar utilizando Hamming (7,4)
def decodificar_hamming(bits):
    n = len(bits)
    r = 0
    
    while (2**r) < (n + 1):
        r += 1
    
    error_pos = 0
    for i in range(r):
        pos = 2**i
        count = 0
        for j in range(1, n + 1):
            if j & pos and int(bits[j - 1]) == 1:
                count += 1
        if count % 2 != 0:
            error_pos += pos
    
    if error_pos:
        bits = list(bits)
        bits[error_pos - 1] = '1' if bits[error_pos - 1] == '0' else '0'
        bits = ''.join(bits)
    
    mensaje = ''
    j = 0
    for i in range(1, n + 1):
        if i != 2**j:
            mensaje += bits[i - 1]
        else:
            j += 1
    
    return mensaje

# Función para decodificar utilizando CRC-32
def decodificar_crc32(bits):
    data_length = len(bits) - 32
    return bits[:data_length]

# Convertir la cadena binaria decodificada en caracteres ASCII
def convertir_binario_a_ascii(mensaje_binario):
    mensaje = ''
    for i in range(0, len(mensaje_binario), 8):
        byte = mensaje_binario[i:i+8]
        caracter = chr(int(byte, 2))
        mensaje += caracter
    return mensaje

# Capa de transmisión: Recibir información
def recibir_informacion():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(('localhost', 65432))
        s.listen()
        conn, addr = s.accept()
        with conn:
            print('Conectado por', addr)
            data = conn.recv(1024)
            if data:
                return data.decode()

def main():
    print("-------------------------------------")
    print("| ¿Qué algoritmo deseas utilizar?   |")
    print("|-----------------------------------|")
    print("| 1. Algoritmo Hamming              |")
    print("| 2. Algoritmo crc-32               |")
    print("-------------------------------------")
    
    opcion = input("Selecciona una opción (1 o 2): ")

    if opcion == '1':
        print("Algoritmo seleccionado: Hamming")
    elif opcion == '2':
        print("Algoritmo seleccionado: crc-32")
    else:
        print("Opción no válida")
        return

    print("Esperando mensaje...")
    cadena_binaria = recibir_informacion()
    print(f"Cadena binaria recibida: {cadena_binaria}")

    if opcion == '1':
        mensaje_binario = decodificar_hamming(cadena_binaria)
    elif opcion == '2':
        mensaje_binario = decodificar_crc32(cadena_binaria)

    print(f"Cadena binaria decodificada: {mensaje_binario}")  # Añadido para mostrar la cadena binaria pura
    mensaje = convertir_binario_a_ascii(mensaje_binario)
    print(f"Mensaje decodificado: {mensaje}")

if __name__ == "__main__":
    main()
