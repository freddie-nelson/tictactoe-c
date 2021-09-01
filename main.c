#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "globals.h"
#include "board.h"

TTF_Font *FONT;

SDL_Texture *numTextures[8];

void setupNumbers(SDL_Renderer *renderer)
{
  SDL_Surface *surface;
  SDL_Color textColor = {0, 0, 255, 0};

  for (uint8_t i = 0; i < 8; i++)
  {
    char text[1];
    sprintf(text, "%u", i + 1);

    surface = TTF_RenderText_Solid(FONT, text, textColor);
    if (surface == NULL)
    {
      printf(" %s ", SDL_GetError());
    }

    numTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
    if (numTextures[i] == NULL)
    {
      printf(" %s ", SDL_GetError());
    }
  }

  SDL_FreeSurface(surface);
}

int main()
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Surface *screenSurface = NULL;

  // init png loading
  IMG_Init(IMG_INIT_PNG);

  // load font
  TTF_Init();
  FONT = TTF_OpenFont("font.ttf", 16);
  if (FONT == NULL)
  {
    printf(" could not find font %s ", stderr);
    exit(EXIT_FAILURE);
  }

  bool quit = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    // create window
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
      printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
      return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
      printf("renderer SDL_ERROR: %s\n", SDL_GetError());
    }
    else
    {
      // get window surface
      screenSurface = SDL_GetWindowSurface(window);
      if (screenSurface == NULL)
      {
        printf("screenSurface SDL_ERROR: %s\n", SDL_GetError());
        return 0;
      }

      quit = false;
    }
  }

  initBoard();

  // game loop
  SDL_Event e;

  while (!quit)
  {
    while (SDL_PollEvent(&e) != 0)
    {
      switch (e.type)
      {
      case SDL_QUIT:
      {
        quit = true;
        break;
      }
      case SDL_MOUSEBUTTONUP:
        clickedCell();
        break;
      }
    }

    // clear renderer
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    drawBoard(window, renderer);

    SDL_RenderPresent(renderer);
    SDL_Delay(0);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}