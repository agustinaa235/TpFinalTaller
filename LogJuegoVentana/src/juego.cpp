#include "../include/juego.h"
#include "../include/textura.h"
#include "../include/lienzo.h"
#define EXITO 0


void Juego::inicializar(const char *titulo, int xpos, int ypos, int ancho, int alto, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == EXITO) {
        this->ventana = SDL_CreateWindow(titulo, xpos, ypos, ancho, alto, flags);
        this->render = SDL_CreateRenderer(this->ventana, -1, 0);
        SDL_SetRenderDrawColor(this->render, 157, 97, 70, 255);
        SDL_RenderClear(this->render);
        this->corriendo = true;
    } else {
        this->corriendo = false;
    }
    this->texturaInferior = new Textura("../../LogJuegoVentana/resources/images/ParteInferior.png", this->render);
}

void Juego::handleEvents() {
    SDL_Event evento;
    SDL_PollEvent(&evento);
    switch (evento.type) {
        case SDL_QUIT:
            this->corriendo = false;
            break;

        default:
            break;
    }
}

void Juego::actualizar() {}

void Juego::renderizar() {
    SDL_RenderClear(this->render);
    Lienzo posiciontexturaini(0, 0, 300, 300);
    Lienzo posiciontexturadest(100, 100, 300, 300);
    this->texturaInferior->renderizar(posiciontexturaini, posiciontexturadest);
    SDL_RenderPresent(this->render);
}

void Juego::clean() {
    SDL_DestroyWindow(this->ventana);
    SDL_DestroyRenderer(this->render);
    SDL_Quit();
}
