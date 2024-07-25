#include <bits/stdc++.h>
#include <iostream>
#include <math.h>
#include <stdio.h>


using namespace std;

// Polinomio generador para CRC-32
const uint32_t POLY = 0xEDB88320; // 1 0000 0100 1100 0001 0001 1101 1011 0111


// Función que convierte un número decimal a binario
string polyToBinaryString(uint32_t poly) {
    string binary;
    while (poly > 0) {
        binary = to_string(poly % 2) + binary;
        poly /= 2;
    }
    binary += "1";
    reverse(binary.begin(), binary.end()); // Invertir el string para obtener el número binario correcto
    return binary;
}

// Función que aplica XOR entre dos números
string xorOperation(string a, string b) {
    string result = "";

    // Se realiza la operación XOR bit a bit
    for (int i = 0; i < a.length(); i++) {
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

// Función que realiza la división de dos números binarios
// retorna el residuo de la división
string divide(string dividend, string divisor) {
    int divisorLength = divisor.length();   // Longitud del divisor
    int dividendLength = dividend.length(); // Longitud del dividendo
    
    int i = 0; // Contador

    string result = ""; // Residuo de la división
    string temp = dividend.substr(0, divisorLength); // Subcadena del dividendo

    // Se realiza la división
    while (divisorLength < dividendLength) {
        // Si el primer bit del dividendo es 0, se realiza un shift a la izquierda
        if (temp[0] == '0') {
            temp = temp.substr(1, temp.length()) + dividend[divisorLength];
        // Si el primer bit del dividendo es 1, se realiza un XOR con el divisor
        } else {
            temp = xorOperation(temp, divisor) + dividend[divisorLength];
        }
        divisorLength++;
    }

    // Se realiza la última operación XOR
    if (temp[0] == '1') {
        temp = xorOperation(temp, divisor);
    }

    // Se obtiene el residuo de la división
    result = temp.substr(1, temp.length());
    return result;
}

// Función que realiza la codificación CRC-32
string encodeCRC32(string message) {
    string polynomial_binary = polyToBinaryString(POLY);    // Polinomio generador en binario
    cout << "Polinomio generador: " << polynomial_binary << endl;
    string dividend = message;                              // Dividendo
    string divisor = polynomial_binary;                     // Divisor
    int dividendLength = dividend.length();                 // Longitud del dividendo

    // Se agrega ceros al dividendo
    dividend.append(32, '0');

    // Se invierten los primeros 32 bits el dividendo
    for (int i = 0; i < 32; i++) {
        if (dividend[i] == '1') {
            dividend[i] = '0';
        } else {
            dividend[i] = '1';
        }
    }

    // Se realiza la división
    string remainder = divide(dividend, divisor);
    cout << "Residuo: " << remainder << endl;

    // Se obtiene el mensaje codificado
    string encodedMessage = message + remainder;
    return encodedMessage;
}

// Funcion main 
int main() 
{   
    string message; // Mensaje a enviar
    string encodedMessage; // Mensaje codificado
    
    cout << "Emisor CRC-32" << endl;

    // Datos a enviar
    cout << "Ingrese los datos a enviar: ";
    cin >> message;

    // Se realiza la codificación CRC-32
    encodedMessage = encodeCRC32(message);

    // Se muestra el mensaje codificado
    cout << "Mensaje codificado: " << encodedMessage << endl;


	return 0; 
} 