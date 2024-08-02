def calculate_parity_bits(data):
    n = len(data)
    r = 0
    
    while (2**r) < (n + r + 1):
        r += 1
        
    parity_bits = [0] * (r + n)
    j = 0
    k = 1
    
    for i in range(1, len(parity_bits) + 1):
        if i == 2**j:
            j += 1
        else:
            parity_bits[i - 1] = int(data[-k])
            k += 1
            
    for i in range(r):
        pos = 2**i
        count = 0
        for j in range(1, len(parity_bits) + 1):
            if j & pos and parity_bits[j - 1] == 1:
                count += 1
        parity_bits[pos - 1] = count % 2
        
    return ''.join(map(str, parity_bits))

def main():
    data = input("Ingrese un mensaje en binario: ")
    encoded_message = calculate_parity_bits(data)
    print(f"Mensaje codificado: {encoded_message}")

if __name__ == "__main__":
    main()