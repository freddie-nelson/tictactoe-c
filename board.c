#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "globals.h"
#include "win.h"
#include "tile.h"

Tile board[9];
int player = X;
int turn = X;

void nextTurn()
{
  // check winner
  isWin(board, turn);

  if (turn == O)
    turn = X;
  else
    turn = O;

  // TODO implement bot
  // if (turn != player) botPlay();
}

// returns -1 if x or y is out of board bounds
int translateMouseToBoardPos(int x, int y)
{
  if (x < SCREEN_PADDING || x > SCREEN_SIZE - SCREEN_PADDING || y < SCREEN_PADDING || y > SCREEN_SIZE - SCREEN_PADDING)
    return -1;

  x -= SCREEN_PADDING;
  y -= SCREEN_PADDING;

  x /= CELL_SIZE + (LINE_THICKNESS * LINE_COUNT / GRID_SIZE);
  if (x == 3)
    x = 2;

  y /= CELL_SIZE + (LINE_THICKNESS * LINE_COUNT / GRID_SIZE);
  if (y == 3)
    y = 2;

  return x + y * GRID_SIZE;
}

void clickedCell()
{
  int x, y;
  SDL_GetMouseState(&x, &y);
  int i = translateMouseToBoardPos(x, y);
  if (i < 0)
    return;

  if (turn == player)
  {
    board[i] = player;
    nextTurn();
  }
}

void drawLines(SDL_Renderer *renderer)
{
  SDL_Rect lineHL;
  lineHL.w = LINE_THICKNESS;
  lineHL.h = SCREEN_SIZE - SCREEN_PADDING * 2;
  lineHL.x = SCREEN_PADDING + CELL_SIZE;
  lineHL.y = SCREEN_PADDING;

  SDL_Rect lineHR;
  lineHR.w = LINE_THICKNESS;
  lineHR.h = SCREEN_SIZE - SCREEN_PADDING * 2;
  lineHR.x = SCREEN_PADDING + LINE_THICKNESS + CELL_SIZE * 2;
  lineHR.y = SCREEN_PADDING;

  SDL_Rect lineVT;
  lineVT.w = SCREEN_SIZE - SCREEN_PADDING * 2;
  lineVT.h = LINE_THICKNESS;
  lineVT.x = SCREEN_PADDING;
  lineVT.y = SCREEN_PADDING + CELL_SIZE;

  SDL_Rect lineVB;
  lineVB.w = SCREEN_SIZE - SCREEN_PADDING * 2;
  lineVB.h = LINE_THICKNESS;
  lineVB.x = SCREEN_PADDING;
  lineVB.y = SCREEN_PADDING + LINE_THICKNESS + CELL_SIZE * 2;

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderFillRect(renderer, &lineHL);
  SDL_RenderFillRect(renderer, &lineHR);
  SDL_RenderFillRect(renderer, &lineVT);
  SDL_RenderFillRect(renderer, &lineVB);
}

void drawPiece(int i, Tile type, SDL_Renderer *renderer)
{
  if (type == EMPTY)
    return;

  int y = i / 3;
  int x = i - y * 3;

  if (type == O)
  {
    // tl to br
    int lStartX = SCREEN_PADDING + (CELL_SIZE + LINE_THICKNESS) * x + CELL_PADDING;
    int startY = SCREEN_PADDING + (CELL_SIZE + LINE_THICKNESS) * y + CELL_PADDING;
    thickLineRGBA(renderer, lStartX, startY, lStartX + CELL_SIZE - CELL_PADDING * 2, startY + CELL_SIZE - CELL_PADDING * 2, LINE_THICKNESS, 0xFF, 0xFF, 0xFF, 0xFF);

    // tr to bl
    int rStartX = lStartX + CELL_SIZE - CELL_PADDING * 2;
    thickLineRGBA(renderer, rStartX, startY, rStartX - CELL_SIZE + CELL_PADDING * 2, startY + CELL_SIZE - CELL_PADDING * 2, LINE_THICKNESS, 0xFF, 0xFF, 0xFF, 0xFF);
  }
  else
  {
    int centerX = SCREEN_PADDING + (CELL_SIZE + LINE_THICKNESS) * x + CELL_SIZE / 2;
    int centerY = SCREEN_PADDING + (CELL_SIZE + LINE_THICKNESS) * y + CELL_SIZE / 2;

    // draw two circles inside each other to simulate stroke width
    filledCircleRGBA(renderer, centerX, centerY, (CELL_SIZE - CELL_PADDING * 2) / 2, 0xFF, 0xFF, 0xFF, 0xFF);      // outside
    filledCircleRGBA(renderer, centerX, centerY, (CELL_SIZE - CELL_PADDING * 2) / 2 - 20, 0x00, 0x00, 0x00, 0xFF); // inside
  }
}

void drawBoard(SDL_Window *window, SDL_Renderer *renderer)
{
  drawLines(renderer);

  for (size_t i = 0; i < GRID_SIZE * GRID_SIZE; i++)
  {
    if (board[i] != EMPTY)
    {
      drawPiece(i, board[i], renderer);
    }
  }
}