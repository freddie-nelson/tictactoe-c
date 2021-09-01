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

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // Create a window usable with OpenGL context
  window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  // Select render driver
  // - A render driver that supports HW acceleration is used when available
  // - Otherwise a render driver supporting software fallback is selected
  SDL_RendererInfo renderDriverInfo;
  uint32_t rendererFlags = SDL_RENDERER_TARGETTEXTURE;
  int32_t nbRenderDrivers = SDL_GetNumRenderDrivers(), index = 0;
  if (nbRenderDrivers < 0)
  {
    exit(EXIT_FAILURE);
  }

  while (index < nbRenderDrivers)
  {
    if (SDL_GetRenderDriverInfo(index, &renderDriverInfo) == 0)
    {
      if (((renderDriverInfo.flags & rendererFlags) == rendererFlags) && ((renderDriverInfo.flags & SDL_RENDERER_ACCELERATED) == SDL_RENDERER_ACCELERATED))
      {
        // Using render driver with HW acceleration
        rendererFlags |= SDL_RENDERER_ACCELERATED;
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, renderDriverInfo.name);
        break;
      }
    }
    ++index;
  }

  if (index == nbRenderDrivers)
  {
    // Let SDL use the first render driver supporting software fallback
    rendererFlags |= SDL_RENDERER_SOFTWARE;
    index = -1;
  }

  // Create renderer
  renderer = SDL_CreateRenderer(window, index, rendererFlags);
  if (renderer == NULL)
  {
    printf("renderer SDL_Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  initBoard();

  // game loop
  bool quit = false;
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

  return EXIT_SUCCESS;
}