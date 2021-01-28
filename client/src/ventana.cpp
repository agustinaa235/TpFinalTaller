#include "../include/ventana.h"
#include <iostream>

Ventana::Ventana(const std::string& titulo, int x, int y, int ancho, int alto, int flags){
    this->ventana = SDL_CreateWindow(titulo.c_str(), x, y, ancho, alto, flags);
    this->render = SDL_CreateRenderer(this->ventana, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!this->ventana){
        throw std::runtime_error("Error: No se creo la ventana");
    }
    SDL_SetRenderDrawColor(this->render, 157, 97, 70, 255);
    SDL_RenderClear(this->render);
}

Ventana::~Ventana(){
    SDL_DestroyRenderer(this->render);
    SDL_DestroyWindow(this->ventana);
}

SDL_Renderer* Ventana::obtener_render(){
    return this->render;
}

void Ventana::renderizar(Textura* texturaInferior){
    //Lienzo posiciontexturaini(0, 0, 800, 40);
    SDL_Rect posiciontexturadest{0, 550, 800, 60};
    texturaInferior->renderizar(NULL, posiciontexturadest);
}

void Ventana::actualizar(){
     SDL_RenderPresent(this->render);
}

void Ventana::limpiar(){
    SDL_RenderClear(this->render);
}
