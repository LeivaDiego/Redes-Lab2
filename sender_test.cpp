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

// Clase que realiza la codificación CRC-32
class CRC32Enconder {
private:
    const string POLY = "100000100110000010001110110110111"; //  0xEDB88320

    string binaryXor(string a, string b) {
        string result = "";
        int n = b.length(); // Longitud de la cadena b

        for (int i = 1; i < n; i++) {
            result += (a[i] == b[i]) ? "0" : "1";
        }
        return result;
    }

    string modulo2division(string dividend, string divisor) {
        int pick = divisor.length();
        string block = dividend.substr(0, pick);
        int n = dividend.length();

        while (pick < n) {
            block = (block[0] == '1') ? binaryXor(divisor, block) + dividend[pick] : binaryXor(string(pick, '0'), block) + dividend[pick];
            pick++;
        }

        block = (block[0] == '1') ? binaryXor(divisor, block) : binaryXor(string(pick, '0'), block);
        return block;
    }

public:
    string encodeCRC32(string message) {
        int n = POLY.length();
        string augmented_message = message + string(n - 1, '0');
        string remainder = modulo2division(augmented_message, POLY);
        string encoded_message = message + remainder;
        return encoded_message;
    }
};

// Clase que realiza la codificación Hamming
class HammingEncoder {
public:
    string encode(string data) {
        int m = data.size();
        int r = 0;
        int power = 1;

        while (power < (m + r + 1)) {
            r++;
            power *= 2;
        }

        string msg(m + r + 1, '0'); // Inicializar con '0'
        int curr = 0;
        for (int i = 1; i <= m + r; i++) {
            if (i & (i - 1)) {
                msg[i] = data[curr++];
            }
        }

        setRedundantBits(msg, m, r);
        return msg.substr(1);
    }

private:
    void setRedundantBits(string &msg, int m, int r) {
        for (int i = 0; i < r; i++) {
            int pos = (1 << i);
            int count = 0;

            for (int j = pos; j <= m + r; j++) {
                if (j & pos) {
                    if (msg[j] == '1') count++;
                }
            }

            msg[pos] = (count & 1) ? '1' : '0';
        }
    }
};

// Funcion que convierte un string a su representacion binaria ASCII
string stringToBinaryASCII(const string &input) {
    string result;
    for (char c : input) {
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

    for (int i = 0; i < msg_length; ++i) {
        if (dist(gen)) {
            message[i] = (message[i] == '0') ? '1' : '0';
        }
    }
    return message;
}

// Funcion para generar un string aleatorio de longitud n
string generateRandomString(int length) {
    const string CHARACTERS = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, CHARACTERS.size() - 1);

    string result;
    for (int i = 0; i < length; ++i) {
        result += CHARACTERS[dis(gen)];
    }
    return result;
}

int main() {
    // Parametros configurables
    int numMessages = 10000; // Cantidad de mensajes a enviar
    int messageLength = 30; // Longitud de cada mensaje
    string choice = "2"; // "1" para Hamming, "2" para CRC-32
    double probability = 0.001; // Probabilidad de ruido

    // Creacion de socket
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Error en creacion de Socket" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); // Puerto de escucha

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Direccion invalida o no soportada" << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Conexion Fallida" << endl;
        return -1;
    }

    CRC32Enconder crc;
    HammingEncoder hamming;

    for (int i = 0; i < numMessages; ++i) {
        string message = generateRandomString(messageLength);
        string binaryASCII = stringToBinaryASCII(message);
        string encodedMessage, noisyMessage;

        if (choice == "1") {
            encodedMessage = hamming.encode(binaryASCII);
        } else if (choice == "2") {
            encodedMessage = crc.encodeCRC32(binaryASCII);
        } else {
            cerr << "Opcion no valida" << endl;
            continue;
        }

        noisyMessage = applyNoise(encodedMessage, probability);
        bool hasError = (encodedMessage != noisyMessage);

        string messageInfo = message + "," + encodedMessage + "," + noisyMessage + "," + choice + "," + (hasError ? "1" : "0");
        send(sock, messageInfo.c_str(), messageInfo.size(), 0);
        cout << "Mensaje enviado al servidor: " << message << endl;
        usleep(1000); // Pausa de 1 ms entre mensajes para evitar problemas de buffer
    }

    close(sock);
    return 0;
}
