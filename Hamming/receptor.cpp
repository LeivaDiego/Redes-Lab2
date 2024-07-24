#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

vector<int> get_parity_bits(const vector<int>& encoded) {
    int r = 0;
    int n = encoded.size();
    
    while (pow(2, r) < (n + 1)) {
        r++;
    }
    
    vector<int> parity_bits(r, 0);
    
    for (int i = 0; i < r; i++) {
        int pos = pow(2, i);
        int count = 0;
        
        for (int j = 1; j <= n; j++) {
            if (j & pos) {
                count += encoded[j - 1];
            }
        }
        
        parity_bits[i] = count % 2;
    }
    
    return parity_bits;
}

int main() {
    string input;
    cout << "Ingrese un mensaje codificado en binario: ";
    cin >> input;
    
    vector<int> encoded(input.length());
    
    for (size_t i = 0; i < input.length(); i++) {
        encoded[i] = input[i] - '0';
    }
    
    vector<int> parity_bits = get_parity_bits(encoded);
    int error_position = 0;
    
    for (size_t i = 0; i < parity_bits.size(); i++) {
        if (parity_bits[i] != 0) {
            error_position += pow(2, i);
        }
    }
    
    if (error_position == 0) {
        cout << "No se detectaron errores. Mensaje original: ";
        for (size_t i = 0; i < encoded.size(); i++) {
            cout << encoded[i];
        }
        cout << endl;
    } else {
        cout << "Error detectado en la posicion: " << error_position << endl;
        encoded[error_position - 1] = !encoded[error_position - 1];
        cout << "Mensaje corregido: ";
        for (size_t i = 0; i < encoded.size(); i++) {
            cout << encoded[i];
        }
        cout << endl;
    }
    
    return 0;
}
