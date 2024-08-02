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




string generateRandomString(int length) {
    const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, characters.size() - 1);

    string result;
    for (int i = 0; i < length; ++i) {
        result += characters[dis(gen)];
    }
    return result;
}

string createMessageVector(const string &original, const string &encoded, const string &noisy) {
    return original + "\n" + encoded + "\n" + noisy + "\n";
}

void runTests(int sock, CRC32Enconder &crc, HammingEncoder &hamming, int numTests, int maxLength, double noiseProbability, int delay, string choice) {
    for (int i = 0; i < numTests; ++i) {
        int length = rand() % maxLength + 1; // Longitud entre 1 y maxLength
        string message = generateRandomString(length);

        // Convertir el mensaje a su representación binaria ASCII
        string binaryASCII = stringToBinaryASCII(message);

        string encodedMessage;
        string noisyMessage;

        // Codificación según selección
        if (choice == "1") {
            // Codificación Hamming
            encodedMessage = hamming.encode(binaryASCII);
        } else if (choice == "2") {
            // Codificación CRC-32
            encodedMessage = crc.encodeCRC32(binaryASCII);
        } else {
            cerr << "Invalid choice" << endl;
            continue;
        }

        noisyMessage = applyNoise(encodedMessage, noiseProbability);

        string messageVector = createMessageVector(message, encodedMessage, noisyMessage);

        // Enviar el vector de mensaje al servidor
        send(sock, messageVector.c_str(), messageVector.size(), 0);
        cout << "Message vector sent to server" << endl;

        // Esperar para evitar saturar el servidor
        sleep(delay);
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Socket creation error" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Invalid address/ Address not supported" << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection Failed" << endl;
        return -1;
    }

    string choice;
    cout << "Encoding methods: " << endl;
    cout << "- [1] Hamming Encoding" << endl;
    cout << "- [2] CRC-32 Encoding" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    int numTests = 1000;          // Número de pruebas
    int maxLength = 5;          // Longitud máxima de los mensajes
    double noiseProbability = 0.001;
    int delay = 0.1;                // Tiempo de espera entre envíos para evitar saturación

    CRC32Enconder crc;
    HammingEncoder hamming;

    runTests(sock, crc, hamming, numTests, maxLength, noiseProbability, delay, choice);

    close(sock);

    return 0;
}