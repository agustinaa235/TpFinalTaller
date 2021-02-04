#include "../include/thclient.h"
#include "socket.h"
#include <string>
#include <utility>
#include "comandos/crearPartida.h"
#include "comandos/unirseAPartida.h"

#define BUF_TAM 50
#define ERROR -1
#define SOCKET_CERRADO 0

ThClient::ThClient(Socket&& un_socket, ManejadorPartidas *manejadorDePartidas) :
        protocolo(new Protocolo(std::move(un_socket))),
        keep_talking(true),
        is_running(true),
        manejadorDePartidas(manejadorDePartidas) {}

ThClient::~ThClient() {
    this->join();
}

void ThClient::stop() {
    this->keep_talking = false;
    this->protocolo->cerrar();
}

void ThClient::procesar_pedido() {
  std::vector<char> serializado = this->protocolo->recibir();
  bool resultado = false;
  std::cout << "recibi: " << serializado.size() << "\n";

  if (serializado[0] == static_cast<int>(Accion::unirseAPartida)){
    UnirseAPartida unirseAPartida;
    unirseAPartida.deserializar(serializado);
    resultado = this->manejadorDePartidas->agregarClienteAPartida(unirseAPartida.getNombreJugador(),unirseAPartida.getNombrePartida());
  }else{
      CrearPartida crearPartida;
      crearPartida.deserializar(serializado);
      resultado = this->manejadorDePartidas->crearPartida(crearPartida.getNombreJugador(),crearPartida.getCantJugadores(),
                                      crearPartida.getNombrePartida(),crearPartida.getRutaArchivo());
  }
  std::vector<char> res;
  res.push_back((char)resultado);
  protocolo->enviar(res);
}

bool ThClient::is_dead() {
    return !this->is_running;
}

void ThClient::run() {
    std::cerr << "envio partidas\n";
    std::vector<char> partidas = this->manejadorDePartidas->serializar();
    for (int i = 0; i < partidas.size(); i++) {
        std::cerr << partidas[i];
    }
    std::cerr << "\nfin envio partidas";
  //  reinterpret_cast<char *>(partidas.data()), partidas.size()
    this->protocolo->enviar(partidas);
    while (this->keep_talking) {
    procesar_pedido();
/*
        std::string mensaje = this->mensaje_cliente.str();
        if (mensaje.size() != 0) {
            mensaje.erase(mensaje.size() - 1);
        }


        this->keep_talking = false;*/
      //  this->keep_talking = false;
    }
    this->protocolo->cerrar();
    is_running = false;
}