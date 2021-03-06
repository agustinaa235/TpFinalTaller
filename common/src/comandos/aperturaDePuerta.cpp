#include "comandos/aperturaDePuerta.h"

AperturaDePuerta::AperturaDePuerta(int idJugador) : Comando(idJugador) {}

AperturaDePuerta::~AperturaDePuerta() {}

std::vector<Actualizacion *> AperturaDePuerta::ejecutar(EstadoJuego &estadoJuego) {
    Actualizacion *puerta = estadoJuego.abrirPuerta(idJugador);
    std::vector<Actualizacion *> actualizaciones;
    if (puerta != NULL) {
        actualizaciones.push_back(puerta);
    }
    return actualizaciones;
}

std::vector<char> AperturaDePuerta::serializar() {
    std::vector<char> informacion;
    std::vector<char> aux(4);
    aux = numberToCharArray(idJugador);
    informacion.insert(informacion.end(), aux.begin(), aux.end());
    aux = numberToCharArray(static_cast<int>(Accion::aperturaDePuerta));
    informacion.insert(informacion.end(), aux.begin(), aux.end());
    return informacion;
}

void AperturaDePuerta::deserializar(std::vector<char> &serializado) {
    std::vector<char> sub(4);
    int idx = 0;
    sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
    this->idJugador = charArrayToNumber(sub);
}
