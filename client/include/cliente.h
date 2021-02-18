#ifndef _CLIENTE_H
#define _CLIENTE_H

#include "../../common/include/socket.h"
#include "protected_queue.h"
#include "blocking_queue.h"
#include "comandos/comando.h"
#include <SDL2/SDL.h>
#include <actualizaciones/actualizacion.h>
#include <config.h>


#define ARCHIVO_DE_CONFIGURACION CLIENT_DIR "config.yaml"

class Cliente {

public:
    Cliente(const char *configFile);

    ~Cliente();

    void run();

private:
    std::string configFile = ARCHIVO_DE_CONFIGURACION;
    Socket socket;
    bool corriendo;

};

#endif
