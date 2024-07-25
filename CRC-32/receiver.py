def binaryXor(a, b):
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


def module2division(dividend, divisor):
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
            block = binaryXor(divisor, block) + dividend[pick]
        # Si el primer bit del bloque es 0,
        # se usa un bloque de ceros para realizar la operacion XOR
        else:
            block = binaryXor('0' * pick, block) + dividend[pick]

        # Se incrementa el índice
        pick += 1

    # Se realiza el XOR de los ultimos bits
    if block[0] == '1':
        block = binaryXor(divisor, block)
    else:
        block = binaryXor('0' * pick, block)

    return block


def decodeCRC32(message, polynomial):
    current_xor = module2division(message[:len(polynomial)], polynomial)
    current = len(polynomial)

    while current < len(message):
        if len(current_xor) != len(polynomial):
            current_xor += message[current]
            current += 1
        else:
            current_xor = module2division(current_xor, polynomial)

    if len(current_xor) == len(polynomial):
        current_xor = module2division(current_xor, polynomial)

    if '1' in current_xor:
        return "False"
    else:
        return True
    

def main():
    print("Receptor de mensajes CRC-32\n")
    polynomial = '100000100110000010001110110110111'
    message = input("Ingrese el mensaje recibido: ")

    if decodeCRC32(message, polynomial):
        print("\nEl mensaje recibido no contiene errores.")
    else:
        print("\nEl mensaje recibido contiene errores.")
        print("Se recomienda solicitar nuevamente el mensaje.")


if __name__ == "__main__":
    main()