import socket

class CRC32Decoder:
    def __init__(self):
        self.polynomial = "100000100110000010001110110110111"

    def binaryXor(self, a, b):
        result = []
        for i in range(1, len(b)):
            if a[i] == b[i]:
                result.append('0')
            else:
                result.append('1')
        return ''.join(result)

    def module2division(self, dividend, divisor):
        pick = len(divisor)
        block = dividend[0: pick]
        n = len(dividend)
        while pick < n:
            if block[0] == '1':
                block = self.binaryXor(divisor, block) + dividend[pick]
            else:
                block = self.binaryXor('0' * pick, block) + dividend[pick]
            pick += 1
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
        return '1' not in current_xor

    def decode(self, message):
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
            bits[error_pos - 1] = '1' if bits[error_pos - 1] == '0' else '0'
            bits = ''.join(bits)
        else:
            print("No se detectaron errores.")
            bits = ''.join(bits)
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
        if len(byte) == 8:
            ascii_message += chr(int(byte, 2))
    return ascii_message

def main():
    hamming = HammingDecoder()
    crc32 = CRC32Decoder()

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 8080))
    server_socket.listen(1)

    print("Esperando conexion...")
    conn, addr = server_socket.accept()
    print(f"Conectado por: {addr}")

    while True:
        data = conn.recv(1024).decode('utf-8')
        if not data:
            break

        parts = data.split(',')
        if len(parts) == 5:
            original_message = parts[0]
            encoded_message = parts[1]
            noisy_message = parts[2]
            encoder_type = parts[3]
            has_error = parts[4] == '1'

            print(f"Mensaje original: {original_message}")
            print(f"Mensaje codificado: {encoded_message}")
            print(f"Mensaje con ruido: {noisy_message}")
            print(f"Tipo de codificación: {'Hamming' if encoder_type == '1' else 'CRC-32'}")
            print(f"Tiene error: {has_error}")

            if encoder_type == '1':
                decoded_message = hamming.decode(noisy_message)
                print(f"Mensaje decodificado: {decoded_message}\n")
            else:
                if crc32.detectError(noisy_message):
                    decoded_message = crc32.decode(noisy_message)
                    print(f"Mensaje decodificado: {decoded_message}\n")
                else:
                    print("Error de CRC detectado.\n")

    conn.close()
    server_socket.close()

if __name__ == "__main__":
    main()
