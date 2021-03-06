#include "../include/map.h"
#include "armas/lanzacohetes.h"
#include <algorithm>

#define ANGULO_DEFAULT 0

void Map::agregarArma(Posicion &posicion, Arma *arma) {}

void Map::sacarDelMapa(Posicion &posicion) {
    this->contenedorDeElementos->sacarElementoDePosicion(posicion);
}

Map::Map(unsigned rowSize, unsigned colSize, int anchoPantalla, ConfiguracionPartida& configuracion):
configuracion(configuracion) {
    if (rowSize < 1 || colSize < 1) {
        throw std::runtime_error("Invalid map");
    }

    this->contenedorDeElementos = new ContenedorDeElementos(configuracion);
    this->rowSize = rowSize;
    this->colSize = colSize;
    map.resize(rowSize);
    for (unsigned i = 0; i < rowSize; i++) {
        map[i].resize(colSize, ObjetosJuego::obtenerTipoPorNombre("noItem"));
    }
    setLadoCelda(anchoPantalla);
}

unsigned Map::getRowSize() const {
    return this->rowSize;
}

unsigned Map::getColSize() const {
    return this->colSize;
}

int Map::crearIdValido() {
    int idValido = this->generadorDeId;
    this->generadorDeId++;
    return idValido;
}

void Map::crearElementoPosicionable(const unsigned rowNumber, const unsigned colNumber,
                                    Type value) {
    srand(time(NULL));
    int posElementox = 0;
    int posElementoy = 0;
    if (rowNumber == 0) {
        posElementox = rand() % ladoCelda;
    } else {
        posElementox = (ladoCelda * (rowNumber - 1) + rand() % ladoCelda);
    }
    if (colNumber == 0) {
        posElementoy = rand() % ladoCelda;
    } else {
        posElementoy = ladoCelda * (colNumber - 1) + rand() % ladoCelda;
    }
    Posicion posicion = Posicion(posElementox, posElementoy, ANGULO_DEFAULT);
    if (value.getName() == "comida") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(new Comida(posicion, idValido,configuracion.getPuntosVidaComida(),configuracion.getVidaMax()));
    } else if (value.getName() == "sangre") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(new Sangre(posicion, idValido,configuracion.getVidaMax(),configuracion.getPuntosDeVidaSangre()));
    } else if (value.getName() == "kitsMedicos") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(new KitsMedicos(posicion, idValido,configuracion.getVidaMax(),configuracion.getPuntosDeVidaKits()));
    } else if (value.getName() == "balas") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(new Balas(posicion, configuracion.getBalasEnMapa(), idValido, configuracion.getMaxBalas()));
    } else if (value.getName() == "ametralladora") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(new Ametralladora(posicion, idValido,configuracion));
    } else if (value.getName() == "canionDeCadena") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(new CanionDeCadena(posicion, idValido,configuracion));
    } else if (value.getName() == "lanzaCohetes") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(new LanzaCohetes(posicion, idValido,configuracion));
    } else if (value.getName() == "cruz") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(
                new Tesoro(idValido, ObjetosJuego::obtenerTipoPorNombre("cruz"), configuracion.getPuntosCruz(), posicion));
    } else if (value.getName() == "copa") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(
                new Tesoro(idValido, ObjetosJuego::obtenerTipoPorNombre("copa"), configuracion.getPuntosCopa(), posicion));
    } else if (value.getName() == "cofre") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(
                new Tesoro(idValido, ObjetosJuego::obtenerTipoPorNombre("cofre"), configuracion.getPuntosCofre(), posicion));
    } else if (value.getName() == "corona") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(
                new Tesoro(idValido, ObjetosJuego::obtenerTipoPorNombre("corona"), configuracion.getPuntosCorona(), posicion));
    } else if (value.getName() == "llave") {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(new Llave(posicion, idValido));
    } else {
        int idValido = this->crearIdValido();
        this->contenedorDeElementos->agregarElemento(new NoItem(posicion, idValido));
    }

}
Map::Map(Map&& mapa){
    this->rowSize = mapa.rowSize;
    this->colSize = mapa.colSize;
    this->contenedorDeElementos = mapa.contenedorDeElementos;
    mapa.contenedorDeElementos = NULL;
    this->map = mapa.map;
    this->idCargados = mapa.idCargados;
    this->generadorDeId = mapa.generadorDeId;
    this->ladoCelda = mapa.ladoCelda;
}

void Map::agregarElemento(Item *item) {
    this->agregarElemento(item);
}

Item *Map::buscarElemento(int &posx, int &posy) {
    return this->contenedorDeElementos->buscarElemento(posx, posy);
}

bool verificarTipo(int tipo) {
    return (tipo != TYPE_WALL_2 && tipo != TYPE_WALL && tipo != TYPE_WALL_3 &&
            tipo != TYPE_DOOR && tipo != TYPE_EMPTY && tipo != TYPE_FAKE_WALL);
}

void Map::setValue(const unsigned rowNumber, const unsigned colNumber, Type value) {
    this->map[rowNumber][colNumber] = value;
    int tipo = value.getType();
    if (verificarTipo(tipo)) {
        this->crearElementoPosicionable(rowNumber, colNumber, value);
    } else if (tipo == TYPE_DOOR || tipo == TYPE_KEY_DOOR) {
        this->aniadirPuerta(rowNumber, colNumber, tipo);
    }
}

void Map::aniadirPuerta(const unsigned rowNumber, const unsigned colNumber, int tipoPuerta) {
    bool necesitaLlave = !(tipoPuerta == TYPE_DOOR);//documentar
    Posicion pos((colNumber / 2) * this->ladoCelda, (rowNumber / 2) * this->ladoCelda, ANGULO_DEFAULT);
    Puerta puerta(necesitaLlave, pos, rowNumber, colNumber, false);
    this->contenedorDeElementos->aniadirPuerta(puerta);
}

Type &Map::operator()(const unsigned rowNumber, const unsigned colNumber) {
    return this->map[rowNumber][colNumber];
}

Map::~Map() {
    if (contenedorDeElementos != NULL) delete this->contenedorDeElementos;
}

bool Map::hayPuertas() {
    return this->contenedorDeElementos->hayPuertas();
}

Puerta &Map::puertaMasCercana(Posicion &posicionJugador, double &distancia) {
    return this->contenedorDeElementos->puertaMasCercana(posicionJugador, distancia);

}

std::vector<char> Map::serializar() {
    std::vector<char> informacion;
    std::vector<char> aux(4);
    aux = numberToCharArray(this->rowSize);
    informacion.insert(informacion.end(), aux.begin(), aux.end());

    aux = numberToCharArray(this->colSize);
    informacion.insert(informacion.end(), aux.begin(), aux.end());

    for (unsigned i = 0; i < rowSize; i++) {
        for (unsigned j = 0; j < colSize; j++) {
            aux = numberToCharArray((map[i][j]).getType());
            informacion.insert(informacion.end(), aux.begin(), aux.end());
        }
    }

    std::vector<char> contenedorSerializado = this->contenedorDeElementos->serializar();
    informacion.insert(informacion.end(), contenedorSerializado.begin(), contenedorSerializado.end());
    return informacion;
}

void Map::deserializar(std::vector<char> &serializado) {

    std::vector<char> sub(4);
    int idx = 0;
    sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
    this->rowSize = charArrayToNumber(sub);

    idx += 4;
    sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
    this->colSize = charArrayToNumber(sub);
    Map newMap(rowSize, colSize, ladoCelda * rowSize,configuracion);
    this->map = newMap.map;
    for (unsigned i = 0; i < rowSize; i++) {
        for (unsigned j = 0; j < colSize; j++) {
            idx += 4;
            sub = std::vector<char>(&serializado[idx], &serializado[idx + 4]);
            this->map[i][j] = ObjetosJuego::obtenerTipoPorId(charArrayToNumber(sub));
        }
    }

    idx += 4;
    std::vector<char> contenedorDeElementosSerializado(serializado.begin() + idx, serializado.end());
    this->contenedorDeElementos = new ContenedorDeElementos(configuracion);
    this->contenedorDeElementos->deserializar(contenedorDeElementosSerializado);
}

std::vector<Item *> &Map::obtenerItems() {
    return this->contenedorDeElementos->obtenerItems();
}

ContenedorDeElementos &Map::obtenerContenedor() {
    return *this->contenedorDeElementos;
}

bool Map::hayColision(int fila, int columna) {
    try {
        int tamFila = this->rowSize;
        int tamCol = this->colSize;
        if (fila < 0 || fila > tamFila || columna < 0 || columna > tamCol)
            return false;
        int tipo = this->map[fila][columna].getType();
        if (tipo == TYPE_DOOR) {
            Puerta *puerta = this->contenedorDeElementos->obtenerPuertaEn(fila, columna);
            if (puerta->estaAbierta()) {
                tipo = TYPE_EMPTY;
            }
        }
        return (tipo == TYPE_DOOR || tipo == TYPE_WALL || tipo == TYPE_WALL_2 || tipo == TYPE_WALL_3 ||
                tipo == TYPE_KEY_DOOR || tipo == TYPE_FAKE_WALL);
    } catch (std::exception &exc) {
        return false;
    }
}

Posicion Map::obtenerPosicionInicialValida() {
    bool posEsValida = false;
    int posX, posY;
    srand(time(NULL));
    while (!posEsValida) {
        posX = rand() % this->rowSize;
        posY = rand() % this->colSize;
        if (!this->hayColision(posX, posY)) posEsValida = true;
    }
    if (posX == 0) {
        posX = rand() % this->ladoCelda;
    } else {
        posX = posX * (this->ladoCelda - 1) + rand() % this->ladoCelda;
    }
    if (posY == 0) {
        posY = rand() % this->ladoCelda;
    } else {
        posY = posY * (this->ladoCelda - 1) + rand() % this->ladoCelda;
    }

    Posicion posicion(posX, posY, ANGULO_DEFAULT);
    return posicion;
}

void Map::setLadoCelda(int anchoPantalla) {
    this->ladoCelda = anchoPantalla / rowSize;
}

int Map::getLadoCelda() {
    return this->ladoCelda;
}

std::vector<std::vector<int>> Map::getMapanumerico() {
    std::vector<std::vector<int>> mapaNumerico;
    for (unsigned i = 0; i < rowSize; i++) {
        std::vector<int> vector;
        for (unsigned j = 0; j < colSize; j++) {
            vector.push_back(map[i][j].getType());
        }
        mapaNumerico.push_back(vector);
    }
    return mapaNumerico;
}
