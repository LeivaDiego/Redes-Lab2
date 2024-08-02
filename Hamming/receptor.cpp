/**
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

// Inicializar Winsock
void inicializar_winsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << endl;
        exit(1);
    }
}

// Capa de presentación: Decodificar mensaje
string decodificar_mensaje(const string& binario) {
    string mensaje = "";
    cout << "Decodificando: " << binario << endl; // Mensaje de depuración
    for (size_t i = 0; i < binario.size(); i += 8) {
        bitset<8> bits(binario.substr(i, 8));
        mensaje += char(bits.to_ulong());
    }
    return mensaje;
}

// Capa de enlace: Verificar integridad y corregir mensaje
string verificar_integridad(const string& binario) {
    int n = binario.size();
    int r = 0;
    while ((1 << r) < (n + 1)) {
        r++;
    }
    
    vector<int> hamming_code(n);
    for (int i = 0; i < n; i++) {
        hamming_code[i] = binario[i] - '0';
    }
    
    int error_position = 0;
    for (int i = 0; i < r; i++) {
        int pos = 1 << i;
        int count = 0;
        for (int j = 1; j <= n; j++) {
            if (j & pos && hamming_code[j - 1] == 1) {
                count++;
            }
        }
        if (count % 2 != 0) {
            error_position += pos;
        }
    }

    if (error_position != 0) {
        cout << "Error detectado en la posicion: " << error_position << endl;
        hamming_code[error_position - 1] = !hamming_code[error_position - 1];
    }

    string resultado = "";
    int j = 0;
    for (int i = 1; i <= n; i++) {
        if (i != (1 << j)) {
            resultado += to_string(hamming_code[i - 1]);
        } else {
            j++;
        }
    }
    cout << "Resultado despues de verificar integridad: " << resultado << endl; // Mensaje de depuración
    return resultado;
}

// Capa de transmisión: Recibir información
string recibir_informacion() {
    inicializar_winsock();
    
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        cerr << "Socket failed: " << WSAGetLastError() << endl;
        WSACleanup();
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(65432);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(server_fd);
        WSACleanup();
        exit(1);
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        cerr << "Listen failed: " << WSAGetLastError() << endl;
        closesocket(server_fd);
        WSACleanup();
        exit(1);
    }

    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket == INVALID_SOCKET) {
        cerr << "Accept failed: " << WSAGetLastError() << endl;
        closesocket(server_fd);
        WSACleanup();
        exit(1);
    }

    int valread = recv(new_socket, buffer, 1024, 0);
    if (valread == SOCKET_ERROR) {
        cerr << "Recv failed: " << WSAGetLastError() << endl;
    }

    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();

    return string(buffer, valread);
}

int main() {
    string mensaje_con_ruido = recibir_informacion();
    cout << "Mensaje recibido con ruido: " << mensaje_con_ruido << endl; // Mensaje de depuración
    string mensaje_integridad = verificar_integridad(mensaje_con_ruido);
    string mensaje = decodificar_mensaje(mensaje_integridad);
    cout << "Mensaje decodificado: " << mensaje << endl; // Mensaje de depuración final
    return 0;
}
**/

#include <iostream>
#include <string>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

// Inicializar Winsock
void inicializar_winsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << endl;
        exit(1);
    }
}

// Capa de transmisión: Recibir información
string recibir_informacion() {
    inicializar_winsock();
    
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        cerr << "Socket failed: " << WSAGetLastError() << endl;
        WSACleanup();
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(65432);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(server_fd);
        WSACleanup();
        exit(1);
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        cerr << "Listen failed: " << WSAGetLastError() << endl;
        closesocket(server_fd);
        WSACleanup();
        exit(1);
    }

    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket == INVALID_SOCKET) {
        cerr << "Accept failed: " << WSAGetLastError() << endl;
        closesocket(server_fd);
        WSACleanup();
        exit(1);
    }

    int valread = recv(new_socket, buffer, 1024, 0);
    if (valread == SOCKET_ERROR) {
        cerr << "Recv failed: " << WSAGetLastError() << endl;
    }

    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();

    return string(buffer, valread);
}

int main() {
    string cadena_lista = recibir_informacion();
    cout << "Cadena JSON recibida: " << cadena_lista << endl;  // Depuración
    return 0;
}



