#include "thclient.h"
#include "socket.h"
#include <string>
#include <utility>

#define BUF_TAM 50
#define ERROR -1
#define SOCKET_CERRADO 0

ThClient::ThClient(Socket un_socket) :
        socket(std::move(un_socket)),
        keep_talking(true),
        is_running(true) {}

ThClient::~ThClient() {
    this->join();
}

void ThClient::stop() {
    this->keep_talking = false;
    this->socket.cerrar();
}

void ThClient::procesar_pedido() {
    char buffer[BUF_TAM];
    for (int i = 0; i < BUF_TAM; i++) {
        buffer[i] = 0;
    }

    bool socket_cerrado = false;
    bool error_al_recibir = false;

    while (!socket_cerrado && !error_al_recibir) {
        int cant_recibida = this->socket.recibir(buffer, BUF_TAM);
        if (cant_recibida == ERROR) {
            error_al_recibir = true;
        } else if (cant_recibida == SOCKET_CERRADO) {
            socket_cerrado = true;
        } else {
            this->mensaje_cliente.write(buffer, cant_recibida);
        }
    }
    this->socket.apagar_lectura();
}

bool ThClient::is_dead() {
    return !this->is_running;
}

void ThClient::run() {
    while (this->keep_talking) {
        procesar_pedido();

        std::string mensaje = this->mensaje_cliente.str();
        if (mensaje.size() != 0) {
            mensaje.erase(mensaje.size() - 1);
        }

        this->socket.apagar_escritura();
        this->mensaje_cliente.clear();

        this->keep_talking = false;
    }
    this->socket.cerrar();
    is_running = false;
}
