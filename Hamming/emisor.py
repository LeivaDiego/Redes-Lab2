'''
#------------------------------------------------------------------------------------
# Pruebas emisor que no funciona
#------------------------------------------------------------------------------------

import socket
import random

# Capa de aplicación: Solicitar mensaje y algoritmo
def solicitar_mensaje():
    mensaje = input("Ingrese el mensaje a enviar: ")
    return mensaje

# Capa de presentación: Codificar mensaje a ASCII binario
def codificar_mensaje(mensaje):
    return ''.join(format(ord(char), '08b') for char in mensaje)

# Capa de enlace: Calcular paridad usando Hamming
def calcular_paridad(bits):
    n = len(bits)
    r = 0
    while (2**r) < (n + r + 1):
        r += 1

    hamming_code = [0] * (n + r)
    j = 0
    k = 1

    for i in range(1, len(hamming_code) + 1):
        if i == 2**j:
            j += 1
        else:
            hamming_code[i - 1] = int(bits[-k])
            k += 1

    for i in range(r):
        pos = 2**i
        count = 0
        for j in range(1, len(hamming_code) + 1):
            if j & pos and hamming_code[j - 1] == 1:
                count += 1
        hamming_code[pos - 1] = count % 2

    return ''.join(map(str, hamming_code))

# Capa de ruido: Aplicar ruido
def aplicar_ruido(bits, probabilidad_error=0.01):
    bits_ruido = ''
    for bit in bits:
        if random.random() < probabilidad_error:
            bits_ruido += '1' if bit == '0' else '0'
        else:
            bits_ruido += bit
    return bits_ruido

# Capa de transmisión: Enviar información
def enviar_informacion(mensaje_codificado):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('localhost', 65432))
        s.sendall(mensaje_codificado.encode())

def main():
    mensaje = solicitar_mensaje()
    mensaje_codificado = codificar_mensaje(mensaje)
    print(f"Mensaje codificado en binario ASCII: {mensaje_codificado}")  # Depuración
    mensaje_integridad = calcular_paridad(mensaje_codificado)
    print(f"Mensaje con paridad Hamming: {mensaje_integridad}")  # Depuración
    mensaje_con_ruido = aplicar_ruido(mensaje_integridad)
    print(f"Mensaje con ruido: {mensaje_con_ruido}")  # Depuración
    enviar_informacion(mensaje_con_ruido)
    print(f"Mensaje enviado: {mensaje_con_ruido}")

if __name__ == "__main__":
    main()              

#------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------
'''




'''
#------------------------------------------------------------------------------------
# Pruebas para enviar listas
#------------------------------------------------------------------------------------

import socket
import json

# Capa de aplicación: Solicitar lista
def solicitar_lista():
    lista = input("Ingresa la lista que deseas enviar: ")
    return lista

# Capa de transmisión: Enviar información
def enviar_informacion(cadena_lista):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('localhost', 65432))
        s.sendall(cadena_lista.encode())

def main():
    lista = solicitar_lista()
    print(f"Lista enviada: {lista}")  # Depuración
    enviar_informacion(lista)

if __name__ == "__main__":
    main()

#------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------
'''





#------------------------------------------------------------------------------------
# Pruebas para cadenas binarias
#------------------------------------------------------------------------------------
'''
import socket

# Capa de aplicación: Solicitar cadena binaria
def solicitar_cadena_binaria():
    return input("Ingresa la cadena binaria que deseas enviar: ")

# Capa de transmisión: Enviar información
def enviar_informacion(cadena_binaria):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('localhost', 65432))
        s.sendall(cadena_binaria.encode())

def main():
    cadena_binaria = solicitar_cadena_binaria()
    print(f"Cadena binaria enviada: {cadena_binaria}")  # Depuración
    enviar_informacion(cadena_binaria)

if __name__ == "__main__":
    main()
'''
#------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------



import socket

# Capa de aplicación: Solicitar cadena binaria
def solicitar_cadena_binaria():
    return input("Ingresa la cadena binaria que deseas enviar: ")

# Capa de transmisión: Enviar información
def enviar_informacion(cadena_binaria):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('localhost', 65432))
        s.sendall(cadena_binaria.encode())

def main():
    cadena_binaria = solicitar_cadena_binaria()
    print(f"Cadena binaria enviada: {cadena_binaria}")  # Depuración
    enviar_informacion(cadena_binaria)

if __name__ == "__main__":
    main()


