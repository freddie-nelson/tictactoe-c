#ifndef BOARDH
#define BOARDH

#include <SDL2/SDL.h>

void initBoard();

void nextTurn();
void clickedCell();

void drawBoard(SDL_Window *window, SDL_Renderer *renderer);

#endif