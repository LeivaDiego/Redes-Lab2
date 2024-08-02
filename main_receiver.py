import socket

class CRC32Decoder:
    def __init__(self):
        self.polynomial = "100000100110000010001110110110111"

    def binaryXor(self, a, b):
        result = []
        # Se realiza el XOR bit a bit
        for i in range(1, len(b)):
            # Si los bits son iguales, se agrega un 0 al resultado
            if a[i] == b[i]:
                result.append('0')
            # Si los bits son diferentes, se agrega un 1 al resultado
            else:
                result.append('1')

        return ''.join(result)

    def module2division(self, dividend, divisor):
        # Se obtiene la cantidad de bits a aplicar XOR a la vez
        pick = len(divisor)

        # Se divide el mensaje en partes de tamaño pick
        block = dividend[0: pick]

        # Se obtiene la longitud del mensaje
        n = len(dividend)

        # Se realiza el XOR de los bloques
        while pick < n:
            # Si el primer bit del bloque es 1, 
            # se reemplaza el bloque por el resultado de la operacion XOR
            if block[0] == '1':
                block = self.binaryXor(divisor, block) + dividend[pick]
            # Si el primer bit del bloque es 0,
            # se usa un bloque de ceros para realizar la operacion XOR
            else:
                block = self.binaryXor('0' * pick, block) + dividend[pick]

            # Se incrementa el índice
            pick += 1

        # Se realiza el XOR de los ultimos bits
        if block[0] == '1':
            block = self.binaryXor(divisor, block)
        else:
            block = self.binaryXor('0' * pick, block)

        return block


    def detectError(self, message):
        current_xor = self.module2division(message[:len(self.polynomial)], self.polynomial)
        current = len(self.polynomial)

        while current != len(message):
            if len(current_xor) != len(self.polynomial):
                current_xor += message[current]
                current += 1
            else:
                current_xor = self.module2division(current_xor, self.polynomial)

        if len(current_xor) == len(self.polynomial):
            current_xor = self.module2division(current_xor, self.polynomial)

        if '1' in current_xor:
            return False
        else:
            return True


    def decode(self, message):
        print("No se detectaron errores.")
        # Eliminar los últimos 32 bits del mensaje
        message = message[:-32]
        return binary_to_ascii(message)
    
class HammingDecoder:

    def decode(self, message):
        bits = list(message)
        n = len(bits)
        r = 0

        while (2 ** r) < (n + 1):
            r += 1

        error_pos = 0
        for i in range(r):
            pos = 2 ** i
            count = 0
            for j in range(1, n + 1):
                if j & pos and int(bits[j - 1]) == 1:
                    count += 1
            if count % 2 != 0:
                error_pos += pos

        if error_pos:
            print(f"Error detectado en la posición: {error_pos}")
            # Corrige el bit si hay un solo error
            bits[error_pos - 1] = '1' if bits[error_pos - 1] == '0' else '0'
            bits = ''.join(bits)
        else:
            print("No se detectaron errores.")
            bits = ''.join(bits)

        # Eliminar los bits de paridad
        mensaje = ''
        j = 0
        for i in range(1, n + 1):
            if i != 2 ** j:
                mensaje += bits[i - 1]
            else:
                j += 1

        return binary_to_ascii(mensaje)


def binary_to_ascii(binary_str):
    ascii_message = ''
    for i in range(0, len(binary_str), 8):
        byte = binary_str[i:i + 8]
        if len(byte) == 8:  # Asegurarse de que el byte tenga 8 bits
            ascii_message += chr(int(byte, 2))
    return ascii_message


def main():

    hamming = HammingDecoder()
    crc32 = CRC32Decoder()

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 8080))  # Bind to localhost and port 8080
    server_socket.listen(1)

    print("Esperando conexion...")
    conn, addr = server_socket.accept()
    print(f"Conectado por: {addr}")

    print("-------------------------------------")
    print("| ¿Qué algoritmo deseas utilizar?   |")
    print("|-----------------------------------|")
    print("| 1. Algoritmo Hamming              |")
    print("| 2. Algoritmo crc-32               |")
    print("-------------------------------------")
    
    opcion = input("Selecciona una opción (1 o 2): ")
    
    print("\nEsperando mensaje...")

    # Recibir datos
    data = conn.recv(1024).decode('utf-8')
    print(f"Mensaje Recibido: {data}")

    if opcion == '1':
        mensaje_decodificado = hamming.decode(data)
        print(f"Mensaje Decodificado: {mensaje_decodificado}")
    elif opcion == '2':
        error_detectado = crc32.detectError(data)
        if not error_detectado:
            print("Error de CRC detectado.")
        else:
            mensaje_decodificado = crc32.decode(data)
            print(f"Mensaje Decodificado: {mensaje_decodificado}")
    

    conn.close()
    server_socket.close()

if __name__ == "__main__":
    main()