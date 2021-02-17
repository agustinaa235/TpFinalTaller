#include "../include/manejadorPartidas.h"

#include "map.h"
#include "protected_queue.h"
#include "comandos/comando.h"
#include "map_translator.h"
#include <yaml-cpp/yaml.h>

ManejadorPartidas::ManejadorPartidas(std::map<std::string,std::string>& mapas) :
        partidas(),
        esta_corriendo(true),
        mapas(mapas) {}

void ManejadorPartidas::cerrarPartidas() {
  this->eliminarPartidasTerminadas();
}

void ManejadorPartidas::agregarMapa(std::string nombreMapa, std::string archivoMapa) {
    this->mapas.insert(std::make_pair(nombreMapa, archivoMapa));
}

Map ManejadorPartidas::buscarMapa(std::string &archivoMapa) {
  std::string ruta = this->mapas.at(archivoMapa);
  return MapTranslator::yamlToMap(YAML::LoadFile(ruta));
}

int ManejadorPartidas::crearPartida(std::string &nombreJugador,int &cant_jugadores,
                                    std::string &nombre_partida,std::string &archivoMapa,
                                    Protocolo* protocolo, int& screenWidth) {
    int idCliente = -1;
    if (partidas.count(nombre_partida) > 0) {
        return idCliente;
    } else {
        Map mapa = this->buscarMapa(archivoMapa);
        mapa.setLadoCelda(800);
        std::cerr << "/* MANEJADOR DE PARTIDAS SETEO LADO DE LA CELDA CON */" << screenWidth <<  '\n';
        Servidor *servidor = new Servidor(mapa, cant_jugadores);
        ManejadorCliente* cliente = new ManejadorCliente(servidor->obtenerColaEventos(),protocolo, idCliente);//cheq
        servidor->agregarCliente(nombreJugador, cliente, idCliente);
        this->partidas.insert({nombre_partida, servidor});
        return idCliente;
    }
}

int ManejadorPartidas::agregarClienteAPartida(std::string &nombreJugador,
                                               std::string &nombre_partida, Protocolo* protocolo) {

    Servidor *servidor = this->partidas.at(nombre_partida);
    int idJugador = -1;
    if (servidor->yaArranco()) {
        std::cout << "ya arranco la partida\n";
        return idJugador;
    } else {
        ManejadorCliente* cliente = new ManejadorCliente(servidor->obtenerColaEventos(),protocolo, idJugador);
        servidor->agregarCliente(nombreJugador, cliente, idJugador);
        return idJugador;
    }
}

void ManejadorPartidas::eliminarPartidasTerminadas() {
    std::map<std::string, Servidor *>::iterator it;
    for (it = this->partidas.begin(); it != this->partidas.end();++it) {
        if (it->second->terminoPartida()) {
            it->second->joinClientes();
            it->second->join();
            delete it->second;
            it = this->partidas.erase(it);
        } else {
            ++it;
        }
    }
}

void ManejadorPartidas::run() {
    while (this->esta_corriendo) {
        /*deberia bloquearse esperando partidas*/
        this->eliminarPartidasTerminadas();
        std::chrono::milliseconds duration(100);
        std::this_thread::sleep_for(duration);
        this->esta_corriendo = this->partidas.size() != 0;
    }

}

ManejadorPartidas::~ManejadorPartidas() {

}

std::vector<char> ManejadorPartidas::serializar() {
    std::vector<char> informacion;
    std::vector<char> informacionPartidas;
    std::map<std::string, Servidor *>::iterator it;
    int i = 0;
    for (it = this->partidas.begin(); it != this->partidas.end(); ++it) {
        std::pair<std::string, Servidor *> pair = *it;
        if (!pair.second->terminoPartida() && !pair.second->yaArranco()) {
            i++;
            std::vector<char> sizeNombre = numberToCharArray(pair.first.size());
            informacionPartidas.insert(informacionPartidas.end(), sizeNombre.begin(), sizeNombre.end());
            informacionPartidas.insert(informacionPartidas.end(), pair.first.begin(), pair.first.end());
            std::vector<char> partidaSerializada = pair.second->serializar();
            informacionPartidas.insert(informacionPartidas.end(), partidaSerializada.begin(), partidaSerializada.end());
        }
    }
    std::vector<char> sizePartidas = numberToCharArray(i);
    informacion.insert(informacion.end(), sizePartidas.begin(), sizePartidas.end());
    if (i > 0) {
        informacion.insert(informacion.end(), informacionPartidas.begin(), informacionPartidas.end());
    }
    std::cout << "TAMANIO DE LAS PARTIDASSSSSSS: " << informacion.size() << std::endl;
    return informacion;
}
