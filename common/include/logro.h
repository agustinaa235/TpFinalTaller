#ifndef LOGRO_H
#define LOGRO_H

#include "iserializable.h"

/*
Al finalizar la partida se deberá mostrar un top 5 de los jugadores con más enemigos matadas, más puntos
por tesoros y más balas disparadas
*/

class Logro : public ISerializable {
public:
    bool operator>(Logro &logro);

    bool operator==(Logro &logro);

    void aniadirBalasDisparadas(int cantidadDeBalas);

    void aniadirPuntosPorTesoro(int puntos);

    void aniadirEnemigosMatados(int cantidadDeEnemigos);

    std::vector<char> serializar() override;
    void deserializar(std::vector<char> &serializado) override;
    int obtenerPuntosTotales();

private:
    int enemigosMatados = 0;
    int puntosTotalesPorTesoros = 0; //no se si se acumula o deberia compararse uno por uno, sino decision de implementacion: se acumula
    int balasDisparadas = 0;
};

#endif
