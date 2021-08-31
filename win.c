#include <stdlib.h>
#include "tile.h"

const unsigned wins[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};

// returns player who has won or 0
Tile winner(Tile board[9])
{
  for (size_t i = 0; i < 8; i++)
  {
    if (board[wins[i][0]] != EMPTY && board[wins[i][0]] == board[wins[i][1]] && board[wins[i][1]] == board[wins[i][2]])
      return board[wins[i][0]];
  }

  return EMPTY;
}