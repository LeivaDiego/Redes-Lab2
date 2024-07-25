#include <bits/stdc++.h>
using namespace std;

// Polinomio generador para CRC-32
const uint32_t POLY = 0xEDB88320;

// Función que convierte un número decimal a binario
string decimalToBinary(uint32_t poly) {
    string binary;
    while (poly > 0) {
        binary = to_string(poly % 2) + binary;
        poly /= 2;
    }
    reverse(binary.begin(), binary.end()); // Invertir el string para obtener el número binario correcto
    return binary;
}

// Función main
int main() {
    cout << "Polinomio generador: " << hex << POLY << endl;
    cout << "Polinomio generador en  decimal: " << dec << POLY << endl;
    cout << "Polinomio generador en  binario: " << decimalToBinary(POLY) << endl;
    cout << "Polinomio generador de internet: 00000100110000010001110110110111" << endl;
    return 0;
}