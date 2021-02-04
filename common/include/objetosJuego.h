#ifndef _OBJETOS_JUEGO_H
#define _OBJETOS_JUEGO_H

#include <vector>

class ObjetosJuego;

class Type {
public:
    std::string getName() { return this->name; }

    int getType() { return this->type; }

    ~Type() {}

private:
    Type(std::string name, int type) : name(name), type(type) {}

    std::string name;
    int type;

    friend class ObjetosJuego;

};

class ObjetosJuego {
public:
    static std::vector<Type> obtenerObjetos() {
        std::vector<Type> types;
        types.push_back(Type("empty", 0));
        types.push_back(Type("door", 1));
        types.push_back(Type("wall", 2));
        types.push_back(Type("fakeDoor", 3));
        types.push_back(Type("keyDoor", 4));
        types.push_back(Type("comida", 5));
        types.push_back(Type("kitsMedicos", 6));
        types.push_back(Type("sangre", 7));
        types.push_back(Type("balas", 8));
        types.push_back(Type("lanzaCohetes", 9));
        types.push_back(Type("ametralladora", 10));
        types.push_back(Type("canionDeCadena", 11));
        types.push_back(Type("llave", 12));
        types.push_back(Type("tesoro", 13));
        types.push_back(Type("cruz", 14));
        types.push_back(Type("copa", 15));
        types.push_back(Type("cofre", 16));
        types.push_back(Type("corona", 17));
        types.push_back(Type("noItem", 18));
        types.push_back(Type("agua", 19));
        types.push_back(Type("mesa", 20));
        types.push_back(Type("lampara", 21));
        types.push_back(Type("muertoColgante", 22));
        types.push_back(Type("tanque", 23));
        types.push_back(Type("barril", 24));
        types.push_back(Type("planta", 24));
        types.push_back(Type("pistola", 25));
        types.push_back(Type("cuchillo", 27));
        return types;
    }

    static Type obtenerTipoPorId(int id) {
        for (Type t: obtenerObjetos()) {
            if (t.getType() == id)
                return t;
        }
        return Type("noItem", 18);
    }


    static Type obtenerTipoPorNombre(std::string nombre) {
        for (Type t: obtenerObjetos()) {
            if (t.getName() == nombre)
                return t;
        }
        return Type("noItem", 18);
    }

private:
    ObjetosJuego() {}
};

enum class Accion {
    moverse = 0, rotarDerecha = 1, rotarIzquierda = 2, moverArriba = 3, moverAbajo = 4,
    rotar = 5, ataque = 6, aperturaDePuerta = 7, crearPartida = 8,unirseAPartida = 9
};


#endif