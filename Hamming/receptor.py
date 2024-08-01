import socket

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

# Capa de enlace: Verificar y corregir errores utilizando código Hamming
def verificar_y_corregir_hamming(bits):
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
        print(f"Error detectado en la posición: {error_pos}")
        bits = list(bits)
        bits[error_pos - 1] = '1' if bits[error_pos - 1] == '0' else '0'
        bits = ''.join(bits)
        print(f"Bits corregidos: {bits}")
    else:
        print("No se detectaron errores")
    
    return bits

# Capa de presentación: Decodificar cadena binaria
def decodificar_mensaje(bits):
    m = len(bits)
    r = 0
    
    while (2**r) < (m + 1):
        r += 1
        
    mensaje = ''
    j = 0
    
    for i in range(1, m + 1):
        if i != 2**j:
            mensaje += bits[i - 1]
        else:
            j += 1
    
    return mensaje

# Convertir la cadena binaria decodificada en caracteres ASCII
def convertir_binario_a_ascii(mensaje_binario):
    mensaje = ''
    for i in range(0, len(mensaje_binario), 8):
        byte = mensaje_binario[i:i+8]
        caracter = chr(int(byte, 2))
        mensaje += caracter
    return mensaje

def main():
    cadena_codificada = recibir_informacion()
    print(f"Cadena binaria recibida: {cadena_codificada}")  # Depuración
    bits_corregidos = verificar_y_corregir_hamming(cadena_codificada)
    mensaje_binario = decodificar_mensaje(bits_corregidos)
    print(f"Mensaje binario decodificado: {mensaje_binario}")  # Depuración
    mensaje = convertir_binario_a_ascii(mensaje_binario)
    print(f"Mensaje decodificado: {mensaje}")

if __name__ == "__main__":
    main()
