#include <bits/stdc++.h>

using namespace std;

// Polinomio generador para CRC-32
const string POLY = "100000100110000010001110110110111"; //  0xEDB88320


// Funcion que aplica XOR entre dos strings
string binaryXor(string a, string b) {
    string result = "";

    int n = b.length(); // Longitud de la cadena b

    // Se realiza el XOR bit a bit
    for (int i = 1; i < n; i++) {
        // Si los bits son iguales, se agrega un 0 al resultado
        if (a[i] == b[i]) {
            result += "0";
        // Si los bits son diferentes, se agrega un 1 al resultado
        } else {
            result += "1";
        }
    }
    return result;
}


// Funcion que realiza la division modulo 2
string modulo2division(string dividend, string divisor) {
    // Se obtiene la cantidad de bits a aplicar XOR a la vez
    int pick = divisor.length();

    // Se divide el mensaje en partes de tamaño pick
    string block = dividend.substr(0, pick);

    // Se obtiene la longitud del mensaje
    int n = dividend.length();

    // Se realiza la division modulo 2
    while (pick < n) {
        // Si el primer bit del bloque es 1, 
        // se reemplaza el bloque por el resultado de la operacion XOR
        if (block[0] == '1') {
            block = binaryXor(divisor, block) + dividend[pick];
        }
        // Si el primer bit del bloque es 0,
        // se usa un bloque de ceros para realizar la operacion XOR
        else {
            block = binaryXor(string(pick, '0'), block) + dividend[pick];
        }
        // Se incrementa el contador
        pick++;
    }

    // Para los ultimos bits del mensaje se realiza la operacion XOR
    if (block[0] == '1') {
        block = binaryXor(divisor, block);
    } else {
        block = binaryXor(string(pick, '0'), block);
    }

    return block;
}


// Funcion que realiza la codificacion CRC-32
string encodeCRC32(string message, string polynomial) {
    int n = polynomial.length(); // Longitud del polinomio generador

    // Se agrega n-1 ceros al final del mensaje
    string augmented_message = message + string(n - 1, '0');

    // Se realiza la division modulo 2 para obtener el residuo
    string remainder = modulo2division(augmented_message, polynomial);
    cout << "Residuo: " << remainder << endl;

    // Se obtiene el mensaje codificado
    string encoded_message = message + remainder;

    // Se retorna el mensaje codificado y el residuo
    return encoded_message;
}


// Funcion principal
int main() 
{   
    string message; // Mensaje a enviar
    string encodedMessage; // Mensaje codificado
    
    cout << "Emisor CRC-32" << endl;
    // Se muestra el polinomio generador
    cout << "Polinomio generador: " << POLY << endl;
    cout << endl;

    // Datos a enviar
    cout << "Ingrese los datos a enviar: ";
    cin >> message;
    cout << endl;

    // Se realiza la codificación CRC-32
    encodedMessage = encodeCRC32(message, POLY);
    cout << endl;
    
    // Se muestra el mensaje codificado
    cout << "Mensaje codificado: " << encodedMessage << endl;

	return 0; 
} 