#include <iostream>
#include <string>
#include <bitset>
#include <random>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

using namespace std;

// Clase que realiza la codificacion CRC-32
class CRC32Enconder {
private:
    // Polinomio generador CRC-32
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

public:
    // Funcion que realiza la codificacion CRC-32
    string encodeCRC32(string message) {
        int n = POLY.length(); // Longitud del polinomio generador

        // Se agrega n-1 ceros al final del mensaje
        string augmented_message = message + string(n - 1, '0');

        // Se realiza la division modulo 2 para obtener el residuo
        string remainder = modulo2division(augmented_message, POLY);

        // Se obtiene el mensaje codificado
        string encoded_message = message + remainder;

        // Se retorna el mensaje codificado
        return encoded_message;
    }

};

// Clase que realiza la codificacion Hamming
class HammingEncoder {
public:
    // Funcion que realiza la codificacion Hamming
    string encode(string data) {
        int m = data.size();
        int r = 0;
        int power = 1;

        // Encontrar el numero de bits redundantes
        while (power < (m + r + 1)) {
            r++;
            power *= 2;
        }

        // Asignar memoria para el mensaje con datos y bits redundantes
        string msg(m + r + 1, '0'); // Inicializar con '0'

        int curr = 0;
        // Inicializar el mensaje con bits de datos y '0' para bits redundantes
        for (int i = 1; i <= m + r; i++) {
            if (i & (i - 1)) {
                msg[i] = data[curr++];
            }
        }

        // Calcular la paridad para los bits redundantes
        setRedundantBits(msg, m, r);

        // Retornar el mensaje codificado
        return msg.substr(1);
    }

private:
    // Funcion que calcula los bits redundantes
    void setRedundantBits(string &msg, int m, int r) {
        // Calcular la paridad para los bits redundantes
        // basado en paridad par
        for (int i = 0; i < r; i++) {
            int pos = (1 << i);
            int count = 0;

            for (int j = pos; j <= m + r; j++) {
                if (j & pos) {
                    if (msg[j] == '1') count++;
                }
            }

            // Asignar el bit redundante
            msg[pos] = (count & 1) ? '1' : '0';
        }
    }
};

// Funcion que convierte un string a su representacion binaria ASCII
string stringToBinaryASCII(const string &input) {
    string result;
    for (char c : input) {
        // Convertir el caracter a su representacion binaria ASCII de 8 bits
        // y agregarlo al resultado
        result += bitset<8>(c).to_string();
    }
    return result;
}

// Funcion que aplica ruido a un string codificado con probabilidad p
string applyNoise(string message, double probability) {
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution dist(probability);

    int msg_length = message.length();

    // Iterar por cada bit del mensaje
    for (int i = 0; i < msg_length; ++i) {
        if (dist(gen)) {
            // Invertir el bit
            message[i] = (message[i] == '0') ? '1' : '0';
        }
    }

    // Retornar el mensaje con ruido
    return message;
}


int main() {

    // Creacion de socket
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Socket creation error" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); // Puerto de escucha

    // Convertir la direccion IP a binario
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Invalid address/ Address not supported" << endl;
        return -1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection Failed" << endl;
        return -1;
    }

    
    string choice, message, encodedMessage, noisyMessage;
    
    double probability = 0.001;

    CRC32Enconder crc;
    HammingEncoder hamming;

    cout << "Encoding methods: " << endl ;
    cout << "- [1] Hamming Encoding" << endl;
    cout << "- [2] CRC-32 Encoding" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    
    cout << "Enter message to encode: ";
    cin.ignore(); // Ignore newline character left in buffer
    getline(cin, message);

    // Convertir el mensaje a su representacion binaria ASCII
    string binaryASCII = stringToBinaryASCII(message);
    
    cout << endl;
    
    if (choice == "1") {
        // Se realiza la codificación Hamming
        cout << "Codificacion con Hamming:" << endl;
        encodedMessage = hamming.encode(binaryASCII);
        // Se muestra el mensaje codificado
        cout << "Encoded message: " << encodedMessage << endl;
        // Aplicar ruido a un mensaje codificado
        noisyMessage = applyNoise(encodedMessage, probability);
        cout << "Noisy message: " << noisyMessage << endl;
        // Enviar mensaje codificado al servidor
        send(sock, noisyMessage.c_str(), noisyMessage.size(), 0);
        cout << "Message sent to server" << endl;

    } else if (choice == "2") {
        // Se realiza la codificación CRC-32
        cout << "Codificacion con CRC-32:" << endl;
        encodedMessage = crc.encodeCRC32(binaryASCII);
        // Se muestra el mensaje codificado
        cout << "Encoded message: " << encodedMessage << endl;
        // Aplicar ruido a un mensaje codificado
        noisyMessage = applyNoise(encodedMessage, probability);
        cout << "Noisy message: " << noisyMessage << endl;
        // Enviar mensaje codificado al servidor
        send(sock, noisyMessage.c_str(), noisyMessage.size(), 0);
        cout << "Message sent to server" << endl;


    } else {
        cout << "Invalid choice" << endl;
    }

    close(sock);

    return 0;
}