#ifndef _LOGIN_H
#define _LOGIN_H

#include <SDL2/SDL.h>
#include "../include/fonts.h"
#include "socket.h"
#include "protocolo.h"

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720

class LogInWindow {

public:
    LogInWindow();

    void run();

    ~LogInWindow();

    Protocolo *obtenerProtocolo() {
        return this->protocolo;
    }

    uint32_t obtenerIdJugador() {
        return this->idCliente;
    }

private:
    SDL_Renderer *renderer;
    SDL_Window *window;
    Fonts fonts;
    Socket socket;//hay qeu sacarlo
    Protocolo *protocolo;
    uint32_t idCliente;

};

#endif
